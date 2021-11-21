struct Color {
	int r;
	int g;
	int b;
};
struct Color color_pick ();
struct Color color_apply (struct Color *c, int amount);
struct Color color_get_bg(GtkWidget *self);
char* color_to_hex(struct Color *color, char*space);
char* color_to_rgb(struct Color *color, char*space);
char *color_to_hsv(struct Color *color, char*space);
void color_set_bg(struct Color *color_data, GtkWidget *widget);
