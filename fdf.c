/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lworden <lworden@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 20:27:28 by lworden           #+#    #+#             */
/*   Updated: 2024/05/06 23:07:31 by lworden          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <X11/Xlib.h>

// -lm required to link math library
// cc fdf.c fdf_utils0.c fdf_utils1.c fdf_utils2.c fdf_utils3.c 
// 		-L ./lib/ -lft -lmlx -lXext -lX11 -lm
// man -M ../man/ mlx_loop
// valgrind --trace-children=yes ./a.out ../maps/test_maps/t1.fdf

int	main(int argc, char **argv)
{
	t_maps	maps;
	t_vars	p_vars;

	p_vars.mlx_ptr = mlx_init();
	if (argc != 2)
		return (1);
	if (!(ft_strnstr(argv[1], ".fdf", ft_strlen(argv[1]))))
		return (1);
	if (init_map(&maps, &p_vars, argv))
		return (1);
	calc_vect(&maps, &p_vars);
	if (!p_vars.mlx_ptr)
		return (1);
	p_vars.win_ptr = mlx_new_window(p_vars.mlx_ptr, 1200, 800, "cpt");
	//draw(&p_vars);
	draw_image(&maps, &p_vars);
	mlx_hook(p_vars.win_ptr, 3, 1L << 1, &key_up, &p_vars);
	mlx_hook(p_vars.win_ptr, 2, 1L << 0, &key_down, &p_vars);
	mlx_hook(p_vars.win_ptr, 17, StructureNotifyMask, &terminator, &p_vars);
	mlx_loop_hook(p_vars.mlx_ptr, &loop_actions, &p_vars);
	mlx_loop(p_vars.mlx_ptr);
	return (0);
}

void	init_vars(t_vars *p_vars, t_maps *maps)
{
	p_vars->m = maps;
	p_vars->scale = 8;
	p_vars->originx = 1200.0;
	p_vars->originy = 100.0;
	p_vars->toggle_plane = -1;
	p_vars->toggle_grid = 1;
	p_vars->toggle_key = 0;
	p_vars->z_s = 8;
	p_vars->toggle_z = -1;
	p_vars->img = mlx_new_image(p_vars->mlx_ptr, 1200, 1200);
	maps->xaxis = 0;
	maps->yaxis = 0;
}

void	calc_vect(t_maps *maps, t_vars *p_vars)
{
	t_vec	org;
	int		i;
	int		j;

	org.x = p_vars->originx;
	org.y = p_vars->originy;
	i = 0;
	while (i < maps->yaxis)
	{
		j = 0;
		while (j < maps->xaxis)
		{
			maps->map_vec[i][j].x = org.x + (j * p_vars->scale);
			maps->map_vec[i][j].y = org.y + (j * (p_vars->scale / 4))   // implement way to adjust at run time
				+ (maps->map_vec[i][j].z * -1 * (p_vars->scale / p_vars->z_s));
			maps->map_vec[i][j].y_o = org.y + (j * (p_vars->scale / 2));
			j++;
		}
		org.x -= (p_vars->scale);
		org.y += (p_vars->scale / 2);
		i++;
	}
}

int	init_map(t_maps *maps, t_vars *p_vars, char *argv[])
{
	int	err;

	err = 0;
	init_vars(p_vars, maps);
	err += calc_axes(maps, argv[1]);
	err += init_arrs(maps);
	err += init_zaxis(maps, argv[1]);
	if (err < 0)
		return (1);
	return (0);
}

int	terminator(t_vars *p_vars)
{
	ft_printf("Hasta la vista, baby!\n");
	free_2d_arr(p_vars->m);
	mlx_destroy_window(p_vars->mlx_ptr, p_vars->win_ptr);
	mlx_destroy_display(p_vars->mlx_ptr);
	free(p_vars->mlx_ptr);
	exit(0);
}

// typedef struct vv
// {
// 	void	*mlx;
// 	void	*win;
// 	void	*img;
// 	char	*addr;
// } vv;


int	draw_image(t_maps *maps, t_vars *p_vars)
{

	calc_vect(p_vars->m, p_vars);
	//mlx_clear_window(p_vars->mlx_ptr, p_vars->win_ptr);
	
	vv	v;
	int	bits_per_pixel;
	int	size_line;
	int	endian;
	int color_int;
	int	black;

	bits_per_pixel = 8;
	size_line = 1200;
	endian = 0;


	//p_vars->img = mlx_new_image(p_vars->mlx_ptr, 1200, 1200);

	v.addr = mlx_get_data_addr(p_vars->img, &bits_per_pixel, &size_line, &endian);

	color_int = mlx_get_color_value(p_vars->mlx_ptr, 0x00FFFFFF);
	black = mlx_get_color_value(p_vars->mlx_ptr, 0x00000000);

	// for (int i = 0; i < (1200 * 800) * 4; i+=)
	// {
	// 	//if (i % 1200 == 0)
	// 		*((unsigned int *)(v.addr + i)) = color_int;
	// }
	
// 	t_edge	ed;

// 	int	i;
// 	ed.col = 0x00003300;
// 	ed.i = 0;
// 	while (ed.i < maps->yaxis - 1)
// 	{
// 		ed.j = 0;
// 		while (ed.j < maps->xaxis)
// 		{
// 			ed_setup(maps, &ed, 1);
// 			ed.g_o = gradient(ed.yy, ed.yy2, ed.exx, ed.eyx2);
// 			ed.x = maps->map_vec[ed.i][ed.j].x;
// 			ed.y_o = maps->map_vec[ed.i][ed.j].y_o;
// 			while (ed.y_o < maps->map_vec[ed.i + 1][ed.j].y_o)
// 			{
// 				//mlx_pixel_put(mlx_ptr, win_ptr, (int)ed.x,
// 				//	(int)ed.y_o, ed.col);
// 				printf("%d\n", (int)ed.y_o);
// 				i = ((int)ed.y_o * 1200) + ((int)ed.x);  
// 				*((unsigned int *)(v.addr + i)) = color_int;
// 				ed.x += ed.g_o;
// 				ed.y_o++;
// 			}
// 			ed.j++;
// 		}
// 		ed.i++;
// 	}
	
	int ofs;
	int	i;
	int	j;

	for (int i = 0; i < (1200 * 800) * 4; i++)
	{
			*((unsigned int *)(v.addr + i)) = black;
	}

	// add limits to stop loop trying to draw pixels out side of image
	// if < y == n ...
	i = 0;
	while (i < maps->yaxis - 1)
	{
		j = 0;
		while (j < maps->xaxis)
		{
			// mlx_pixel_put(p_vars->mlx_ptr, p_vars->win_ptr, (int)maps->map_vec[i][j].x,
			// 	(int)maps->map_vec[i][j].y, 0x00FFFFFF);
			ofs = ((int)maps->map_vec[i][j].y * 4800) + ((int)maps->map_vec[i][j].x * 2);
			//printf("%d\n", (int)maps->map_vec[i][j].x);
 			*((unsigned int *)(v.addr + ofs)) = color_int;
			j++;
		}
		i++;
	}

	//* (data->img.bits_per_pixel / 8

	mlx_put_image_to_window(p_vars->mlx_ptr, p_vars->win_ptr, p_vars->img, 0, 0);

	return (0);	
}

