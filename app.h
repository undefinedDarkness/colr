#ifndef APP_INCLUDED
#define APP_INCLUDED

#define UNUSED __attribute__((unused))	
#define BOX gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0)
#include <gtk/gtk.h>
#include "color.h"
#include <stdlib.h>

// Stores a bunch of widgets that callbacks need to use, and a color
struct CallbackData {
	GtkWidget *sidebar;

	GtkWidget *color;
	GtkWidget *color_light;
	GtkWidget *color_dark;
	
	GtkWidget *rgb;
	GtkWidget *hex;
	GtkWidget *hsv;
	struct Color color_data;
};

GtkWidget *create_color_row(const char *label, GtkWidget *panel);
void show_color(UNUSED GtkWidget *widget, struct CallbackData *data);
void load_css();
void add_new_color(UNUSED GtkWidget *widget, struct CallbackData *data);

#endif
