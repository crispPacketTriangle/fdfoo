#include "fdf.h"

// cc image.c -L ./lib/ -L ./minilibx-linux/ -Lmlx_Linux -lft -lmlx -lXext -lX11 -lz -lm

// void	*mlx_new_image(void *mlx_ptr,int width,int height);
// char	*mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian);
// int		mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y);

typedef struct vv
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
} vv;

int	term(vv *v);

int	main(int argc, char **argv)
{

	vv	v;
	int	bits_per_pixel;
	int	size_line;
	int	endian;
	int color_int;

	bits_per_pixel = 8;
	size_line = 1200;
	endian = 0;

	v.mlx = mlx_init();
	v.win = mlx_new_window(v.mlx, 1200, 800, "cpt");
	
	v.img = mlx_new_image(v.mlx, 1200, 800);

	v.addr = mlx_get_data_addr(v.img, &bits_per_pixel, &size_line, &endian);

	color_int = mlx_get_color_value(v.mlx, 0x00FFFFFF);

	for (int i = 0; i < (1200 * 800) * 4; i++)
	{
		if (i % 100 == 0)
			*((unsigned int *)(v.addr + i)) = color_int;
	}

	mlx_put_image_to_window(v.mlx, v.win, v.img, 0, 0);
	
	mlx_hook(v.win, 17, StructureNotifyMask, &term, &v);
	mlx_loop(v.mlx);

	return (0);
}

int	term(vv *v)
{
	ft_printf("Hasta la vista, baby!\n");
	mlx_destroy_window(v->mlx, v->win);
	mlx_destroy_display(v->mlx);
	free(v->mlx);
	exit(0);
}