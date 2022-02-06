#include "app.h"
#include "resources.h"
#include <ctype.h>

void paste_color_to_clipboard(UNUSED GtkWidget* parent, struct Color *c) {
	GtkClipboard* clip = gtk_clipboard_get_default(gdk_display_get_default());
	/* const char *to_paste = gtk_label_get_text(GTK_LABEL(source)); */
	char *to_paste = malloc(8);
	color_to_hex(c, to_paste);
	gtk_clipboard_set_text(clip, to_paste, 8);
	free(to_paste);
}

void button_cursor(GtkWidget *btn, char*cursor) {
  gdk_window_set_cursor(
                        gtk_button_get_event_window(GTK_BUTTON(btn)),
                        gdk_cursor_new_from_name(gdk_display_get_default(), cursor == NULL ? "pointer" : cursor));
}

void paste_label_to_clipboard(UNUSED GtkWidget* parent, GtkWidget *source) {
	GtkClipboard* clip = gtk_clipboard_get_default(gdk_display_get_default());
	const char *to_paste = gtk_label_get_text(GTK_LABEL(source));
	/* char *to_paste = malloc(8); */
	/* color_to_hex(&c, to_paste); */
	gtk_clipboard_set_text(clip, to_paste, 8);
	/* free(to_paste); */
}

int starts_with(char *check, char *full) {
	for (int i = 0; i < strlen(check); i++) {
		if (check[i] != full[i]) {
			return 1;
		}
	}
	return 0;
}

void upper_case(char *s) {
  for (int i = 0; i < strlen(s); i++) {
    s[i] = toupper(s[i]);
  }
}

// Simply frees the 2nd item it gets
void free_2nd(UNUSED GtkWidget *widget, struct CallbackData *data) {
	free(data);
}

// See: https://www.cc.gatech.edu/data_files/public/doc/gtk/tutorial/gtk_tut-14.html
void attach_menu(GtkWidget* self, GdkEvent *event, GtkWidget *menu) {
	if (event->type == GDK_BUTTON_PRESS) {
		GdkEventButton *event_button;
		event_button = (GdkEventButton *)event;
		if (event_button->button == GDK_BUTTON_SECONDARY) {
			gtk_menu_popup_at_pointer(GTK_MENU(menu), event);
		}
	}
}

void load_css() {
	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_resource(provider, "/undefinedDarkness/colr/app.css");
	/* gtk_css_provider_load_from_path(provider, "app.css", NULL); */
	GdkScreen *display = gdk_display_get_default_screen(gdk_display_get_default());
	gtk_style_context_add_provider_for_screen(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void remove_current_color(UNUSED GtkWidget*self, struct CallbackData *data) {
	GList *children = gtk_container_get_children(GTK_CONTAINER(data->sidebar));
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
	char *buf = malloc(10);
	FILE *file = fopen(path, "r");
	while (fgets(buf, 9, file)) {
		buf[7] = '\0'; // strip newline
		ui->color_data = color_from_hex(buf);
		add_new_color(ui);	
		/* g_print("%s", buf); */
	}
	free(buf);
	fclose(file);
}

void init_resource() {
	GResource *res = index_get_resource();
	g_resources_register(res);
	
	GtkIconTheme *theme = gtk_icon_theme_get_default();
	gtk_icon_theme_add_resource_path (theme, "/undefinedDarkness/colr/icons");
}
