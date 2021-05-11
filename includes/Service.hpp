#ifndef SERVICE_HPP
# define SERVICE_HPP

# include "./Connection.hpp"

class Service : public Connection
{
private:
	/* data */
public:
	Service(/* args */);
	virtual ~Service();
};

#endif