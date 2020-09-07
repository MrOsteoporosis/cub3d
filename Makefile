# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: averheij <averheij@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/05/29 15:42:33 by averheij      #+#    #+#                  #
#    Updated: 2020/09/07 12:18:45 by averheij      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME	=	cub3d
CFILES	=	init.c bmp.c events.c movement.c ray_cast.c ray_sky_dist_draw.c\
			ray_z_util.c cub_parse_main.c cub_parse_meta.c cub_parse_map.c\
			cub_parse_z_util_1.c cub_parse_z_util_2.c sprite.c sprite_z_util.c
OFILES	=	$(CFILES:%.c=objects/%.o)
FLAGS	=	-Werror -Wall -Wextra
CC		=	gcc $(FLAGS)
OBJDIR	=	./objects
OS		=	$(shell uname)

ifeq ($(OS),Linux)
	MLX_DIR		=	mlxlinux
	MLX_NAME	=	mlx_Linux
	MLX_INCLUDE	=	mlxlinux
	DEFINE		=	-D LINUX=1
	EXTRA_FLAGS	=	-lz -lm -lX11 -lXext $(DEFINE)
else
	MLX_DIR		=	mlx
	MLX_NAME	=	mlx
	MLX_INCLUDE	=	mlx
	DEFINE		=	-D MACOS=1
	EXTRA_FLAGS	=	-framework OpenGL -framework AppKit libmlx.dylib $(DEFINE)
endif

BOLD	=	\033[1m
CLEAR	=	\033[0m

all: $(NAME)

$(NAME): $(OBJDIR) $(OFILES)
	@echo "$(BOLD)/--------     mlx     --------\\ $(CLEAR)"
	make -C $(MLX_DIR)
ifneq ($(OS),Linux)
	cp $(MLX_DIR)/libmlx.dylib .
endif
	@echo ""
	@echo "$(BOLD)/--------    libft    --------\\ $(CLEAR)"
	make -C libft
	@echo ""
	$(CC) -o $(NAME) $(OFILES) -L$(MLX_DIR) -l$(MLX_NAME) -Llibft -lft $(EXTRA_FLAGS)

$(OBJDIR):
	mkdir -p $@

objects/%.o: %.c
	$(CC) -I$(MLX_INCLUDE) -Ilibft -c $< -o $@ $(DEFINE)

bonus: all

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
