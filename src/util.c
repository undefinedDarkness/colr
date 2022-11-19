#include "util.h"
#include "ui/ui.h"
#include <ctype.h>


char modB[100];

void paste_color_to_clipboard(UNUSED GtkWidget* parent, struct Color *c) {
	GtkClipboard* clip = gtk_clipboard_get_default(gdk_display_get_default());
	color_to_hex(*c,  modB);
	gtk_clipboard_set_text(clip, modB, 8);
}

void button_cursor(GtkWidget *btn, char*cursor) {
  gdk_window_set_cursor(
                        gtk_button_get_event_window(GTK_BUTTON(btn)),
                        gdk_cursor_new_from_name(gdk_display_get_default(), cursor == NULL ? "pointer" : cursor));
}

void paste_label_to_clipboard(UNUSED GtkWidget* parent, GtkWidget *source) {
	GtkClipboard* clip = gtk_clipboard_get_default(gdk_display_get_default());
	const char *to_paste = gtk_label_get_text(GTK_LABEL(source));
	gtk_clipboard_set_text(clip, to_paste, -1);
}

// Simply frees the 2nd item it gets
void free_2nd(UNUSED GtkWidget *widget, struct CallbackData *data) {
	free(data);
}

// See: https://www.cc.gatech.edu/data_files/public/doc/gtk/tutorial/gtk_tut-14.html
void attach_menu(UNUSED GtkWidget* self, GdkEvent *event, GtkWidget *menu) {
	if (event->type == GDK_BUTTON_PRESS) {
		GdkEventButton *event_button;
		event_button = (GdkEventButton *)event;
		if (event_button->button == GDK_BUTTON_SECONDARY) {
			gtk_menu_popup_at_pointer(GTK_MENU(menu), event);
		}
	}
}



// This function is the opposite of tiny...
void save_to_disk(UNUSED GtkWidget*self, GtkWidget*sidebar) {
	GtkWidget *chooser = gtk_file_chooser_dialog_new(
			"Save As",
			NULL,
			GTK_FILE_CHOOSER_ACTION_SAVE,
			"Cancel",
			GTK_RESPONSE_CANCEL,
			"Save",
			GTK_RESPONSE_ACCEPT,
			NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(chooser), TRUE);
	/* gtk_window_resize(GTK_WINDOW(chooser), 300, 200); */
	if (gtk_dialog_run(GTK_DIALOG(chooser)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));

		// Get all the children
		GList *children = gtk_container_get_children(GTK_CONTAINER(sidebar));
		children=children->next; // Skip picker button
		FILE *file = fopen(filename, "w");
		while (children != NULL) {
			struct Color c = color_get_bg(children->data);
			fprintf(file, "#%02x%02x%02x\n", c.r, c.g, c.b);
			children = children->next;
		}

		// Cleanup
		g_list_free(children);
		free(filename);
		fclose(file);
	}
	gtk_widget_destroy(chooser);
}

void parse_colors_from_file(const char* path, struct CallbackData *ui) {
	/* g_print("parsing from: '%s'\n", path); */
	FILE *file = fopen(path, "r");
	while (fgets(modB, 9, file)) {
		modB[7] = '\0'; // strip newline
		ui->color_data = color_from_hex(modB);
		add_new_color(ui);	
		/* g_print("%s", buf); */
	}
	fclose(file);
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
		char *ft = g_content_type_guess(path, NULL, 0, NULL);
		if (strcmp(ft, "application/octet-stream") == 0 || strcmp(ft, "text/plain") == 0) { 
			parse_colors_from_file(path, ui);
		}
		else if (strncmp("image/", ft, sizeof("image/")) == 0) {
			struct Color c;
			color_get_dominant(path, &c);
			ui->color_data = c;
			add_new_color(ui);
		}
		else {
			g_warning("Unknown filetype: %s", ft);
		}
		free(uris);
	}

	g_free(type);
}
