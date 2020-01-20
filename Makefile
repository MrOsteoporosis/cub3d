# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: averheij <averheij@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/01/15 15:20:06 by averheij       #+#    #+#                 #
#    Updated: 2020/01/20 13:14:04 by averheij      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME	=	cub3d
CFILES	=	init.c event_basic.c util_pixel.c
OFILES	=	$(CFILES:.c=.o)
FLAGS	=	#-Werror -Wall -Wextra
CC		=	gcc $(FLAGS)

all: $(NAME)

$(NAME): $(OFILES)
	@echo "/--------mlx--------\\"
	make -C mlx
	@echo "\\-------------------/"
	$(CC)-Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME) $(OFILES)

%.o: %.c
	@printf "Compiling $<	| "
	$(CC) -Imlx -Iinc -Ilibft -c $< -o $@

clean:
	@echo "Cleaning objects:"
	rm -f $(OFILES)
	@echo "/--------mlx--------\\"
	make clean -C mlx
	@echo "\\-------------------/"

fclean: clean
	@echo "Cleaning binary:"
	rm -f $(NAME)

re: fclean all