#include "app.h"

void paste_to_clipboard(UNUSED GtkWidget* parent, GtkWidget *source) {
	GtkClipboard* clip = gtk_clipboard_get_default(gdk_display_get_default());
	const char *to_paste = gtk_label_get_text(GTK_LABEL(source));
	gtk_clipboard_set_text(clip, to_paste, strlen(to_paste));
}

void free_2nd(UNUSED GtkWidget *widget, struct CallbackData *data) {
	free(data);
}

inline void load_css() {
	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(provider, "app.css", NULL);
	GdkScreen *display = gdk_display_get_default_screen(gdk_display_get_default());
	gtk_style_context_add_provider_for_screen(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
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
