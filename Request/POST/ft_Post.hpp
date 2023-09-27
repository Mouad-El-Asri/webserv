#ifndef FT_POST_HPP
#define FT_POST_HPP

#include <string>
#include <sys/socket.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <poll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../Multiplexing/multiplexing.hpp"

int ft_my_Post(t_client_info *client);
std::string* get_body();
void response(ssize_t csock_fd);
std::string& grab_path(std::string& req);
std::string* handle_cgi();
size_t hextodec(const char *s);
ssize_t get_length(std::string& req);
void handle_content_length(std::string& req_body, int binary_data_start);
void handle_chunked_encoding(const std::string& chunked_data);
std::string& grab_method(std::string& req);
void handle_Post(t_client_info *client);

#endif