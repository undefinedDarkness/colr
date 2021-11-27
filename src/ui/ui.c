#include "../app.h"

void apply_style (GtkWidget *w, char*style) {
	GtkCssProvider *css = gtk_css_provider_new();
	gtk_css_provider_load_from_data(css, style, strlen(style), NULL);
	gtk_style_context_add_provider(gtk_widget_get_style_context(w) ,GTK_STYLE_PROVIDER(css), 600);
}

void show_color(UNUSED GtkWidget *widget, struct CallbackData *data) {
	struct Color color_light = color_apply(&data->color_data, 25);
	struct Color color_dark = color_apply(&data->color_data, -25);
	char *color_text = malloc(20);
	color_set_bg(&data->color_data, data->color, color_text);
	gtk_label_set_text(GTK_LABEL(data->hex), color_text);
	
	color_set_bg(&color_light, data->color_light, color_text);
	color_set_bg(&color_dark, data->color_dark, color_text);

	
	color_to_rgb(&data->color_data, color_text);
	gtk_label_set_text(GTK_LABEL(data->rgb), color_text);
	
	/* color_to_hex(&data->color_data, color_text); */
	
	color_to_hsv(&data->color_data, color_text);
	gtk_label_set_text(GTK_LABEL(data->hsv), color_text);

	gtk_widget_show_all(data->panel);
	free(color_text);
}

void add_new_color(struct CallbackData *data) {
	// Check for duplicate colors
	GList *children = gtk_container_get_children(GTK_CONTAINER(data->sidebar));
	children=children->next; // Skip picker button
	while (children != NULL) {
			struct Color c = color_get_bg(children->data);
			if (c.r == data->color_data.r && c.g == data->color_data.g && c.b == data->color_data.b) {
				gtk_button_clicked(children->data);
				goto end;
			}
			children = children->next;
	}	

	GtkWidget *button = gtk_button_new();

	char *space = malloc(8);
	color_set_bg(&data->color_data, button, space);
	free(space);

	// Create a copy of the struct 
	struct CallbackData *copy = malloc(sizeof (struct CallbackData));
	memcpy(copy, data, sizeof (struct CallbackData));
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(show_color), copy); 
	g_signal_connect(G_OBJECT(button), "destroy", G_CALLBACK(free_2nd), copy);
	gtk_widget_show_all(button);
	
	// Switch to the new color
	show_color(NULL, copy);
	gtk_container_add(GTK_CONTAINER(data->sidebar), button);

	end:
	g_list_free(children);
}


GtkWidget *create_color_row(const char *label, GtkWidget *panel) {
	GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
	gtk_style_context_add_class(gtk_widget_get_style_context(row), "color-row");
	gtk_box_pack_start(GTK_BOX(row), gtk_label_new(label), 0, 0, 0);
	GtkWidget *show = gtk_label_new("");
	gtk_label_set_selectable(GTK_LABEL(show), 1);
	
	GtkWidget *clipboard = gtk_button_new_from_icon_name("gtk-copy", GTK_ICON_SIZE_BUTTON);
	g_signal_connect(G_OBJECT(clipboard), "clicked", G_CALLBACK(paste_to_clipboard), show);
	/* on_hover_pointer(clipboard); */

	gtk_box_pack_start(GTK_BOX(row), show, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(row), clipboard, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(panel), row);
	return show;
}

// Create right click menu
// Callback data has no color
GtkWidget *create_menu(struct CallbackData *ui) {
	GtkWidget *right_click_menu = gtk_menu_new();
	GtkWidget *item_1 = gtk_menu_item_new_with_label("Save Palette");
	GtkWidget *item_2 = gtk_menu_item_new_with_label("Copy to Clipboard");
	GtkWidget *item_3 = gtk_menu_item_new_with_label("Remove Color");
	
	gtk_container_add(GTK_CONTAINER(right_click_menu), item_1);
	gtk_container_add(GTK_CONTAINER(right_click_menu), item_2);
	gtk_container_add(GTK_CONTAINER(right_click_menu), item_3);
	
	g_signal_connect(G_OBJECT(item_3), "activate", G_CALLBACK(remove_current_color), ui);
	g_signal_connect(G_OBJECT(item_1), "activate", G_CALLBACK(save_to_disk), ui->sidebar);
	g_signal_connect(G_OBJECT(item_2), "activate", G_CALLBACK(paste_to_clipboard), ui->hex); // TODO: Select the one under the mouse, not the selected one
	gtk_widget_show_all(right_click_menu);
	return right_click_menu;
}


