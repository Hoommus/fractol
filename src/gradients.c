#include "fractol_common.h"

static struct s_gradient *g_gradients[10];

void grad_table_init(void) {
	g_gradients[0] = grad_create_from(GRADIENT_LINEAR, 100, 2,
									  COLOR_WHITE, 0,
									  COLOR_100_BLACK, 100);
	g_gradients[1] = grad_create_from(GRADIENT_LINEAR, 100, 4,
									  COLOR_GOLDEN_YELLOW, 0, 0xFFA216, 30,
									  COLOR_MACHARIUS_SOLAR_ORANGE, 90, COLOR_ULTRAMARINE, 100);
	g_gradients[2] = grad_create_from(GRADIENT_LINEAR, 100, 4,
									  COLOR_ULTRAMARINE, 0, COLOR_WARLOCK_PURPLE, 40,
									  COLOR_MACHARIUS_SOLAR_ORANGE, 70, COLOR_WARLOCK_PURPLE, 100);
	g_gradients[3] = grad_create_from(GRADIENT_LINEAR, 100, 4,
									  COLOR_GREEN, 0, COLOR_WARLOCK_PURPLE, 10, COLOR_RED, 30,
									  COLOR_WARLOCK_PURPLE, 100);
	g_gradients[4] = grad_create_from(GRADIENT_LINEAR, 100, 3,
									  COLOR_GREEN, 0, COLOR_RED, 50, COLOR_BLUE, 100);
	g_gradients[5] = grad_create_from(GRADIENT_LINEAR, 100, 3,
									  COLOR_GREEN, 0, COLOR_GREEN, 30, COLOR_100_BLACK, 100);
	g_gradients[6] = grad_create_from(GRADIENT_LINEAR, 100, 3,
									  0x833ab4, 0, 0xfd1d1d, 50, 0xfcb045, 100);
	g_gradients[7] = grad_create_from(GRADIENT_LINEAR, 100, 2,
									  0x22c1c3, 0, 0xfdbb2d, 100);
	g_gradients[8] = grad_create_from(GRADIENT_LINEAR, 100, 2,
									  COLOR_ULTRAMARINE, 0, COLOR_GOLDEN_YELLOW, 100);
	g_gradients[9] = grad_create_from(GRADIENT_LINEAR, 100, 4,
									  COLOR_DARK_ANGEL_GREEN, 0, COLOR_WARLOCK_PURPLE, 10,
									  COLOR_DARK_ANGEL_GREEN, 30, COLOR_WARLOCK_PURPLE, 100);
}

struct s_gradient *grad_from_table(uint32_t i) {
	return (g_gradients[i > 10 ? i % 10 : i]);
}

void grad_dump(struct s_gradient *gradient) {
	t_gradient_point *point;

	point = gradient->points_list;
	while (point) {
		ft_dprintf(2, "%3d %.6X\n", point->iteration, point->rgba);
		point = point->next;
	}
}
