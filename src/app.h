#ifndef APP_INCLUDED
#define APP_INCLUDED

/* -- IMPORTS -- */
#include <gtk/gtk.h>
#include "color/color.h"
#include <stdlib.h>
#include <string.h>

/* -- MACROS -- */
#define UNUSED __attribute__((unused))	

// Shameful
#define BOX gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0)

#define ENABLED_COLOR_SPACES 2

/* -- STRUCTURES -- */
struct Colorspace {
	GtkWidget *display;
	void (*formatter)(struct Color*, char*);
	char *fields[3];
};

struct CallbackData {
	GtkWidget *sidebar;
	GtkWidget *panel;

	GtkWidget *color;
	GtkWidget *color_light;
	GtkWidget *color_dark;
	
	struct Colorspace* color_spaces;
	struct Color color_data; // I am not entirely sure why we have this
};

enum FileTypes {
	FT_PLAINTEXT,
	FT_URILIST
};

#endif
