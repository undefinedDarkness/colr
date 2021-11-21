struct Color {
	unsigned int r;
	unsigned int g;
	unsigned int b;
};
struct Color pick_color ();
struct Color color_apply (struct Color *c, int amount);
char* color_to_hex(struct Color *color, char*space);
char* color_to_rgb(struct Color *color, char*space);
char *color_to_hsv(struct Color *color, char*space);
void color_set_bg(struct Color *color_data, GtkWidget *widget);
