/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_errors.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouzanb <abouzanb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 03:57:31 by abouzanb          #+#    #+#             */
/*   Updated: 2023/10/09 06:28:35 by abouzanb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "get.hpp"

void method_get::set_error_500()
{
	std::stringstream ss ;
	std::string ext;
	std::string body = "<head><title>500 Internal Server Error</title></head><body><center><h1>500 Internal Server Error</h1></center><hr><center>Elhazin server</center></body></html>";
	infa.buffer_to_send = extansion_handling["500"];
	ss << body.size();
	infa.buffer_to_send += ss.str();
	infa.buffer_to_send += "\r\n\r\n";
	infa.buffer_to_send += body;
	infa.status = 1;
}

void	method_get::set_error_403()
{
	std::stringstream ss ;
	std::string ext;
	struct stat st;
	this ->path  = erros_page["403"];
	this->file = new std::ifstream(path.c_str(),  std::ios::binary);
	if (!file->is_open())
	{
		this->set_error_500();
		infa.status = 1;
		return;
	}
	stat(path.c_str() , &st);
	this->size = st.st_size;
	infa.size = st.st_size;
	ext = extansion_handling["403"];
	ss << this->size;
	ext += ss.str();
	ext += "\r\n\r\n";
	this->infa.buffer_to_send = ext;
	this->infa.status = 1;
	infa.file = file;
}

void	method_get::set_error_404()
{
	struct stat st;
	std::stringstream	ss;
	std::string			ext;
	std::cout << "url : " << url << std::endl;
	std::cout << "path : " << path << std::endl;
	this ->path  = erros_page["404"];
	infa.file = new std::ifstream(path.c_str(),  std::ios::binary);
	if (!infa.file->is_open())
	{
		this->set_error_500();
		return;
	}
	stat(path.c_str() , &st);
	this->size = 		st.st_size;
	infa.size = 		st.st_size;
	ext = extansion_handling["404"];
	ss << this->size;
	ext += ss.str();
	ext += "\r\n\r\n";
	this->infa.buffer_to_send = ext;
	this->infa.status = 1;
}


void method_get::set_not_allowed()
{
	std::stringstream ss;
	std::string body = "<head><title>405 Method Not Allowed</title></head><body><center><h1>405 Method Not Allowed</h1></center><hr></body></html>";
	infa.buffer_to_send  = extansion_handling["405"];
	ss << body.size();
	infa.buffer_to_send += ss.str();
	infa.buffer_to_send += "\r\n\r\n";
	infa.buffer_to_send += body;
	infa.status = 1;
}