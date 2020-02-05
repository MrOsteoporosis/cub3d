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

//mac keycodes
//# define WKEY   13
//# define AKEY   0
//# define SKEY   1
//# define DKEY   2
//# define LAKEY  123
//# define RAKEY  124

//linux keycodes
# define WKEY   119
# define AKEY   97
# define SKEY   115
# define DKEY   100
# define LAKEY  65361
# define RAKEY  65363

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
	float		playerx;
	float		playery;
	float		lookdir;
	float		radians_per_pixel;
	float		proj_plane_dist;
}				t_world;

typedef struct  s_movement {
    short       forward;
    short       backward;
    short       strafeleft;
    short       straferight;
    short       lookleft;
    short       lookright;
    float       speedy;
    float       speedx;
}               t_movement;

typedef struct	s_vars {
	void		*mlx;
	void		*win;
	t_data		img;
	t_world		world;
    t_movement  move;
	int			stop;
}				t_vars;

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

void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	my_mlx_sliver_put(t_data *data, int x, int y, int height, int color);
void	my_mlx_clear_frame(t_data *data, int width, int height);
int		create_trgb(int t, int r, int g, int b);

int		close(t_vars *vars);
int     key_press(int keycode, t_vars *vars);
int		key_release(int keycode, t_vars *vars);
int		mouse_move(int x, int y);

int		render(t_vars *vars);
int     do_movement(t_vars *vars);
int     cast_ray(t_vars *vars);
int     cast_vertical(t_vars *vars, t_caster *caster, float tan_a);
int     cast_horizontal(t_vars *vars, t_caster *caster, float tan_a);
int		extendray(t_vars *vars, t_ray *ray);
int		distanceanddraw(t_vars *vars, t_caster *caster);

float	ray_angle(float lookdir, float raydir);
int		check_bounds(t_world *world, t_ray *ray);
char	**statictodynamic(void);
#endif
