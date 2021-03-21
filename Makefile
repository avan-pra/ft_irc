# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/26 16:25:58 by jvaquer           #+#    #+#              #
#    Updated: 2021/03/21 23:03:50 by jvaquer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Serv

C++ = clang++

FLAG = -Wall -Werror -Wextra

SRC = srcs/main.cpp srcs/parser.cpp srcs/get_client.cpp srcs/parse_conf.cpp \
	  srcs/run_server.cpp srcs/setup_serv.cpp srcs/init_command.cpp \
	  srcs/create_error_msg.cpp srcs/commands/nick.cpp srcs/libft/ft_atoi.cpp \
	  srcs/libft/ft_bzero.cpp srcs/libft/ft_split.cpp

OBJ = srcs/main.o srcs/parser.o srcs/get_client.o srcs/parse_conf.o \
	  srcs/run_server.o srcs/setup_serv.o srcs/init_command.o \
	  srcs/create_error_msg.o srcs/commands/nick.o srcs/libft/ft_atoi.o \
	  srcs/libft/ft_bzero.o srcs/libft/ft_split.o

all : $(NAME)

$(NAME) : $(OBJ)
		$(C++) $(FLAGS) $(OBJ) -o $(NAME)

clean :
		rm -rf $(OBJ)

fclean : clean
		rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
