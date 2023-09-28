#ifndef get_hpp
#define get_hpp


#include <sys/wait.h>

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <sys/stat.h> 
#include <sys/types.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <map>
#include <netinet/tcp.h> 
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <sys/stat.h> 
#include <sys/types.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <map>
#include <netinet/tcp.h>
#include   "/nfs/homes/abouzanb/Desktop/cloned_webserver/includes/Servers.hpp"
#include "/nfs/homes/abouzanb/Desktop/cloned_webserver/Multiplexing/multiplexing.hpp"
#include "../includes/conf_parser.hpp"

class info : public t_client_info {
	public :
	info(int sock)
	{
		socket = sock;
		size = 0;
		file = NULL;
		status = 1;
		was_read = 0;
	}
	int was_read;
	int socket;
	int size;
	int status;
	std::string path;
	std::string containte;
	std::ifstream *file;
	std::string buffer_to_send;

};

class method_get
{
	protected:
	std::map<std::string, std::string> map;
	std::map<std::string, std::string> extansion_handling;
	std::string path;
	std::string route;
	std::string extansion;
	bool auto_index;
	std::vector<std::string> index;
	public :
	int size;
	std::ifstream *file;
	std::string url;
	info &infa;
	Directives keep;
	std::map<int , std::string> erros_page;
	std::string location;
	method_get(Directives k, std::string l, info &inf);
	void get_check_path();
	void folder_handling();
	void file_handling();
	void handle_auto_index();
	void set_error_404();
	void set_error_500();
	void set_error_403();
	void check_location();
	void send_indexing(DIR *dir);
};

class ft_delete : public method_get
{

    public :
    ft_delete(Directives k, std::string l, info &inf);
    void location_check();
    void remove_them(std::string path);
    void check_stat();
    void remove_file(std::string path);
    void remove_folder(std::string path);
};


#endif