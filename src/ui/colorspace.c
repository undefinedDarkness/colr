#include "../app.h"


struct Colorspace* init_color_spaces(GtkWidget *parent) {
	struct Colorspace *spaces = malloc(sizeof(struct Colorspace) * TOTAL_ENABLED_COLOR_SPACES);
	spaces[0] = (struct Colorspace){ create_color_row("RGB", parent), &color_to_rgb }; 
	spaces[1] = (struct Colorspace){ create_color_row("HEX", parent), &color_to_hex };
	spaces[2] = (struct Colorspace){ create_color_row("HSV", parent), &color_to_hsv };
	return spaces;
};
