#include "get.hpp"

ft_delete::ft_delete(Directives& k, std::string l, info &inf) : method_get(k, l,inf)
{
    this->keep = k;
    this->url = l;
    this->infa = inf;
    try 
 {
    this->location_check();
    this->check_stat();
    infa.buffer_to_send = extansion_handling["html"];
    infa.buffer_to_send = "HTTP/1.1 204 No Content\r\n\r\n";
    infa.status = 1;

}
catch (std::exception &e)
{
    infa.status = 1;
    return ;
}
}

void ft_delete::remove_them(std::string path)
{
    struct stat st;
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(path.c_str())))
        return ;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;
        std::string full_path = path + "/" + name;
        if (stat(full_path.c_str(), &st) == -1)
            continue;
        if (S_ISDIR(st.st_mode))
        {
            remove_them(full_path);
            remove_folder(full_path);
        }
        else
            remove_file(full_path);
    }
    closedir(dir);
    remove_folder(path);


}

void ft_delete::remove_folder(std::string path)
{
    if (fork() == 0)
    {
      const char* argv[] = {"rm", "-rf", path.c_str(), NULL};

        execve("/bin/rm", const_cast<char *const *>(argv), NULL);
        return ;
    }
    wait(NULL);
}

void ft_delete::remove_file(std::string path)
{
    if (fork() == 0)
    {   
        const  char *argv[] = {"rm", "-f", path.c_str(), NULL};
        execve("/bin/rm",  const_cast<char *const *>(argv), NULL);
        return ;
    }
    wait(NULL);
}

void ft_delete::check_stat()
{
	struct stat st;
	if (stat(path.c_str() , &st) == 0)
	{
        if (S_ISDIR(st.st_mode))
	        remove_them(path);
	    else
		    remove_file(path);
	}
	else
    {
        set_error_404();
        std::cout << "path:444444444444444444 " << path << std::endl;
        throw std::exception();
    }
}

void ft_delete::location_check()
{
	int forbidden = 0;
	size_t i = 0;
	int max_lenght = 0;
	int size = 0;
	int is_checked = 0;
	while (i < keep.getLocationsVec().size())
	{
		size = keep.getLocationsVec()[i].getLocation().size();
		if (keep.getLocationsVec()[i].getLocation() == url.substr(0, size) && size >= max_lenght)
		{
			if (keep.getLocationsVec()[i].getAcceptedMethods()["DELETE"] == true)
			{
				max_lenght = size;
				this->route = keep.getLocationsVec()[i].getRoot();
				path = route + url.substr(size, url.size());
                std::cout << "path: " << path << std::endl;
                is_checked = 1;
				forbidden = 0;
			}
			else
				forbidden = 1;
		}
		i++;
	}
	if (forbidden == 1)
	{
		set_error_403();
		throw std::runtime_error("403");
	}
	if (is_checked == 0)
	{
		set_error_404();
		throw std::runtime_error("404");
	}
}
