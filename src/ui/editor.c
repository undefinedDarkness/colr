#include "editor.h"

// Internal State Structure.
struct EditorCB {
	GtkWidget *scale_r;
	GtkWidget *scale_g;
	GtkWidget *scale_b;
	char *buffer;
	struct CallbackData *cb;
};

// TODO:
// Since most of the format stays the same, Why recreate it every time
static char *create_color_range_gradient(struct Color c, enum ColorChannel channel,char*buffer) {
	switch(channel) {
		case COLOR_CHANNEL_RED:
			sprintf(buffer, "trough{background-image:linear-gradient(to right,#00%02x%02x,#ff%02x%02x);}", c.g, c.b, c.g, c.b);
			break;
		case COLOR_CHANNEL_BLUE:
			sprintf(buffer, "trough{background-image:linear-gradient(to right,#%02x%02x00,#%02x%02xff);}", c.r, c.g, c.r, c.g);
			break;
		case COLOR_CHANNEL_GREEN:
			sprintf(buffer, "trough{background-image:linear-gradient(to right,#%02x00%02x,#%02xff%02x);}", c.r, c.b, c.r, c.b);
			break;
	}
	return buffer;
}

static GtkWidget *label_scale(GtkWidget *scale, const char* label) {
	GtkWidget * container = BOX;
	GtkWidget *lw = gtk_label_new(label);
	gtk_label_set_yalign(GTK_LABEL(lw), 0.7);
	gtk_container_add(GTK_CONTAINER(container), lw);
	gtk_box_pack_start(GTK_BOX(container), scale, 1, 1, 0);
	return container;
}

static void update_r(GtkWidget *self, struct EditorCB *ui) {
	ui->cb->color_data.r = gtk_range_get_value(GTK_RANGE(self));

	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_GREEN, ui->buffer);
	apply_style(ui->scale_g, ui->buffer);
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_BLUE, ui->buffer);
	apply_style(ui->scale_b, ui->buffer);

	show_color(NULL, ui->cb);
};

static void update_g(GtkWidget *self, struct EditorCB *ui) {

	ui->cb->color_data.g = gtk_range_get_value(GTK_RANGE(self));
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_RED, ui->buffer);
	apply_style(ui->scale_r, ui->buffer);
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_BLUE, ui->buffer);
	apply_style(ui->scale_b, ui->buffer);

	show_color(NULL, ui->cb);
};

static void update_b(GtkWidget *self, struct EditorCB *ui) {
	ui->cb->color_data.b = gtk_range_get_value(GTK_RANGE(self));
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_RED, ui->buffer);
	apply_style(ui->scale_r, ui->buffer);
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_GREEN, ui->buffer);
	apply_style(ui->scale_g, ui->buffer);

	show_color(NULL, ui->cb);
};

static void editor_on_selection(GtkWidget *self, UNUSED int resp, struct EditorCB *ui) {
	add_new_color(ui->cb);
	free(ui->buffer);
	free(ui);
	gtk_widget_destroy(self);
}



void color_edit_menu(UNUSED GtkWidget *self, struct CallbackData *cbd) {
	struct EditorCB *ui = malloc(sizeof(struct EditorCB));
	ui->cb = cbd;
	Color c = color_get_bg(cbd->color); // Get starting color
	cbd->color_data = c; 
	ui->buffer = malloc(100); // This is super ugly yes but it saves a lottt of allocations
	GtkWidget *dialog = gtk_dialog_new_with_buttons(
			"Edit Color", 
			NULL, 
			GTK_DIALOG_MODAL, 
			"Select",
			GTK_RESPONSE_ACCEPT,
			NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog), 250, 180);
	GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	// TODO: Look into using the gtk color scale widget
	ui->scale_r = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 255, 5);
	gtk_range_set_value(GTK_RANGE(ui->scale_r), c.r);
	ui->scale_g = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 255, 5);
	gtk_range_set_value(GTK_RANGE(ui->scale_g), c.g);
	ui->scale_b = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 255, 5);
	gtk_range_set_value(GTK_RANGE(ui->scale_b), c.b);

	update_r(ui->scale_r, ui); update_g(ui->scale_g, ui); update_b(ui->scale_b, ui);
	g_signal_connect(G_OBJECT(ui->scale_r), "value-changed", G_CALLBACK(update_r), ui);
	g_signal_connect(G_OBJECT(ui->scale_g), "value-changed", G_CALLBACK(update_g), ui);
	g_signal_connect(G_OBJECT(ui->scale_b), "value-changed", G_CALLBACK(update_b), ui);
	g_signal_connect(G_OBJECT(dialog),      "response",      G_CALLBACK(editor_on_selection), ui);

	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->scale_r, "Red"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->scale_g, "Green"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->scale_b, "Blue"));
	gtk_widget_show_all(dialog);
}
