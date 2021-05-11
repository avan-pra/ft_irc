#ifndef UNREGISTERED_HPP
# define UNREGISTERED_HPP

# include "./Connection.hpp"

class Unregistered : public Connection
{
private:
    /* data */
public:
    Unregistered(/* args */);
    ~Unregistered();
};

Unregistered::Unregistered(/* args */)
{
}

Unregistered::~Unregistered()
{
}


#endif