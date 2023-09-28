#include "get.hpp"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

typedef struct sockaddr_in sock_addr;
ssize_t get_length(std::string& req)
{

    std::istringstream s;
    s.str(req);
    std::string line;
    while(std::getline(s, line))
    {
        if (line.find("Content-Length") != std::string::npos)
        {
            size_t sep = line.find(":") + 2;
            printf("{%ld}\n", sep);
            return atoi((line.substr(sep,line.length())).c_str());
        }
    }
    return -1;
}
void handle_content_length(std::string& req_body, int binary_data_start)
{
        std::ofstream img;
        const char* data;
        img.open("image.mp4", std::ios::binary | std::ios::app);

            // Write the binary data to the "img.png" file
        if (img.is_open())
        {
            data = req_body.c_str() + binary_data_start;
            img.write(data, req_body.length() - binary_data_start);
        }
        else
        {
            std::cerr << "Error: Could not open img.png for writing." << std::endl;
        }
        img.close();
}
std::string* get_body()
{
    std::ifstream file;
    file.open("index.html");
    std::string *body = new std::string;
    std::string line;
    while(getline(file, line))
        body->append(line);
    return body;
}

void handle_chunked_encoding(const std::string& chunked_data) {
    std::ofstream img;
    img.open("image.mp4", std::ios::binary | std::ios::app);

    size_t pos = 0;
    while (pos < chunked_data.length()) {
        size_t chunk_size_end = chunked_data.find("\r\n", pos);
        if (chunk_size_end == std::string::npos)
            break;

        std::string chunk_size_str = chunked_data.substr(pos, chunk_size_end - pos);
        size_t chunk_size = strtol(chunk_size_str.c_str(), nullptr, 16);
        if (chunk_size == 0)
            break;

        pos = chunk_size_end + 2; // Move past "\r\n"
        if (pos + chunk_size > chunked_data.length())
            break;

        img.write(chunked_data.c_str() + pos, chunk_size);
        pos += chunk_size + 2; // Move past chunk data and "\r\n"
    }

    img.close();
}

int main() {
    std::string *body = get_body();
    std::string res_total = "HTTP/1.1 200 OK\r\n" \
    "Date: Thu, 19 Feb 2009 12:27:04 GMT\r\n" \
    "Server: Apache/2.2.3\r\n" \
    "Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\r\n" \
    "ETag: \"56d-9989200-1132c580\"\r\n" \
    "Content-Type: text/html\r\n" \
    "Content-Length:"+std::to_string((*body).length())+"\r\n" \
    "Accept-Ranges: bytes\r\n" \
    "Connection: close\r\n" \
    "\r\n" \
    + *body;
    const char *res = res_total.c_str();
    int option_value = 1;
    int i = 0;
    sock_addr server;
    bzero(&server,sizeof(server));
    sock_addr client;
    char *buf;
    std::string req_body;
    size_t allread = 0;
    size_t bl = 0;
    size_t binary_data_start;

    const char* remote_host = "127.0.0.1";

    int csock_fd;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = setsockopt(sock_fd, SOL_SOCKET,SO_REUSEADDR, &option_value,sizeof(option_value));
    if (sock_fd <0  || ret < 0) {
        perror("socket");
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(CONNECTION_PORT);
    inet_aton(remote_host, (in_addr*)&server.sin_addr.s_addr);
    memset(&(server.sin_zero), '\0', 8);
    int bind_s = bind(sock_fd, (struct sockaddr*)&server,sizeof(server));
    if (bind_s < 0) {
        perror("bind");
        exit(1);
    }
    bind_s = listen(sock_fd,4);
    if (bind_s < 0) {
        perror("couldnt listen to connections");
        exit(1);
    }
    socklen_t length_of_connection = sizeof(client);
    ssize_t readen = 1;
    buf = new char[65536]();

    req_body.clear();
    csock_fd = accept(sock_fd,(struct sockaddr*)&client,&length_of_connection);
    bool is_chunked_encoding = false;
    bool is_content_length = false;
    while (1) {
        bzero(buf, sizeof(buf));
        if (csock_fd < 1) {
            perror("client socket");
            exit(1);
        }
        readen = recv(csock_fd, buf, 65536, 0);
        allread+=readen;
        if (readen < 1)
            break;
        
        req_body.append(buf, readen);
        if (req_body.find("Content-Length") != std::string::npos && i == 0) {
            is_content_length = true;
        }
        if (is_content_length)
        {
            if (i == 0)
            {
                bl = get_length(req_body);
                binary_data_start = req_body.find("\r\n\r\n") + 4;
                i++;
            }
            handle_content_length(req_body,binary_data_start);
            req_body.clear();
        }
        if (!is_chunked_encoding) {
            // Check if the request uses chunked encoding
            if (req_body.find("Transfer-Encoding: chunked") != std::string::npos) {
                is_chunked_encoding = true;
                size_t start_pos = req_body.find("\r\n\r\n") + 4;
                req_body = req_body.substr(start_pos);
            }
        }
        binary_data_start = 0;
        if (is_chunked_encoding && req_body.find("\r\n0\r\n\r\n") != std::string::npos) {
            break;
        }
        if (is_content_length && allread >= bl)
            break;
    }

    if (is_chunked_encoding) {
        handle_chunked_encoding(req_body);
    }
    send(csock_fd, res, strlen(res), 0);


    delete [] buf;
    close(csock_fd);
    close(sock_fd);
    return 0;
}