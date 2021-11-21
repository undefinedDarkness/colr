#include "app.h"

void load_css() {
	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(provider, "app.css", NULL);
	GdkScreen *display = gdk_display_get_default_screen(gdk_display_get_default());
	gtk_style_context_add_provider_for_screen(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void show_color(UNUSED GtkWidget *widget, struct CallbackData *data) {
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

void free_2nd(UNUSED GtkWidget *widget, struct CallbackData *data) {
	free(data);
}

void add_new_color(UNUSED GtkWidget *widget, struct CallbackData *data) {
	GtkWidget *button = gtk_button_new();
	data->color_data = pick_color();

	color_set_bg(&data->color_data, button);

	// create a copy 
	struct CallbackData *copy = malloc(sizeof (struct CallbackData));
	memcpy(copy, data, sizeof (struct CallbackData));	
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(show_color), copy); 
	g_signal_connect(G_OBJECT(button), "destroy", G_CALLBACK(free_2nd), copy);
	gtk_widget_show_all(button);
	gtk_container_add(GTK_CONTAINER(data->sidebar), button);
}

void paste_to_clipboard(UNUSED GtkWidget* parent, GtkWidget *source) {
	GtkClipboard* clip = gtk_clipboard_get_default(gdk_display_get_default());
	const char *to_paste = gtk_label_get_text(GTK_LABEL(source));
	gtk_clipboard_set_text(clip, to_paste, strlen(to_paste));
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
