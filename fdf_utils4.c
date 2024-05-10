/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_utils4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lworden <lworden@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 20:36:14 by lworden           #+#    #+#             */
/*   Updated: 2024/05/10 21:23:00 by lworden          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	free_p(char **p_line)
{
	int	i;

	i = 0;
	while (p_line[i])
	{
		free(p_line[i]);
		i++;
	}
	free(p_line);
}

void	free_all(t_read *r_vars, int end)
{
	free(r_vars->line);
	free_p(r_vars->p_line);
	if (end)
		close(r_vars->fd);
}

void	flat_flip(t_vars *p_vars, t_edge *ed, void *mlx_ptr, void *win_ptr)
{
	if (ed->y == p_vars->m->map_vec[ed->i + 1][ed->j].y)
	{
		while (ed->x > p_vars->m->map_vec[ed->i + 1][ed->j].x)
		{
			draw_image(p_vars, ed->x, ed->y, p_vars->white);
			//mlx_pixel_put(mlx_ptr, win_ptr, (int)ed->x, (int)ed->y, ed->col);
			ed->x--;
		}
	}
	else if (p_vars->m->map_vec[ed->i][ed->j].y > p_vars->m->map_vec[ed->i + 1][ed->j].y)
		flip_y_edge(ed, p_vars, mlx_ptr, win_ptr);
}

void	flip_y_edge(t_edge *ed, t_vars *p_vars, void *mlx_ptr, void *win_ptr)
{
	while (ed->x > p_vars->m->map_vec[ed->i + 1][ed->j].x)
	{
		draw_image(p_vars, ed->x, ed->y, p_vars->white);
		//mlx_pixel_put(mlx_ptr, win_ptr, (int)ed->x, (int)ed->y, 0x00FFFFCC);
		ed->x -= ed->g;
		ed->y--;
	}
}
