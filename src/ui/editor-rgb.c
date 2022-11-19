#include "editor.h"

// Internal State Structure.
struct EditorCB {
	GtkWidget *scale_r;
	GtkWidget *scale_g;
	GtkWidget *scale_b;
	struct CallbackData *cb;
};

// TODO:
// Since most of the format stays the same, Why recreate it every time
static char *create_color_range_gradient(struct Color c, enum ColorChannel channel) {
	switch(channel) {
		case COLOR_CHANNEL_RED:
			sprintf(modB, "trough{background-image:linear-gradient(to right,#00%02x%02x,#ff%02x%02x);}", c.g, c.b, c.g, c.b);
			break;
		case COLOR_CHANNEL_BLUE:
			sprintf(modB, "trough{background-image:linear-gradient(to right,#%02x%02x00,#%02x%02xff);}", c.r, c.g, c.r, c.g);
			break;
		case COLOR_CHANNEL_GREEN:
			sprintf(modB, "trough{background-image:linear-gradient(to right,#%02x00%02x,#%02xff%02x);}", c.r, c.b, c.r, c.b);
			break;
	}
	return modB;
}


static void update_r(GtkWidget *self, struct EditorCB *ui) {
	ui->cb->color_data.r = gtk_range_get_value(GTK_RANGE(self));

	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_GREEN);
	apply_style(ui->scale_g, modB);
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_BLUE);
	apply_style(ui->scale_b, modB);

	show_color(NULL, ui->cb);
};

static void update_g(GtkWidget *self, struct EditorCB *ui) {

	ui->cb->color_data.g = gtk_range_get_value(GTK_RANGE(self));
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_RED);
	apply_style(ui->scale_r, modB);
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_BLUE);
	apply_style(ui->scale_b, modB);

	show_color(NULL, ui->cb);
};

static void update_b(GtkWidget *self, struct EditorCB *ui) {
	ui->cb->color_data.b = gtk_range_get_value(GTK_RANGE(self));
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_RED);
	apply_style(ui->scale_r, modB);
	create_color_range_gradient(ui->cb->color_data, COLOR_CHANNEL_GREEN);
	apply_style(ui->scale_g, modB);

	show_color(NULL, ui->cb);
};

static void update_from_src(UNUSED GtkWidget* parent, GtkWidget* self, UNUSED int index, struct EditorCB *ui) {
	GtkWidget *area = gtk_widget_get_parent(gtk_widget_get_parent(ui->scale_r));

	if (area == self) {
		Color c = ui->cb->color_data;
		gtk_range_set_value(GTK_RANGE(ui->scale_r), c.r);
		gtk_range_set_value(GTK_RANGE(ui->scale_g), c.g);
		gtk_range_set_value(GTK_RANGE(ui->scale_b), c.b);
	}

}

void rgb_editor(GtkWidget*nb, STATE *cbd) {
	struct EditorCB *ui = malloc(sizeof(struct EditorCB));
	ui->cb = cbd;
	Color c = color_get_bg(cbd->color); // Get starting color
	cbd->color_data = c; 

	GtkWidget* content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8); 

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

	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->scale_r, "Red"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->scale_g, "Green"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->scale_b, "Blue"));

	g_signal_connect(G_OBJECT(nb), "switch-page", G_CALLBACK(update_from_src), ui);
	gtk_notebook_append_page(GTK_NOTEBOOK(nb), content, gtk_label_new("RGB"));
}
