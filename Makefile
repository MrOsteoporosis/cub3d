# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: averheij <averheij@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/01/15 15:20:06 by averheij       #+#    #+#                 #
#    Updated: 2020/01/15 16:04:14 by averheij      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME	=	cub3d
CFILES	=	init.c
OFILES	=	$(CFILES:.c=.o)
FLAGS	=	#-Werror -Wall -Wextra
CC		=	gcc $(FLAGS)

all: $(NAME)

$(NAME): $(OFILES)
	$(CC) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME) $(OFILES)

%.o: %.c
	@printf "Compiling $<\n"
	@$(CC) -Imlx -Iinc -Ilibft -c $< -o $@

clean:
	@echo "Cleaning objects"
	@rm -f $(OFILES)

fclean: clean
	@echo "Cleaning binary"
	@rm -f $(NAME)

re: fclean all