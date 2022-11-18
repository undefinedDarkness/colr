#include "../app.h"

void about_dialog(UNUSED GtkWidget* self, GtkWidget * parent) {
	GtkWidget *diag = gtk_message_dialog_new_with_markup(
			GTK_WINDOW(parent),
			0,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			"<b>Colr</b>\nA simple color picker built in C with GTK");
	
	GtkWidget *area = gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(diag));
	GtkWidget *link = gtk_link_button_new_with_label("https://github.com/undefinedDarkness/colr", "Repository");
	gtk_container_add(GTK_CONTAINER(area), link);

	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(diag), "Author: undefinedDarkness");
	gtk_widget_show_all(area);
	gtk_dialog_run(GTK_DIALOG(diag));
	gtk_widget_destroy(diag);
}
