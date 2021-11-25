struct Color {
	int r;
	int g;
	int b;
};

enum ColorChannel { COLOR_CHANNEL_RED, COLOR_CHANNEL_GREEN, COLOR_CHANNEL_BLUE };

// Misc
struct Color color_pick ();
struct Color color_apply (struct Color *c, int amount);
char *create_color_range_gradient(struct Color c, enum ColorChannel, char*buffer);

// Color Conversion Functions
char* color_to_hex(struct Color *color, char*space);
char* color_to_rgb(struct Color *color, char*space);
char *color_to_hsv(struct Color *color, char*space);

// Get & Set Widget Background
void color_set_bg(struct Color *color_data, GtkWidget *widget);
struct Color color_get_bg(GtkWidget *self);
