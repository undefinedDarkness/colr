#ifndef APP_INCLUDED
#define APP_INCLUDED

#define HAS_WINDOW(w) g_print("Has window? %s", gtk_widget_get_has_window(w) == TRUE ? "TRUE" : "FALSE")
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

// User Interface:
GtkWidget *create_color_row(const char *label, GtkWidget *panel);
void add_new_color(struct CallbackData *data);
void attach_menu(GtkWidget *self, GdkEvent *event, GtkWidget *sidebar);

// Tiny:
void load_css();
void paste_to_clipboard(UNUSED GtkWidget* parent, GtkWidget *source);
void free_2nd(UNUSED GtkWidget *widget, struct CallbackData *data);
void save_to_disk(GtkWidget*self, GtkWidget *widget);

#endif
