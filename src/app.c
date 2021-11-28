#include "app.h"

void add_new_color_from_pick(UNUSED GtkWidget *self, struct CallbackData *ui) {
	ui->color_data = color_pick();
	add_new_color(ui);
}

void add_new_color_from_widget(GtkWidget *self, struct CallbackData *ui) {
	ui->color_data = color_get_bg(self);
	add_new_color(ui);
}

void on_file_drop(
		UNUSED GtkWidget *self, 
		UNUSED GdkDragContext *ctx,
		UNUSED int x, UNUSED int y,
		GtkSelectionData *sel,
		UNUSED int info,
		UNUSED int time,
		struct CallbackData *ui) {
	char* type = gdk_atom_name(gtk_selection_data_get_data_type(sel));
	
	if (strcmp(type, "text/uri-list") == 0) {
		char **uris = gtk_selection_data_get_uris(sel);
		const char* path = g_uri_get_path(g_uri_parse(uris[0], 0, NULL));
		parse_colors_from_file(path, ui);
		free(uris);
	}

	g_free(type);
}

int main(int argc, char ** argv) {
	gtk_init(&argc, &argv);
	register_resource();

	/*
	              Window
	 /-----------------------------------\
	 |  Sidebar |         Panel          |
	 |  .----.  | .--------------------. |
	 |  | 🎨 |  | | Dark | XXX | Light | |
	 |  '----'  | '--------------------' |
	 |  .----.  | .-------------------.  |
	 |  | XX |  | | RGB  --------  📋 |  |
	 |  '----'  | '-------------------'  |
	 \-----------------------------------/
	 
	 */

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Colr Pickr");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

	/* The weird indentation is to show the structure of the widgets with respect to each other.  */
	GtkWidget *layout = BOX;
	gtk_container_add(GTK_CONTAINER(window), layout);

		// Create new sidebar
		GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
		gtk_style_context_add_class(gtk_widget_get_style_context(sidebar), "sidebar");

		GtkWidget *sidebar_container = gtk_scrolled_window_new(NULL, NULL);
		gtk_widget_add_events(sidebar_container, GDK_BUTTON_PRESS_MASK);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sidebar_container), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
		gtk_container_add(GTK_CONTAINER(sidebar_container), sidebar);
		gtk_box_pack_start(GTK_BOX(layout), sidebar_container, 0, 0 , 0);
	
			// Color picker button
			GtkWidget *picker = gtk_button_new_from_icon_name("color-picker", GTK_ICON_SIZE_BUTTON);
			gtk_container_add(GTK_CONTAINER(sidebar), picker);

	// Main preview panel
	GtkWidget *panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(panel), "color-preview");
	gtk_box_pack_end(GTK_BOX(layout), panel, TRUE, TRUE, 0);

		// Create color display row
		GtkWidget *color_row = BOX;
		gtk_box_set_homogeneous(GTK_BOX(color_row), TRUE);
		gtk_container_add(GTK_CONTAINER(panel), color_row);
		gtk_style_context_add_class(gtk_widget_get_style_context(color_row), "color-display");
		
			GtkWidget *color_light = gtk_button_new();
			gtk_container_add(GTK_CONTAINER(color_row), color_light);

			GtkWidget *color = gtk_button_new();
			gtk_container_add(GTK_CONTAINER(color_row), color);

			GtkWidget *color_dark = gtk_button_new();
			gtk_container_add(GTK_CONTAINER(color_row), color_dark);
		
	// Assemble data to pass.
	// TODO: Dont create variables, just use the struct.
	struct CallbackData ui = {
		.color_light = color_light,
		.color = color,
		.color_dark = color_dark,
		.rgb = create_color_row("RGB", panel),
		.hex = create_color_row("HEX", panel),
		.hsv = create_color_row("HSV", panel),
		.sidebar = sidebar,
		.panel = panel,
	};

	// Connect signals
	g_signal_connect(G_OBJECT(sidebar_container), "button-press-event", G_CALLBACK(attach_menu), create_menu(&ui));
	
	g_signal_connect(G_OBJECT(color),       "clicked", G_CALLBACK(color_edit_menu), &ui);
	g_signal_connect(G_OBJECT(color_light), "clicked", G_CALLBACK(add_new_color_from_widget), &ui);
	g_signal_connect(G_OBJECT(color_dark),  "clicked", G_CALLBACK(add_new_color_from_widget), &ui);
	
	g_signal_connect(G_OBJECT(picker), "clicked", G_CALLBACK(add_new_color_from_pick), &ui);
	g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);

	// Setup drag & drop
	GtkTargetEntry targets[] = {
		{ "text/plain", 0, FT_PLAINTEXT },
		{ "text/uri-list", 0, FT_URILIST }
	};
	gtk_drag_dest_set(sidebar, GTK_DEST_DEFAULT_ALL, targets, G_N_ELEMENTS(targets), GDK_ACTION_COPY);
	g_signal_connect(G_OBJECT(sidebar), "drag-data-received", G_CALLBACK(on_file_drop), &ui);

	load_css();
	gtk_widget_show_all(window);
	gtk_widget_hide(panel);
	gtk_main();
}
