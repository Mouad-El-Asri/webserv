#include "ft_Post.hpp"


std::string& grab_method(std::string& req)
{  
    
    std::string *ret = new std::string();
    *ret = req.substr(0, req.find(" "));
    return *ret;
}

std::string& convert_to_str(size_t len)
{
    std::stringstream ss;
    ss << len;
    std::string *ret = new std::string();
    ss >> *ret;
    return *ret;
}

std::string* get_body()
{
    std::ifstream file;
    file.open("/nfs/homes/ceddibao/Desktop/projects/webserv/index.html");
    std::string *body = new std::string;
    std::string line;
    while(getline(file, line))
        body->append(line);
    return body;
}
void response(ssize_t csock_fd)
{
    std::string *body = get_body();
    std::string res_total = "HTTP/1.1 200 OK\r\n" \
    "Date: Thu, 19 Feb 2009 12:27:04 GMT\r\n" \
    "Server: 0rph1no/1.1\r\n" \
    "Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\r\n" \
    "ETag: \"56d-9989200-1132c580\"\r\n" \
    "Content-Type: text/html\r\n" \
    "Content-Length:"+convert_to_str((*body).length())+"\r\n" \
    "Accept-Ranges: bytes\r\n" \
    "Connection: close\r\n" \
    "\r\n" \
    + *body;
    const char *res = res_total.c_str();
    send(csock_fd, res, strlen(res), 0);
}

std::string& grab_path(std::string& req)
{  
    //GET /HAMID HTTP/1.1
    std::string *ret = new std::string();
    req = req.substr(0, req.find("\r\n"));
    *ret = req.substr(req.find(" ")+1, (req.find("HTTP/1.1") - 5));
    return *ret;
}

std::string* handle_cgi()
{
    // int fds[2];
    // int ret = pipe(fds);
    // if (ret < 0)
    // {
    //     perror("pipe");
    //     exit(3);
    // }
    // pid_t child = fork();
    // if (ret < 0)
    // {
    //     perror("child");
    //     exit(2);
    // } 
    // dup2(1, fds[1]);
    // if (child == 0)
    // {
    //     char *cmds[3] = {"/bin/php", "test.php", NULL};
    //     ret = execve("/bin/php",cmds,NULL);
    //     std::cout << "sdfdsfdsfdsfdsfsdfsdfsdfsdfdsfsdfdsfdsfsdf\n";
    //     exit(0);
    // }
    // close(fds[1]);
    // close(fds[0]);
    // wait(NULL);
    // char buff[1000];
    // bzero(buff,sizeof(buff));
    // int readen = read(fds[0], buff, 999);
    // std::string *ret_str = new std::string();
    // ret_str->append(buff, readen);
    // return ret_str;
    return NULL;
}

size_t hextodec(const char *s)
{
    std::string str = s;
    int i;
    
    std::istringstream iss(str);
    iss >> std::hex >> i;
    return i;

}
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

void handle_chunked_encoding(const std::string& chunked_data) {
    std::ofstream img;
    std::string chunk_size_str;
    size_t chunk_size;
    img.open("image.mp4", std::ios::binary | std::ios::app);

    size_t pos = 0;
    while (pos < chunked_data.length()) {
        size_t chunk_size_end = chunked_data.find("\r\n", pos);
        if (chunk_size_end == std::string::npos)
            break;

        chunk_size_str = chunked_data.substr(pos, chunk_size_end - pos);
        chunk_size = hextodec(chunk_size_str.c_str());
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




int ft_my_Post(t_client_info *client)
{
    client->all_received+=client->received;
        
    client->req_body.append(client->request, client->received);
    if (client->req_body.find("Content-Length") != std::string::npos && client->times == 0) {
        if (client->req_body.find("\r\n\r\n") + 5 > (size_t)client->received)
            return 0;
        client->is_content_length = true;
    }
    if (client->is_content_length)
    {
        if (client->times == 0)
        {
            client->bl = get_length(client->req_body);
            client->binary_data_start = client->req_body.find("\r\n\r\n") + 4;
        }
        handle_content_length(client->req_body,client->binary_data_start);
        client->req_body.clear();
    }
    if (!client->is_chunked_encoding) {
        if (client->req_body.find("Transfer-Encoding: chunked") != std::string::npos) {
            if (client->req_body.find("\r\n\r\n") + 10 > (size_t)client->received)
                return 0;
            client->is_chunked_encoding = true;
            size_t start_pos = client->req_body.find("\r\n\r\n") + 4;
            client->req_body = client->req_body.substr(start_pos);
        }
    }
    client->binary_data_start = 0;
    client->times++;
    if (client->is_chunked_encoding && client->req_body.find("\r\n0\r\n\r\n") != std::string::npos) {
        return 1;
    }
    if (client->is_content_length && client->all_received >= client->bl)
        return 0;
    return 0;
}

void handle_Post(t_client_info *client)
{
    std::string temp;
    temp.append(client->request, sizeof(client->request));
    if (client->times == 0)
		client->header.method = grab_method(temp);
    if (client->header.method == "POST")
    {
	    if (ft_my_Post(client))
		    handle_chunked_encoding(client->req_body);
        return;
    }
}

Request::Request()
{
}

Request::~Request()
{
}