/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 11:57:39 by averheij       #+#    #+#                */
/*   Updated: 2020/02/10 11:59:41 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# define FOV	1.0472
# define HALF_FOV	0.5236
# define DEG90  1.5708
# define DEG180 3.14159
# define DEG270 4.71239
# define FRAME_WIDTH	720
# define FRAME_HEIGHT	480
# define HALF_FRAME_HEIGHT	240
# define FRICTION   0.04
# define MOVESPEED  1
# define LOOKSPEED  0.02
# define GRID   512
# define GRIDPOW    9

//mac keycodes
# define WKEY   13
# define AKEY   0
# define SKEY   1
# define DKEY   2
# define LAKEY  123
# define RAKEY  124

//linux keycodes
//# define WKEY   119
//# define AKEY   97
//# define SKEY   115
//# define DKEY   100
//# define LAKEY  65361
//# define RAKEY  65363

typedef struct	s_data {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_data;

typedef struct  s_tex {
    t_data      img;
    char        *path;
    int         width;
    int         height;
}               t_tex;

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
    t_tex       no;
    t_tex       so;
    t_tex       we;
    t_tex       ea;
    t_tex       s;
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
    float       dist;
    int         real_height;
    int         height;
    int         tex_offset;
}				t_ray;

typedef struct	s_caster {
	float		raydir;
	int			column;
	float		a;
	t_ray		v;
	t_ray		h;
    t_ray       *near;
}				t_caster;

int		close(t_vars *vars);
int     key_press(int keycode, t_vars *vars);
int		key_release(int keycode, t_vars *vars);
int		mouse_move(int x, int y);

int		render(t_vars *vars);
int     cast_ray(t_vars *vars);
int     cast_vertical(t_vars *vars, t_caster *caster, float tan_a);
int     cast_horizontal(t_vars *vars, t_caster *caster, float tan_a);
int		extendray(t_vars *vars, t_ray *ray);
int		distanceanddraw(t_vars *vars, t_caster *caster);
int     draw_texture_column(t_vars *vars, t_caster *caster);

int     do_movement(t_vars *vars);
int     adjust_speed(t_vars *vars, int dir);
int     adjust_look(t_vars *vars);

void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	my_mlx_sliver_put(t_data *data, int x, int y, int height, int color);
void	my_mlx_clear_frame(t_data *data, int width, int height);
int		create_trgb(int t, int r, int g, int b);

float	ray_angle(float lookdir, float raydir);
int		check_bounds(t_world *world, t_ray *ray);
#endif
