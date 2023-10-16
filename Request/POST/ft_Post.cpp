#include "ft_Post.hpp"


std::string* grab_method(std::string& req)
{  
    
    std::string *ret = new std::string();
    *ret = req.substr(0, req.find(" "));
    return ret;
}

std::string* convert_to_str(size_t len)
{
    std::stringstream ss;
    ss << len;
    std::string *ret = new std::string();
    ss >> *ret;
    return ret;
}

std::string* get_body(t_client_info *client, Directives &working)
{
    std::string filename;
    filename = working.getErrorPages()[client->header.status];
    std::ifstream file;
    file.open(filename.c_str());
    std::string *body = new std::string;
    std::string line;
    while(getline(file, line))
        body->append(line);
    return body;
}


void response(t_client_info* client, std::vector<int> clientSockets, std::vector<Directives> &servers)
{
    (void)servers;
    (void)clientSockets;
    std::string res_total;
    
    std::string *body = NULL;
    std::string *cl = NULL;
    std::string headers_cgi;
    std::string body_cgi;
    if (client->header.isError)
    {
        body = get_body(client, client->directive);
        cl = convert_to_str((*body).length());
    }
    else if (client->isRedirection && !client->header.isError)
    {
        res_total = client->header.statuscode + "\r\n"\
        + "Location: " + client->working_location.getReturn()[1] + "\r\n";
    }
    else if (client->cgi && !client->header.isError)
    {
            try
            {
                body = handle_cgi(client, client->directive);
            }
            catch(std::exception& e)
            {
                delete body;
                if (strcmp(e.what(), "Child cgi exited") == 0)
                {
                // std::cout << "dlkhl liha" << std::endl;
                client->header.status = "502";
                client->header.statuscode = "HTTP/1.1 502 Gateway timeout";
                }
                else
                {
                // std::cout << "dlkhl liha" << std::endl;
                    client->header.status = "504";
                    client->header.statuscode = "HTTP/1.1 504 Gateway timeout";
                }
                body = get_body(client, client->directive);
                client->header.isError = true;
            }
            cl = convert_to_str((*body).length());
    }
    else
    {
        body = new std::string("");
        cl = new std::string("0");
    }
    if (client->cgi && !client->isRedirection && !client->header.isError)
    {
        size_t breack = (*body).find("\r\n\r\n");
        if (breack != std::string::npos)
        {
            headers_cgi = (*body).substr(0, breack);
            body_cgi = (*body).substr(breack + 4);
        }
        else
        {
            headers_cgi = "";
            body_cgi = body->substr(0, 5);
        }  
        cl = convert_to_str((body_cgi).length());
        res_total = client->header.statuscode + "\r\n"\
        + "Content-Type: text/html\r\n" \
        + "Content-Length:" + *cl + "\r\n" \
        + headers_cgi + ((headers_cgi != "") ? "\r\n\r\n" : "\r\n") \
        + body_cgi;
        // std::cout << res_total << std::endl; 
    }
    else if (!client->isRedirection)
    {
        res_total = client->header.statuscode + "\r\n"\
        "Date: Thu, 19 Feb 2009 12:27:04 GMT\r\n" \
        "Server: 0rph1no/1.1\r\n" \
        "Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\r\n" \
        "ETag: \"56d-9989200-1132c580\"\r\n" \
        "Content-Type: text/html\r\n" \
        "Content-Length: "+*cl+"\r\n" \
        "Accept-Ranges: bytes\r\n" \
        "Connection: close\r\n" \
        "\r\n" \
        + *body;
        std::cerr << res_total << std::endl;
    }
    const char *res = res_total.c_str();
    send(client->socket, res, strlen(res), 0);
    if (cl)
        delete cl;
    if (body)
        delete body;
}

std::string grab_path(std::string& req)
{  
    //GET /HAMID HTTP/1.1
    std::string ret;
    req = req.substr(0, req.find("\r\n"));
    ret = req.substr(req.find(" ")+1, (req.find("HTTP/1.1") - 6));
    return ret;
}
std::string grab_path_dir(std::string& req)
{  
    std::string ret;
    if (req == "/")
        ret = req;
    else
    {
        size_t found = req.rfind("/");
        if (found != std::string::npos && found != 0)
        {
            ret = req.substr(0, req.rfind("/"));
        }
        else
        {
            ret = req;
        }
    }
    return ret;
}

