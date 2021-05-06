# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/26 16:25:58 by jvaquer           #+#    #+#              #
#    Updated: 2021/05/06 01:05:40 by jvaquer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

C++ = clang++

CXX = clang++

CERT = godirc.crt

KEY = godirc.key

CUR_DIR = $(shell pwd)

FLAG = -Wall -Werror -Wextra -std=c++98

CXXFLAGS = $(FLAG)

FLAGS = -lssl -lcrypto -ldl -L$(CUR_DIR)/openssl-1.1.1j

#srcs file
SRCS =			$(addprefix $(DIR_SRCS), $(SRC)) 
DIR_SRCS = ./srcs/
SRC = main.cpp parser.cpp get_client.cpp \
	  run_server.cpp init_command.cpp \
	  create_msg.cpp iterate_client.cpp iterate_server.cpp iterate_connection.cpp\
	  find_client_or_channel.cpp send_msg_to.cpp mask_parser.cpp \
	  tls.cpp error.cpp acquire_message.cpp class/client.cpp find_connections.cpp\
	  Disconnect.cpp \

OBJ = $(SRCS:.cpp=.o)

#libft file
SRCS_LIB = $(addprefix $(DIR_LIB), $(SRC_LIB))
DIR_LIB = ./srcs/libft/
SRC_LIB = ft_atoi.cpp ft_bzero.cpp ft_split.cpp char2hex.cpp ft_to_string.cpp

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
		  whois.cpp whowas.cpp ison.cpp userhost.cpp \
		  away.cpp die.cpp kill.cpp \

OBJ_CMD = $(SRCS_CMD:.cpp=.o)

#config folder
SRCS_CONF = $(addprefix $(DIR_CONF), $(SRC_CONF))
DIR_CONF = ./srcs/config/
SRC_CONF = parse_conf.cpp setup_serv.cpp

OBJ_CONF = $(SRCS_CONF:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(OBJ_LIB) $(OBJ_CMD) $(OBJ_CONF)
		$(C++) $(FLAGS) $(OBJ) $(OBJ_LIB) $(OBJ_CMD) $(OBJ_CONF) -o $(NAME)

certs :
		openssl req -x509 -nodes -days 365 -newkey rsa:4096 \
		-keyout ./$(KEY) -out ./$(CERT) \
		-subj "/C=FR/ST=Paris/L=Paris/O=42/OU=21/CN=ircserv"

install :
		tar -xf openssl-1.1.1j.tar
		cd openssl-1.1.1j && ./Configure linux-x86_64
		cd openssl-1.1.1j && make
		cd openssl-1.1.1j && sudo make install
		sudo ldconfig

del_certs :
		rm -rf $(CERT) $(KEY)

clean :
		rm -rf $(OBJ) $(OBJ_CMD) $(OBJ_LIB) $(OBJ_CONF)

fclean : clean
		rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
