typedef struct _GtkWidget GtkWidget;

struct Color {
	int r;
	int g;
	int b;
};

typedef struct Color Color;

enum ColorChannel { COLOR_CHANNEL_RED, COLOR_CHANNEL_GREEN, COLOR_CHANNEL_BLUE };

struct ColorHSV {
	double hue;
	double saturation;
	double value;
};

typedef struct ColorHSV ColorHSV;
ColorHSV color_hsv(Color);
Color color_hsv_re(ColorHSV);

// Misc
int color_pick (struct Color *color);
struct Color color_apply (struct Color *c, int amount);
/* char *create_color_range_gradient(struct Color c, enum ColorChannel, char*buffer); */
struct Color color_from_hex(char *hex);
void color_get_dominant(const char*path, struct Color *c);

// Color Conversion Functions
void color_to_hex(struct Color color, char*space);
void color_to_rgb(struct Color color, char*space);
void color_to_hsv(struct Color color, char*space);

// TODO: Move to UI
// Get & Set Widget Background
void color_set_bg(struct Color *color_data, GtkWidget *widget);
struct Color color_get_bg(GtkWidget *self);
