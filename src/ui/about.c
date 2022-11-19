#include "../app.h"

void about_dialog(UNUSED GtkWidget* self, GtkWidget * parent) {
	GtkWidget *diag = gtk_dialog_new_with_buttons("ABOUT", GTK_WINDOW(parent), 0, "OK", GTK_RESPONSE_ACCEPT, NULL);
	
	GtkWidget *area = gtk_dialog_get_content_area(GTK_DIALOG(diag));
	GtkWidget *emma = gtk_image_new_from_resource("/undefinedDarkness/colr/emma.png");
	GtkWidget *text = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(text), "<b>COLR</b>: A simple color picker built in C with GTK for X11\nAuthor: undefinedDarkness");
	GtkWidget *link = gtk_link_button_new_with_label("https://github.com/undefinedDarkness/colr", "Respository");

	gtk_container_add(GTK_CONTAINER(area), emma);
	gtk_container_add(GTK_CONTAINER(area), text);
	gtk_container_add(GTK_CONTAINER(area), link);
	gtk_box_set_spacing(GTK_BOX(area), 16);

	gtk_widget_show_all(area);
	gtk_dialog_run(GTK_DIALOG(diag));
	gtk_widget_destroy(diag);
}
