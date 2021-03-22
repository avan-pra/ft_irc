# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/26 16:25:58 by jvaquer           #+#    #+#              #
#    Updated: 2021/03/22 14:03:31 by jvaquer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Serv

C++ = clang++

FLAG = -Wall -Werror -Wextra -std=c++98

DIR_SRCS = ./srcs/

SRC = main.cpp parser.cpp get_client.cpp parse_conf.cpp \
	  run_server.cpp setup_serv.cpp init_command.cpp \
	  create_error_msg.cpp commands/nick.cpp commands/user.cpp libft/ft_atoi.cpp \
	  libft/ft_bzero.cpp libft/ft_split.cpp

OBJ = $(SRCS:.cpp=.o)

SRCS =			$(addprefix $(DIR_SRCS), $(SRC))

all : $(NAME)

$(NAME) : $(OBJ)
		$(C++) $(FLAGS) $(OBJ) -o $(NAME)

clean :
		rm -rf $(OBJ)

fclean : clean
		rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
