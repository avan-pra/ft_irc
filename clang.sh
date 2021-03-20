#!/bin/bash

if [ $1 = "flags" ]
then
	clang++ srcs/*.cpp srcs/libft/*.cpp srcs/commands/*.cpp -o serv
else
	clang++ -Wall -Werror -Wextra --std=c++98 srcs/*.cpp srcs/libft/*.cpp srcs/commands/*.cpp -o serv
fi
