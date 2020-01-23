# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: averheij <averheij@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/01/15 15:20:06 by averheij       #+#    #+#                 #
#    Updated: 2020/01/23 10:05:54 by averheij      ########   odam.nl          #
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
	@echo "/--------libft--------\\"
	make -C libft
	@echo "\\-------------------/"
	$(CC)-Lmlx -lmlx -Llibft -lft -framework OpenGL -framework AppKit -o $(NAME) $(OFILES)

%.o: %.c
	@printf "Compiling $<	| "
	$(CC) -Imlx -Ilibft -c $< -o $@

clean:
	@echo "Cleaning objects:"
	rm -f $(OFILES)
	@echo "/--------mlx--------\\"
	make clean -C mlx
	@echo "\\-------------------/"
	@echo "/--------libft--------\\"
	make clean -C libft
	@echo "\\-------------------/"

fclean: clean
	@echo "Cleaning binary:"
	rm -f $(NAME)
	@echo "/--------libft--------\\"
	make fclean -C libft
	@echo "\\-------------------/"

re: fclean all