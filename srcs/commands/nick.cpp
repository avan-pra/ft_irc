#include "../../includes/commands.hpp"

void   nick_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
    std::cout << line << std::endl;
    std::cout << client_idx << std::endl;
    std::cout << serv.get_hostname() << std::endl;
}
