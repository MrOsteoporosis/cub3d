/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 11:57:39 by averheij       #+#    #+#                */
/*   Updated: 2020/02/03 12:42:24 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# define FOV	1.0472
# define HALF_FOV	0.5236
# define FRAME_WIDTH	720
# define FRAME_HEIGHT	480
# define HALF_FRAME_HEIGHT	240

typedef struct	s_data {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_data;

typedef struct	s_world {
	char		**map;
	int			map_width;
	int			map_height;
	int			max_x;
	int			max_y;
	int			playerx;
	int			playery;
	float		lookdir;
	float		radians_per_pixel;
	float		proj_plane_dist;
}				t_world;

typedef struct	s_ray {
	float		x;
	float		y;
	float		xincr;
	float		yincr;
	int			gridx;
	int			gridy;
	short		safe;
	short		foundwall;
}				t_ray;

typedef struct	s_caster {
	float		raydir;
	int			column;
	float		a;
	t_ray		v;
	t_ray		h;
}				t_caster;

typedef struct	s_vars {
	void		*mlx;
	void		*win;
	t_data		img;
	t_world		world;
	int			stop;
}				t_vars;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	my_mlx_sliver_put(t_data *data, int x, int y, int height, int color);
void	my_mlx_clear_frame(t_data *data, int width, int height);
int		create_trgb(int t, int r, int g, int b);

int		close(t_vars *vars);
int		key_release(int keycode, t_vars *vars);
int		mouse_move(int var, int var2, int var3);

int		render(t_vars *vars);
int		extendray(t_vars *vars, t_ray *ray);
int		distanceanddraw(t_vars *vars, t_caster *caster);

float	ray_angle(float lookdir, float raydir);
int		check_bounds(t_world *world, t_ray *ray);
char	**statictodynamic(void);
#endif