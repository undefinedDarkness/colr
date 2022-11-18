#ifndef APP_INCLUDED
#define APP_INCLUDED

/* -- IMPORTS -- */
#include <gtk/gtk.h>
#include "color/color.h"
#include <stdlib.h>
#include <string.h>

/* -- MACROS -- */
#define UNUSED __attribute__((unused))	
#define SZ(arr) sizeof(arr)/sizeof(arr[0])

// Shameful
#define BOX gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0)

/* -- STRUCTURES -- */

// TODO: This structure is like a sticky goo infecting everything
// I know its dumb, but I am really too lazy to fix it

struct UIData {
	GtkWidget *sidebar;
	GtkWidget *panel;
	GtkWidget *picker;
	GtkWidget *window;

	GtkWidget *rgb_display;
	GtkWidget *hex_display;
	GtkWidget *hsv_display;
};
typedef struct UIData UI;

struct CallbackData {
	UI* UI;
	GtkWidget *color;
	GtkWidget *color_light;
	GtkWidget *color_dark;

	struct Color color_data; // I am not entirely sure why we have this
};
typedef struct CallbackData STATE;

enum FileTypes {
	FT_PLAINTEXT,
	FT_URILIST
};

#endif
