/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 11:57:39 by averheij       #+#    #+#                */
/*   Updated: 2020/03/02 13:21:21 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# define MAX_RESX   1920
# define MAX_RESY   1080
# define FOV	1.0472
# define HALF_FOV	0.5236
# define M_PI2  6.28318530718
# define DEG90  1.5708
# define DEG180 3.14159
# define DEG270 4.71239
# define FRICTION   0.04
# define MOVESPEED  1
# define LOOKSPEED  0.035
# define GRID   512
# define GRIDPOW	9

# define WKEY   13
# define AKEY   0
# define SKEY   1
# define DKEY   2
# define LAKEY  123
# define RAKEY  124

typedef struct	s_data {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
    int         resx;
    int         resy;
	int			halfresy;
}				t_data;

typedef struct	s_world {
	char		**map;
    int         colorfloor;
    int         colorceiling;
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
	short	   forward;
	short	   backward;
	short	   strafeleft;
	short	   straferight;
	short	   lookleft;
	short	   lookright;
	float	   speedy;
	float	   speedx;
}			   t_movement;

typedef struct	s_vars {
	void		*mlx;
	void		*win;
	t_data		img;
	t_world		world;
	t_movement  move;
	t_data	   	no;
	t_data	   	so;
	t_data	   	we;
	t_data	   	ea;
	t_data	   	s;
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
	float	   	dist;
	int		 	height;
	int		 	real_height;
	t_data	   	*tex;
	int		 	tex_offset;
}				t_ray;

typedef struct	s_caster {
	float		raydir;
	int			column;
	float		a;
	t_ray		v;
	t_ray		h;
	t_ray	   *near;
}				t_caster;

typedef int	    (*t_isfunc)(int c);
typedef int     (*t_efunc)(t_vars *vars, char *line);

int     parse_cub(t_vars *vars, char *map_path);
int     call_element_parser(t_vars *vars, char *line, int *elecount);
int     parse_res(t_vars *vars, char *line);
int     parse_tex(t_vars *vars, char *line);
int     parse_color(t_vars *vars, char *line);
int		parse_rgb_partial(int *color, char **str, int notlast);
int		parse_map(t_vars *vars, int fd);
int		map_line_sanitize(char **lineorigin, char *line, int *ft_width);
int		array_append(char ***map, char *line, int currentlength);
int		validate_map(char **map, t_vars *vars);

int		ft_iswhitespace(int c);
int     ft_skip_comma(char **str);
int     ft_skip_passed_func(char **str, t_isfunc is);
int		count_non_whitespace_char(char *line);
int		is_valid_cub_char(char c);
int		free_line_and_close(int fd, char *line);
int		free_everything(t_vars *vars, int fd, char *line);

int	 	render(t_vars *vars);
void	clear_frame_color_sky_floor(t_data *data, int sky, int ftfloor);
void	cast_ray(t_vars *vars);
void	cast_horizontal(t_world *world, t_ray *ray, float a, float tan_a);
void	cast_vertical(t_world *world, t_ray *ray, float a, float tan_a);
void	extendray(t_world *world, t_ray *ray);
void	calc_offsets(t_vars *vars, t_caster *caster);
void	calc_distance(t_world *world, t_caster *caster);
void	draw_texture_column(t_data *frame, t_ray *ray, int frame_column, t_data *tex);

int		create_trgb(int t, int r, int g, int b);
float	ray_angle(float lookdir, float raydir);
int		check_bounds(t_world *world, t_ray *ray);
int	 	ft_abs(int x);

void	do_movement(t_world *world, t_movement *move);
void	adjust_speed(float lookdir, float movedir, t_movement *move);
void	adjust_look(float *lookdir, t_movement *move);

int	 	close_window(t_vars *vars);
int	 	key_press(int keycode, t_vars *vars);
int	 	key_release(int keycode, t_vars *vars);

int		check_argc_argv(int argc, char **argv);
#endif