std::string get_header_value(t_client_info* client, const char *name, size_t len)
{
    std::string ret;
    // std::cout << client->fst_req << std::endl;
    size_t found = client->fst_req.find(name);
    if (found == std::string::npos)
        ret = "";
    else if (client->is_multipart)
        ret = client->fst_req.substr(found+len, client->fst_req.find(";",found+len) - found-len);
    else
         ret = client->fst_req.substr(found+len, client->fst_req.find("\r\n",found+len) - found-len);
    return ret;
}

std::string grab_filename_from_url(std::string& req)
{
    std::string ret;
    req = req.substr(0, req.find("\r\n"));
    req = req.substr(req.find(" ")+1, (req.find("HTTP/1.1") - 6));
    ret = req.substr(req.rfind("/")+1);
    return ret;
}

std::string get_file_ext(std::string& req)
{
    // PHP_SELF=/cgi-bin/test.php
    std::string ret;
    req = req.substr(req.rfind("/"));
    ret = req.substr(req.rfind("."));
    return ret;
}

std::string* handle_cgi(t_client_info *client, Directives& working)
{
        
        std::string *ret = new std::string();
        std::string filename = client->header.path_p.substr(client->header.path_p.rfind("/"));
        (void)working;
        int fd[2];
        pid_t pid;
        char *argv[3];
        char *envp[13];
        char buf[1024];
        std::string str;
        pipe(fd);
        pid = fork();
        client->Info->start = time(NULL);
        if (pid == 0)
        {
            filename = "cgi-bin"+filename;
            std::string Content_Type;
            std::string Content_Length;
            std::string Url_Path;
            std::string Path_Info;
            std::string Server_Name;
            std::string PHP_SELF;
            std::string Extenstion;
            // std::string *ext = get_ext(Content_Type);
            PHP_SELF = "PHP_SELF="+grab_path(client->fst_req);
            Extenstion =  get_file_ext(PHP_SELF);
            Content_Type = "Content_Type=" + get_header_value(client, "Content-Type", 14);
            Content_Length = "Content-Length=" +  std::to_string(get_length(client->fst_req));
            Url_Path =  "SCRIPT_FILENAME=" + client->working_location.getRoot() +  "/" +grab_filename_from_url(client->fst_req);
            Path_Info = "Path_Info="+ client->working_location.getCgi()[Extenstion];
            Server_Name = "Server_Name=" + working.getServerName();
            std::cout << Url_Path << std::endl;
            close(fd[0]);
            dup2(fd[1], 1);
            close(fd[1]);
            envp[0] = strdup("REQUEST_METHOD=POST");
            envp[1] = strdup("REDIRECT_STATUS=200");
            envp[2] = strdup(Content_Type.c_str());
            envp[3] = strdup(Content_Length.c_str());
            envp[4] = strdup(Url_Path.c_str());
            envp[5] = strdup(Path_Info.c_str());
            envp[6] = strdup(Server_Name.c_str());
            envp[7] = strdup(PHP_SELF.c_str());
            envp[8] = NULL;
            argv[0] = strdup(client->working_location.getCgi()[Extenstion].c_str());
            argv[1] = strdup(filename.c_str());
            argv[2] = NULL;
            execve(argv[0], argv, envp);
            exit(0);
        }
        close(fd[1]);
        int flag = 0;
        int status;
        time_t now = time(NULL);
        while(1)
        {
            if (difftime(now, client->Info->start) > 5)
            {
                flag = 1;
                break;
            }
            now = time(NULL);
            waitpid(pid, &status, WNOHANG);
            if (WIFEXITED(status))
            {
                if (WEXITSTATUS(status) != 0)
                {
                    flag = 2;
                    break;
                }
                flag = 0;
                break;
            }
        }
        if (flag == 1)
        {
            kill(pid, SIGKILL);
            throw std::runtime_error("Child cgi hanged");
        }
        else if (flag == 2)
        {
            kill(pid, SIGKILL);
            throw std::runtime_error("Child cgi exited");
        }
        bzero(buf, sizeof(buf));
        read(fd[0], buf, 1023);
        *ret = buf;
        if (ret->find("Content-Length") != std::string::npos)
        {
            size_t val = get_length(*ret);
            *ret = (*ret).substr(0,val);
        }
        return ret;
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
            // printf("{%ld}\n", sep);
            return atoi((line.substr(sep,line.length())).c_str());
        }
    }
    return -1;
}
std::string get_filename(std::string& req)
{
    // std::cout << req.find("filename=")+10 << "-----" << req.find(".mp4") << std::endl;
    int file_index = req.find("filename=");
    std::string temp;
    temp = req.substr(file_index+10, req.find("\"", file_index+10)-file_index-10);
    return temp;
}
std::string generate_filename(std::string& ext)
{
    std::string result;
    char alpha[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',
                          'o', 'p', 'q', 'r', 's', 't', 'u',
                          'v', 'w', 'x', 'y', 'z' };
    for (int i = 0; i<26; i++)
        result = result + alpha[rand() % 26];
    result = result+"."+ext;
    return result;
}
std::string* get_ext(std::string& req)
{
    std::string *ret = new std::string();
    int ctype = req.find("Content-Type:");
    *ret = req.substr(ctype+14, req.find("\r\n",ctype+1)-(ctype+14));
    *ret = (*ret).substr((*ret).find("/")+1);
    if (*ret == "x-httpd-php")
        *ret = "php";
    return ret;
}

