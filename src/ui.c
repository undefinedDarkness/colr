#include "app.h"

static void show_color(UNUSED GtkWidget *widget, struct CallbackData *data) {
	struct Color color_light = color_apply(&data->color_data, 25);
	struct Color color_dark = color_apply(&data->color_data, -25);
	color_set_bg(&data->color_data, data->color);
	color_set_bg(&color_light, data->color_light);
	color_set_bg(&color_dark, data->color_dark);

	char *color_text = malloc(20);
	
	color_to_rgb(&data->color_data, color_text);
	gtk_label_set_text(GTK_LABEL(data->rgb), color_text);
	
	color_to_hex(&data->color_data, color_text);
	gtk_label_set_text(GTK_LABEL(data->hex), color_text);
	
	color_to_hsv(&data->color_data, color_text);
	gtk_label_set_text(GTK_LABEL(data->hsv), color_text);

	free(color_text);
}

void add_new_color(struct CallbackData *data) {
	GtkWidget *button = gtk_button_new();
	/* data->color_data = pick_color(); */

	color_set_bg(&data->color_data, button);

	// create a copy 
	struct CallbackData *copy = malloc(sizeof (struct CallbackData));
	memcpy(copy, data, sizeof (struct CallbackData));	
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(show_color), copy); 
	g_signal_connect(G_OBJECT(button), "destroy", G_CALLBACK(free_2nd), copy);
	gtk_widget_show_all(button);
	gtk_container_add(GTK_CONTAINER(data->sidebar), button);
}


GtkWidget *create_color_row(const char *label, GtkWidget *panel) {
	GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
	gtk_style_context_add_class(gtk_widget_get_style_context(row), "color-row");
	gtk_box_pack_start(GTK_BOX(row), gtk_label_new(label), 0, 0, 0);
	GtkWidget *show = gtk_label_new("");
	GtkWidget *clipboard = gtk_button_new_from_icon_name("gtk-copy", GTK_ICON_SIZE_BUTTON);
	g_signal_connect(G_OBJECT(clipboard), "clicked", G_CALLBACK(paste_to_clipboard), show);
	gtk_box_pack_start(GTK_BOX(row), show, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(row), clipboard, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(panel), row);
	return show;
}

// See: https://www.cc.gatech.edu/data_files/public/doc/gtk/tutorial/gtk_tut-14.html
void attach_menu(GtkWidget* self, GdkEvent *event, GtkWidget *sidebar) {
	// TODO: This might be created multiple times,
	// And created a menu manually is just painful :(
	GtkWidget *menu = gtk_menu_new();
	GtkWidget *item = gtk_menu_item_new_with_label("Save Palette");
	gtk_container_add(GTK_CONTAINER(menu), item);
	gtk_widget_show_all(menu);
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(save_to_disk), sidebar);	

	if (event->type == GDK_BUTTON_PRESS) {
		GdkEventButton *event_button;
		event_button = (GdkEventButton *)event;
		if (event_button->button == GDK_BUTTON_SECONDARY) {
			gtk_menu_popup_at_pointer(GTK_MENU(menu), event);
		}
	}
}
