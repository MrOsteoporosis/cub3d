# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: averheij <averheij@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/01/15 15:20:06 by averheij       #+#    #+#                 #
#    Updated: 2020/01/28 11:10:23 by averheij      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME	=	cub3d
CFILES	=	init.c event_basic.c util_pixel.c
OFILES	=	$(CFILES:.c=.o)
FLAGS	=	#-Werror -Wall -Wextra
FLAGS	= -g 
CC		=	gcc $(FLAGS)

BOLD	=	\033[1m
CLEAR	=	\033[0m

all: $(NAME)

$(NAME): $(OFILES)
	@echo "$(BOLD)/--------     mlx     --------\\ $(CLEAR)"
	make -C mlx
	@echo ""
	@echo "$(BOLD)/--------    libft    --------\\ $(CLEAR)"
	make -C libft
	@echo ""
	$(CC)-Lmlx -lmlx -Llibft -lft -framework OpenGL -framework AppKit -o $(NAME) $(OFILES)

%.o: %.c
	@printf "Compiling $<	| "
	$(CC) -Imlx -Ilibft -c $< -o $@

clean:
	@echo "Cleaning objects:"
	rm -f $(OFILES)
	@echo "$(BOLD)/--------     mlx     --------\\ $(CLEAR)"
	make clean -C mlx
	@echo ""
	@echo "$(BOLD)/--------    libft    --------\\ $(CLEAR)"
	make clean -C libft
	@echo ""

fclean: clean
	@echo "Cleaning binary:"
	rm -f $(NAME)
	@echo "$(BOLD)/--------    libft    --------\\ $(CLEAR)"
	make fclean -C libft
	@echo ""

re: fclean all
