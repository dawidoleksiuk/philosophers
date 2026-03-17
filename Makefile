# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/29 19:52:22 by doleksiu          #+#    #+#              #
#    Updated: 2026/03/17 20:22:13 by doleksiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -pthread -g -I$(INCS_DIR)

NAME = philo

SRC_DIR = ./srcs
OBJS_DIR = ./objs
INCS_DIR = ./includes
SRCS = philo.c init.c clean_exit.c utils.c simulation.c
OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))
INCS = $(INCS_DIR)/philo.h

all: $(NAME)
 
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
# -fsanitize=address

# -fsanitize=thread
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean: 
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)
	
re: fclean all

run: all
	setarch -R ./philo

.PHONY: all clean fclean re