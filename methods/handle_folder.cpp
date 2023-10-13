/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_folder.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouzanb <abouzanb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 03:57:04 by abouzanb          #+#    #+#             */
/*   Updated: 2023/10/09 06:32:24 by abouzanb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get.hpp"


void method_get::send_indexing(DIR *dir)
{
    struct dirent *dp;
    std::ofstream fill("./indexing.html");
    struct stat st;
    if (!fill.is_open())
    {
        // std::cout << "error in opening file" << std::endl;
        return;
    }
    fill << "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " << url << "</title>\n";
    fill << "<style> body {background-color: #f4f4f4; font-size: 16px; font-family: Arial, sans-serif;}\n";
    fill << "h1 {color: #333333;}\n";
    fill << "a {text-decoration: none; color: #007bff;}\n";
    fill << "a:hover {text-decoration: underline;}\n";
    fill << "pre {font-family: 'Courier New', monospace;}</style>\n";
    fill << "</head>\n<body>\n";
    fill << "<h1>Index of " << url << "</h1>\n";
    fill << "<hr>\n<pre>\n";

    while ((dp = readdir(dir)) != NULL)
    {
        std::string temp = dp->d_name;
        if (temp == "." || temp == "..")
            continue;
        if (dp->d_type == DT_DIR)
            fill << "<a href=\"" << temp << "/\">" << temp << "/</a>\n";
        else
            fill << "<a href=\"" << temp << "\">" << temp << "</a>\n";
    }
    fill << "</pre><hr>\n</body>\n</html>\n";
    closedir(dir);
    fill.close();
    this->path = "./indexing.html";
    stat(path.c_str(), &st);
    this->size = st.st_size;
    file_handling();
    std::runtime_error("\e[91mFolder is requested, response with indexing its content\e[0m");
}

void	method_get::handle_auto_index()
{

	DIR	*dir;
	if	((dir = opendir(path.c_str())) == NULL)
		set_error("404");
	else
	{
		if (this->path[this->path.size() - 1] != '/')
		{
			infa.buffer_to_send = "HTTP1.1/ 301 Moved Permanently\r\nLocation: ";
			infa.buffer_to_send += url + "/";
			infa.buffer_to_send += "\r\n\r\n";
			infa.status = 1;
			closedir(dir);
			throw std::runtime_error("\e[91mFolder is requested , but need to be redirected\e[0m");
		}
		send_indexing(dir);
	}
}

void	method_get::folder_handling()
{
	struct stat	st;
	size_t	i = 0;	
	while (i < this->index.size())
	{
		std::string temp = path;
		temp += index[i];
		if (stat(temp.c_str() , &st) == 0)
		{
			this->size = st.st_size;
			infa.size = st.st_size;
			this->path = temp;
			file_handling();
			throw std::runtime_error("\e[91mFolder is requested , resopnse with index\e[0m");
		}
		i++;
	}
	if (this->auto_index == true)
		handle_auto_index();
	else
		set_error("403");
}
