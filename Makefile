# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: averheij <averheij@student.codam.nl>         +#+                      #
#                                                    +#+                       #
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
#    Updated: 2020/05/29 13:44:40 by averheij      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME	=	cub3d
CFILES	=	init.c bmp.c events.c movement.c ray_cast.c ray_sky_dist_draw.c\
			ray_z_util.c cub_parse_main.c cub_parse_meta.c cub_parse_map.c\
			cub_parse_z_util_1.c cub_parse_z_util_2.c sprite.c sprite_z_util.c\
			get_next_line/get_next_line.c get_next_line/get_next_line_utils.c
OFILES	=	$(CFILES:%.c=objects/%.o)
FLAGS	=	#-Werror -Wall -Wextra
FLAGS	+= 	-g
CC		=	gcc $(FLAGS)

# LINUX	
MLX_DIR	=	mlxlinux
MLX_NAME=	mlx_Linux
MLX_INCLUDE	=	mlxlinux
EXTRA_FLAGS	=	-lz -lm -lX11 -lXext
#MAC
#MLX_DIR	=	mlx
#MLX_NAME=	mlx
#MLX_INCLUDE	=	mlx
#EXTRA_FLAGS	=	-framework OpenGL -framework AppKit libmlx.dylib -fsanitize=address

BOLD	=	\033[1m
CLEAR	=	\033[0m

all: $(NAME)

$(NAME): $(OFILES)
	@echo "$(BOLD)/--------     mlx     --------\\ $(CLEAR)"
	make -C $(MLX_DIR)
	#cp $(MLX_DIR)/libmlx.dylib .
	@echo ""
	@echo "$(BOLD)/--------    libft    --------\\ $(CLEAR)"
	make -C libft
	@echo ""
	$(CC) -o $(NAME) $(OFILES) -L$(MLX_DIR) -l$(MLX_NAME) -Llibft -lft $(EXTRA_FLAGS)

objects/%.o: %.c
	@printf "Compiling $<	| "
	$(CC) -I$(MLX_INCLUDE) -Ilibft -Iget_next_line -c $< -o $@

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
