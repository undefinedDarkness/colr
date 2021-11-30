#include "../app.h"

struct Colorspace* init_color_spaces(GtkWidget *parent) {
	struct Colorspace *spaces = malloc(sizeof(struct Colorspace) * ENABLED_COLOR_SPACES);
	struct Colorspace rgb = { create_color_row("RGB", parent), &color_to_rgb };
	struct Colorspace hex = { create_color_row("HEX", parent), &color_to_hex };
	struct Colorspace hsv = { create_color_row("HSV", parent), &color_to_hsv }; 
	spaces[0] = rgb; 
	spaces[1] = hex;
	spaces[2] = hsv;
	return spaces;
};
