#include "editor.h"

void rgb_editor(GtkWidget*, STATE *cbd);
void hsv_editor(GtkWidget*, STATE *cbd);

GtkWidget *label_scale(GtkWidget *scale, const char* label) {
	GtkWidget * container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	GtkWidget *lw = gtk_label_new(label);
	gtk_label_set_yalign(GTK_LABEL(lw), 0.7);
	gtk_box_pack_start(GTK_BOX(container), lw, 0, 0, 16);
	gtk_box_pack_start(GTK_BOX(container), scale, 1, 1, 0);
	return container;
}


void color_edit_menu(UNUSED GtkWidget *self, STATE *state) {

	GtkWidget *dialog = gtk_dialog_new();
	gtk_window_set_default_size(GTK_WINDOW(dialog), 250, 150);

	GtkWidget *area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_style_context_add_class(gtk_widget_get_style_context(area), "no-padding");

	GtkWidget *notebook = gtk_notebook_new();

	rgb_editor(notebook, state);
	hsv_editor(notebook, state);

	gtk_box_set_homogeneous(GTK_BOX(area), FALSE);
	gtk_box_pack_start(GTK_BOX(area), notebook, 0, 1, 0);
	gtk_widget_show_all(area);

	gtk_dialog_run(GTK_DIALOG(dialog));
} 
