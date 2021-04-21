# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/26 16:25:58 by jvaquer           #+#    #+#              #
#    Updated: 2021/04/21 12:55:32 by jvaquer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

C++ = clang++

CXX = clang++

CERT = godirc.crt

KEY = godirc.key

CUR_DIR = $(shell pwd)

FLAG = -Wall -Werror -Wextra -std=c++98

#CXXFLAGS = $(FLAG)

FLAGS = -lssl -lcrypto -ldl -L$(CUR_DIR)/openssl-1.1.1j

#srcs file
SRCS =			$(addprefix $(DIR_SRCS), $(SRC)) 
DIR_SRCS = ./srcs/
SRC = main.cpp parser.cpp get_client.cpp parse_conf.cpp \
	  run_server.cpp setup_serv.cpp init_command.cpp \
	  create_msg.cpp iterate_client.cpp iterate_server.cpp \
	  find_client_or_channel.cpp send_msg_to.cpp mask_parser.cpp \
	  tls.cpp \

OBJ = $(SRCS:.cpp=.o)

#libft file
SRCS_LIB = $(addprefix $(DIR_LIB), $(SRC_LIB))
DIR_LIB = ./srcs/libft/
SRC_LIB = ft_atoi.cpp ft_bzero.cpp ft_split.cpp char2hex.cpp

OBJ_LIB = $(SRCS_LIB:.cpp=.o)

#command file
SRCS_CMD = $(addprefix $(DIR_CMD), $(SRC_CMD))
DIR_CMD = ./srcs/commands/
SRC_CMD = nick.cpp user.cpp time.cpp \
		  motd.cpp info.cpp version.cpp \
		  pass.cpp join.cpp mode.cpp cap.cpp \
		  ping.cpp pong.cpp quit.cpp names.cpp \
		  list.cpp invite.cpp part.cpp server.cpp \
		  privmsg.cpp who.cpp oper.cpp kick.cpp \
		  topic.cpp notice.cpp specific_modes.cpp \

OBJ_CMD = $(SRCS_CMD:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(OBJ_LIB) $(OBJ_CMD)
		$(C++) $(FLAGS) $(OBJ) $(OBJ_LIB) $(OBJ_CMD) -o $(NAME)

certs :
		openssl req -x509 -nodes -days 365 -newkey rsa:4096 \
		-keyout ./$(KEY) -out ./$(CERT) \
		-subj "/C=FR/ST=Paris/L=Paris/O=42/OU=21/CN=ircserv"

del_certs :
		rm -rf $(CERT) $(KEY)

clean :
		rm -rf $(OBJ) $(OBJ_CMD) $(OBJ_LIB)

fclean : clean
		rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
