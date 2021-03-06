#ifndef APP_INCLUDED
#define APP_INCLUDED
#define TOTAL_ENABLED_COLOR_SPACES 3
#define UNUSED __attribute__((unused))	
#define BOX gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0)
#include <gtk/gtk.h>
#include "color/color.h"
#include <stdlib.h>
#include <string.h>
struct Colorspace {
	GtkWidget *display;
	void (*formatter)(struct Color*, char*);
};// Stores a bunch of widgets that callbacks need to use, and a color ;-;
// This is really ugly but I cant be fucked to fix it
struct CallbackData {
	GtkWidget *sidebar;
	GtkWidget *panel;

	GtkWidget *color;
	GtkWidget *color_light;
	GtkWidget *color_dark;
	
	struct Colorspace* color_spaces;

	GtkWidget *scale_r;
	GtkWidget *scale_b;
	GtkWidget *scale_g;
	char *buffer;

	struct Color color_data;
};

enum FileTypes {
	FT_PLAINTEXT,
	FT_URILIST
};

// User Interface:
GtkWidget *create_color_row(const char *label, GtkWidget *panel);
GtkWidget *create_menu(struct CallbackData *ui);
void add_new_color(struct CallbackData *data);
void color_edit_menu (GtkWidget *self, struct CallbackData *ui);
void show_color(UNUSED GtkWidget *widget, struct CallbackData *data);
void apply_style (GtkWidget *w, char*style);
struct Colorspace* init_color_spaces(GtkWidget *parent);

// Behaviour:
void load_css();
int starts_with(char *check, char *full);
void init_resource();
void paste_color_to_clipboard(UNUSED GtkWidget* parent, struct Color *c);
void paste_label_to_clipboard(UNUSED GtkWidget* parent, GtkWidget *copy);
void remove_current_color(UNUSED GtkWidget*self, struct CallbackData *data);
void free_2nd(UNUSED GtkWidget *widget, struct CallbackData *data);
void parse_colors_from_file(const char* path, struct CallbackData *ui);
void save_to_disk(GtkWidget*self, GtkWidget *widget);
void attach_menu(GtkWidget *self, GdkEvent *event, GtkWidget *sidebar);
void button_cursor(GtkWidget *btn, char* cursor);
#endif
