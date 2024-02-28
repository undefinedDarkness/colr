#include "ui.h"
#include "../util.h"

void apply_style (GtkWidget *w, char*style) {
	GtkCssProvider *css = gtk_css_provider_new();
	gtk_css_provider_load_from_data(css, style, strlen(style), NULL);
	gtk_style_context_add_provider(gtk_widget_get_style_context(w) ,GTK_STYLE_PROVIDER(css), 600);
}

void show_color(UNUSED GtkWidget *widget, struct CallbackData *data) {
	struct Color color_light = color_apply(&data->color_data, 25);
	struct Color color_dark = color_apply(&data->color_data, -25);


	/* Have no clue what this code does */
	/* GList *children = gtk_container_get_children(GTK_CONTAINER(data->UI->sidebar)); */
	/* GList* head = children; */
	/* children=children->next; // Skip picker button */
	/* struct Color previous = color_get_bg(data->color); */
	/* while (children != NULL) { */
	/* 		struct Color c = color_get_bg(children->data); */
	/* 		if (c.r == previous.r && c.g == previous.g && c.b == previous.b) { */
	/* 			color_set_bg(&data->color_data, children->data); */
	/* 			break; */
	/* 		} */
	/* 		children = children->next; */
	/* } */	
	/* g_list_free(head); */

	color_set_bg(&data->color_data, data->color);
	color_set_bg(&color_light, data->color_light);
	color_set_bg(&color_dark, data->color_dark);
	
	color_to_rgb(data->color_data, modB);
	gtk_label_set_text(GTK_LABEL(data->UI->rgb_display), modB);
	
	color_to_hsv(data->color_data, modB);
	gtk_label_set_text(GTK_LABEL(data->UI->hsv_display), modB);

	color_to_hex(data->color_data, modB);
	gtk_label_set_text(GTK_LABEL(data->UI->hex_display), modB);

	gtk_widget_show_all(data->UI->panel);
	/* free(color_text); */
}

void add_new_color(struct CallbackData *data) {
	// Check for duplicate colors
	GList *children = gtk_container_get_children(GTK_CONTAINER(data->UI->sidebar));
	GList* head = children;
	children=children->next; // Skip picker button
	while (children != NULL) {
			struct Color c = color_get_bg(children->data);
			if (c.r == data->color_data.r && c.g == data->color_data.g && c.b == data->color_data.b) {
				gtk_button_clicked(children->data);
				goto end;
			}
			children = children->next;
	}
	g_list_free(head);


	GtkWidget *button = gtk_button_new();

	struct CallbackData *copy = malloc(sizeof (struct CallbackData));
	color_set_bg(&data->color_data, button);

	// Create a copy of the struct to preserve the data
	memcpy(copy, data, sizeof (struct CallbackData));
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(show_color), copy); 
	g_signal_connect(G_OBJECT(button), "destroy", G_CALLBACK(free_2nd), copy); // FREED HERE
	gtk_widget_show_all(button);
	
	// Switch to the new color
	show_color(NULL, copy);
	gtk_container_add(GTK_CONTAINER(data->UI->sidebar), button);
    button_cursor(button, NULL);

	end:
	g_list_free(children);
}






void remove_current_color(UNUSED GtkWidget*self, struct CallbackData *data) {
	GList *children = gtk_container_get_children(GTK_CONTAINER(data->UI->sidebar));
	struct Color ch = color_get_bg(data->color);
	children = children->next; // Skip picker button
	while (children != NULL) {
			struct Color c = color_get_bg(children->data);
			if (ch.r == c.r && ch.g == c.g && ch.b == c.b) {
				gtk_widget_destroy(children->data);
				children = children->next;
				if (children == NULL) {
					children = g_list_first(children);
				}
				gtk_button_clicked(children->data);
				break;
			}
			children = children->next;
	}	
	g_list_free(children);
}