void handle_content_length(t_client_info* client ,std::string& req_body, int binary_data_start)
{
        std::ofstream img;
        const char* data;
        std::string upload = client->working_location.getUploadStore() + "/" +client->header.filename;
        // std::cout << upload << std::endl;
        img.open(upload.c_str(), std::ios::binary);
        if (img.is_open())
        {
            size_t isthere = req_body.find("----------------------------",binary_data_start);
            if (client->is_multipart && isthere != std::string::npos)
            {
                req_body = req_body.substr(0, isthere);
            }
            data = req_body.c_str() + binary_data_start;
            img.write(data, req_body.length() - binary_data_start);
        }
        else
        {
            std::cerr << "Error: Could not open file for writing." << std::endl;
        }
        img.close();
}

void handle_chunked_encoding(t_client_info* client, std::string& chunked_data) {
    std::ofstream img;
    std::string chunk_size_str;
    size_t chunk_size;
    std::string upload = client->working_location.getUploadStore() + "/" +client->header.filename;
    img.open(upload.c_str(), std::ios::binary);
    size_t pos = 0;
    size_t isthere = chunked_data.find("----------------");
    if (client->is_multipart && isthere != std::string::npos)
    {
        chunked_data = chunked_data.substr(0, isthere);
        chunked_data += "0\r\n\r\n";
    }
    // std::cout << chunked_data << std::endl;
    // std::cout << chunked_data << std::endl;
    // std::cout << chunked_data.length() << std::endl;
    while (pos < chunked_data.length()) {
        size_t chunk_size_end = chunked_data.find("\r\n", pos);
        if (chunk_size_end == std::string::npos)
            break;

        chunk_size_str = chunked_data.substr(pos, chunk_size_end - pos);
        chunk_size = hextodec(chunk_size_str.c_str());
        if (chunk_size == 0 || chunk_size_str.find("--------") != std::string::npos)
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
    size_t start_pos;
    if (client->received)
        client->req_body.append(client->request, client->received);
    if (client->times == 0)
    {
        client->header.fst_line = client->req_body.substr(0, client->req_body.find("\r\n"));
        client->fst_req = client->req_body;
    }
    if (client->req_body.find("Content-Length") != std::string::npos && client->times == 0) {
        if (client->req_body.find("\r\n\r\n") + 5 > (size_t)client->received)
            return 4;
        client->is_content_length = true;
    }
    if (client->is_content_length)
    {
        if (client->times == 0)
        {
            client->bl = get_length(client->req_body);
            if (client->req_body.find("multipart/form-data") != std::string::npos)
            {
                client->binary_data_start = client->req_body.rfind("\r\n\r\n") + 4;
                client->is_multipart = true;
                client->header.filename = get_filename(client->req_body);
            }
            else
            {
                std::string* ext = get_ext(client->req_body);
                client->header.filename = generate_filename(*ext);
                delete ext;
                client->binary_data_start = client->req_body.find("\r\n\r\n") + 4;
            }
        }        // client->req_body.clear();
    }
    if (!client->is_chunked_encoding) {
        if (client->req_body.find("Transfer-Encoding: chunked") != std::string::npos) {
            if (client->req_body.find("\r\n\r\n") + 10 > (size_t)client->received)
                return 4;
            if (client->req_body.find("multipart/form-data") != std::string::npos)
            {
                client->header.filename = get_filename(client->req_body);
                start_pos = client->req_body.rfind("\r\n\r\n") + 4;
                client->is_multipart = true;
            }
            else
            {
                start_pos = client->req_body.find("\r\n\r\n") + 4;
                std::string* ext = get_ext(client->req_body);
                client->header.filename = generate_filename(*ext);
                delete ext;
            }
            client->is_chunked_encoding = true;
            client->req_body = client->req_body.substr(start_pos);
        }
    }
    // std::cout << client->bl << " ------ " << client->req_body << std::endl;
    // client->binary_data_start = 0;
    client->times++;
    if (client->is_chunked_encoding && client->req_body.find("\r\n0\r\n\r\n") != std::string::npos) {
        return 3;
    }
    if (client->is_content_length && client->all_received >= client->bl)
        return 0;
    if (client->is_chunked_encoding)
        return 1;
    if (client->is_content_length)
        return 2;
    return 2;
}

bool is_Req_Err(Locations& loc, t_client_info *client, Directives &working)
{
    std::string temp = client->request;
    // std::cout << client->socket << " ---- " << loc.getLocation() << std::endl;
    if (client->times == 0)
    {
        if (loc.getLocation() == "")
        {
            client->times++;
            client->header.isError = true;
            client->header.status = "403";
            client->header.statuscode = "HTTP/1.1 403 Forbidden";
            return true;
        }
        if (loc.getAcceptedMethods()["POST"] == false)
        {
            client->times++;
            client->header.isError = true;
            client->header.status = "405";
            client->header.statuscode = "HTTP/1.1 405 Method Not Allowed";
            return true;
        }
        if (temp.find("Content-Length") != std::string::npos && temp.find("Transfer-Encoding") != std::string::npos)
        {
            client->times++;
            client->header.isError = true;
            client->header.status = "400";
            client->header.statuscode = "HTTP/1.1 400 Bad Request";
            return true;
        }
    }
    if (temp.find("Content-Length") != std::string::npos && client->times == 0)
    {
        int length = get_length(temp);
        if (length > working.getMaxBodySizeInBytes())
        {
            client->times++;
            client->header.isError = true;
            client->header.status = "413";
            client->header.statuscode = "HTTP/1.1 419 Entity Too Big";
            return true;
        }
    }
    else
    {
        if (client->all_received > (size_t)working.getMaxBodySizeInBytes())
        {
            // std::cout << client->all_received << " ----- "<< (size_t)working.getMaxBodySizeInBytes() << std::endl;
            client->times++;
            client->header.isError = true;
            client->header.status = "413";
            client->header.statuscode = "HTTP/1.1 413 Entity Too Big";
            return true;
        }
    }
    return false;
}

int handle_Post(std::vector<int> &clientSockets, std::vector<Directives> &servers, t_client_info *client)
{
    (void)clientSockets;
    if (client->times == 0)
    {
        std::string temp = client->request;
        client->header.file_path = grab_path(temp);
        client->header.path_dir = grab_path_dir(client->header.file_path);
        client->header.path_p = grab_path(temp);
        if (client->header.path_dir == "/sessions")
            client->isSession = true;
        client->directive = servers[client->serverIndex];
        std::vector<Locations> locations = client->directive.getLocationsVec();
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (client->header.path_dir == locations[i].getLocation())
            {
                client->working_location = locations[i];
                break;
            }
        }
    }
        if (client->header.file_path.find(".py") != std::string::npos || client->header.file_path.find(".php") != std::string::npos)
        {
            client->cgi = true;
        }
        int ret;
        if (is_Req_Err(client->working_location, client, client->directive))
            return 3;
        if (client->working_location.getReturn().size() > 0)
        {
            client->isRedirection = true;
            client->header.isError = false;
            client->header.status = client->working_location.getReturn()[0];
            client->header.statuscode = "HTTP/1.1 " + client->header.status + " Redirected";
            return 5;
        }
        ret = ft_my_Post(client);
        if (ret == 4)
        {
            client->header.isError = false;
            client->header.status = "201";
            client->header.statuscode = "HTTP/1.1 201 Created";
            return 4;
        }
	    if (ret == 3)
        {
		    handle_chunked_encoding(client, client->req_body);
            client->header.isError = false;
            client->header.status = "201";
            client->header.statuscode = "HTTP/1.1 201 Created";
            return 0;
        }
        if (ret == 0)
        {
            handle_content_length(client, client->req_body,client->binary_data_start);
            client->header.isError = false;
            client->header.status = "201";
            client->header.statuscode = "HTTP/1.1 201 Created";
            return 1;
        }
    return 2;
}

Request::~Request()
{
}