# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:      :+:    :+:    #
#                                                      +:+                     #
#    By: averheij <averheij@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/01/15 15:20:06 by averheij       #+#    #+#                 #
#    Updated: 2020/02/06 12:41:14 by averheij         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	cub3d
CFILES	=	init.c event_basic.c util_pixel.c raycast.c util_ray.c movement.c
OFILES	=	$(CFILES:%.c=objects/%.o)
FLAGS	=	#-Werror -Wall -Wextra
FLAGS	+= 	-g
CC		=	gcc $(FLAGS)

# LINUX
#MLX_DIR	=	mlxlinux
#MLX_NAME=	minilibx
#MLX_INCLUDE	=	mlxlinux/includes
#EXTRA_FLAGS	=	-lm -lX11 -lXext
#MAC
MLX_DIR	=	mlx
MLX_NAME=	mlx
MLX_INCLUDE	=	mlx
EXTRA_FLAGS	=	-framework OpenGL -framework AppKit libmlx.dylib

BOLD	=	\033[1m
CLEAR	=	\033[0m

all: $(NAME)

$(NAME): $(OFILES)
	@echo "$(BOLD)/--------     mlx     --------\\ $(CLEAR)"
	make -C $(MLX_DIR)
	cp $(MLX_DIR)/libmlx.dylib .
	@echo ""
	@echo "$(BOLD)/--------    libft    --------\\ $(CLEAR)"
	make -C libft
	@echo ""
	$(CC) -o $(NAME) $(OFILES) -L$(MLX_DIR) -l$(MLX_NAME) -Llibft -lft $(EXTRA_FLAGS)

objects/%.o: %.c
	@printf "Compiling $<	| "
	$(CC) -I$(MLX_INCLUDE) -Ilibft -c $< -o $@

clean:
	@echo "Cleaning objects:"
	rm -f $(OFILES)
	@echo "$(BOLD)/--------     mlx     --------\\ $(CLEAR)"
	make clean -C $(MLX_DIR)
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
