#include "ui.h"
#include "../util.h"
#include <assert.h>

static GtkWidget *create_color_row(const char *label, GtkWidget *panel) {
	GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
	gtk_style_context_add_class(gtk_widget_get_style_context(row), "color-row");
	gtk_box_pack_start(GTK_BOX(row), gtk_label_new(label), 0, 0, 0);
	GtkWidget *show = gtk_label_new("");
	gtk_label_set_selectable(GTK_LABEL(show), 1);
	
	GtkWidget *clipboard = gtk_button_new_from_icon_name("copy-button-symbolic", GTK_ICON_SIZE_BUTTON);
	g_signal_connect(G_OBJECT(clipboard), "clicked", G_CALLBACK(paste_label_to_clipboard), show);
    g_signal_connect(G_OBJECT(clipboard), "realize", G_CALLBACK(button_cursor), NULL);
    
	gtk_box_pack_start(GTK_BOX(row), show, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(row), clipboard, 0, 0, 0);

	GtkWidget *frame = gtk_frame_new(NULL);
	gtk_container_add(GTK_CONTAINER(frame), row);
	gtk_container_add(GTK_CONTAINER(panel), frame);
	return show;
}

// Create right click menu
// Callback data has no color

void about_dialog(UNUSED GtkWidget *self, GtkWidget *parent);

static GtkWidget *create_menu(struct CallbackData *state) {
	GtkWidget *right_click_menu = gtk_menu_new();

	GtkWidget *items[] = {
		gtk_menu_item_new_with_label("Save Palette"),
		gtk_menu_item_new_with_label("Copy to Clipboard"),
		gtk_menu_item_new_with_label("Remove Color"),
		gtk_menu_item_new_with_label("About")
	};
	for(int i = 0; i < sizeof(items)/sizeof(GtkWidget*); i++) {
		gtk_container_add(GTK_CONTAINER(right_click_menu), items[i]);
	} 

	g_signal_connect(G_OBJECT(items[3]), "activate", G_CALLBACK(about_dialog), state->UI->window);
	g_signal_connect(G_OBJECT(items[2]), "activate", G_CALLBACK(remove_current_color), state);
	g_signal_connect(G_OBJECT(items[1]), "activate", G_CALLBACK(save_to_disk), state->UI->sidebar);
	g_signal_connect(G_OBJECT(items[0]), "activate", G_CALLBACK(paste_color_to_clipboard), &(state->color_data)); // TODO: Select the one under the mouse, not the selected one
	gtk_widget_show_all(right_click_menu);
	return right_click_menu;
}

static void on_first_display(UNUSED GtkWidget *self, STATE *ui) {
	button_cursor(ui->UI->picker, NULL);
	button_cursor(ui->color_light, NULL);
	button_cursor(ui->color_dark, NULL);

	gtk_widget_hide(ui->UI->panel);
	GtkClipboard *clip = gtk_clipboard_get_default(gdk_display_get_default());
	GtkTargetEntry targets[] = {
		{ "UTF8_STRING", 0, 0 }
	};
	gtk_clipboard_set_can_store(clip, targets, 1);
}

static void before_exit(UNUSED GtkWidget *self, void *_) {
	GtkClipboard *clip = gtk_clipboard_get_default(gdk_display_get_default());
	gtk_clipboard_store(clip);
	gtk_main_quit();
}

STATE init_ui() {
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

	static UI ui; // pretty much used throughout
	static STATE state;

	ui.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(ui.window), "Colr Pickr");
	gtk_window_set_default_size(GTK_WINDOW(ui.window), 400, 300);
	gtk_window_set_type_hint(GTK_WINDOW(ui.window), GDK_WINDOW_TYPE_HINT_DIALOG);

	/* The weird indentation is to show the structure of the widgets with respect to each other.  */
	GtkWidget *layout = BOX;
	gtk_container_add(GTK_CONTAINER(ui.window), layout);

		// Create new sidebar
		ui.sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
		gtk_style_context_add_class(gtk_widget_get_style_context(ui.sidebar), "sidebar");

		GtkWidget *sidebar_container = gtk_scrolled_window_new(NULL, NULL);
		gtk_widget_add_events(sidebar_container, GDK_BUTTON_PRESS_MASK);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sidebar_container), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
		gtk_container_add(GTK_CONTAINER(sidebar_container), ui.sidebar);
		gtk_box_pack_start(GTK_BOX(layout), sidebar_container, 0, 0 , 0);
	
			// Color picker button
			ui.picker = gtk_button_new_from_icon_name("color-picker-button-symbolic", GTK_ICON_SIZE_BUTTON);
			gtk_container_add(GTK_CONTAINER(ui.sidebar), ui.picker);

	// Main preview panel
	ui.panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(ui.panel), "color-preview");
	gtk_box_pack_end(GTK_BOX(layout), ui.panel, TRUE, TRUE, 0);

		// Create color display row
		GtkWidget *color_row = BOX;
		gtk_box_set_homogeneous(GTK_BOX(color_row), TRUE);
		gtk_container_add(GTK_CONTAINER(ui.panel), color_row);
		gtk_style_context_add_class(gtk_widget_get_style_context(color_row), "color-display");
		
			state.color_light = gtk_button_new();
			gtk_container_add(GTK_CONTAINER(color_row), state.color_light);

			state.color = gtk_button_new();
			gtk_container_add(GTK_CONTAINER(color_row), state.color);

			state.color_dark = gtk_button_new();
			gtk_container_add(GTK_CONTAINER(color_row), state.color_dark);

			ui.rgb_display = create_color_row("RGB", ui.panel);
			ui.hex_display = create_color_row("HEX", ui.panel);
			ui.hsv_display = create_color_row("HSV", ui.panel);
		
	state.UI = &ui;

	// Connect signals
	g_signal_connect(G_OBJECT(sidebar_container), "button-press-event", G_CALLBACK(attach_menu), create_menu(&state));
	
	g_signal_connect(G_OBJECT(ui.window), "destroy", G_CALLBACK(before_exit), NULL);
	g_signal_connect(G_OBJECT(ui.window), "show", G_CALLBACK(on_first_display), &state);

	// Setup drag & drop
	GtkTargetEntry targets[] = {
		{ "text/plain", 0, FT_PLAINTEXT },
		{ "text/uri-list", 0, FT_URILIST }
	};
	gtk_drag_dest_set(ui.sidebar, GTK_DEST_DEFAULT_ALL, targets, G_N_ELEMENTS(targets), GDK_ACTION_COPY);
	g_signal_connect(G_OBJECT(ui.sidebar), "drag-data-received", G_CALLBACK(on_file_drop), &ui);
    
	return state;
}
