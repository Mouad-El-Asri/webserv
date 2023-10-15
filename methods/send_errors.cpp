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

void	method_get::set_error(const char  *str)
{
	struct stat st;
	std::stringstream	ss;
	std::string			ext;
	this ->path  = erros_page[str];
	infa.file = new std::ifstream(path.c_str(),  std::ios::binary);
	if (!infa.file->is_open())
	{
		infa.buffer_to_send = "HTTP/1.1 500 Internal Server Error\r\nContent-Lenght: 0\r\n\r\n";
		infa.status = 1;
		infa.file->close();
		infa.file = NULL;
		return;
	}
	stat(path.c_str() , &st);
	this->size = 		st.st_size;
	infa.size = 		st.st_size;
	ext = extansion_handling[str];
	ss << this->size;
	ext += ss.str();
	ext += "\r\n\r\n";
	this->infa.buffer_to_send = ext;
	this->infa.status = 1;
}
