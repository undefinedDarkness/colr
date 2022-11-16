#include "../app.h"

struct Colorspace* init_color_spaces(GtkWidget *parent) {
	struct Colorspace *spaces = malloc(sizeof(struct Colorspace) * ENABLED_COLOR_SPACES);
	
	spaces[0] = (struct Colorspace){ 
		.display = create_color_row("RGB", parent), 
		.formatter = &color_to_rgb,
		.fields = { "Red", "Blue", "Green" }
	}; 

	spaces[1] = (struct Colorspace){ 
		.display = create_color_row("HSV", parent),
		.formatter = &color_to_hsv,
		.fields = { "Hue", "Saturation", "Value" }
	};

	return spaces;
};
