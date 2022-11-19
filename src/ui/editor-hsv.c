#include "editor.h"

static char *create_color_range_gradient(ColorHSV a, ColorHSV b) {
	Color c1 = color_hsv_re(a);
	Color c2 = color_hsv_re(b);
	sprintf(modB, "trough{background-image:linear-gradient(to right,rgb(%d,%d,%d),rgb(%d,%d,%d));}", c1.r, c1.g, c1.b, c2.r, c2.g, c2.b);
	return modB;
}

// Internal State Structure.
struct EditorCB {
	GtkWidget *hue;
	GtkWidget *saturation;
	GtkWidget *value;
	ColorHSV c;
	struct CallbackData *cb;
};



static void update_hue(GtkWidget *self, struct EditorCB *ui) {
	ui->c.hue = gtk_range_get_value(GTK_RANGE(self));
	ui->cb->color_data = color_hsv_re(ui->c);
	ColorHSV c = ui->c;

	create_color_range_gradient((ColorHSV){c.hue,0,c.value}, (ColorHSV){c.hue,100,c.value});
	apply_style(ui->saturation, modB);
	
	create_color_range_gradient((ColorHSV){c.hue,c.saturation,0}, (ColorHSV){c.hue,c.saturation,100});
	apply_style(ui->value, modB);

	show_color(NULL, ui->cb);
}

static void update_saturation(GtkWidget*self, struct EditorCB *ui) {
	ui->c.saturation = gtk_range_get_value(GTK_RANGE(self));
	ui->cb->color_data = color_hsv_re(ui->c);
	ColorHSV c = ui->c;
	
	create_color_range_gradient((ColorHSV){c.hue,c.saturation,0}, (ColorHSV){c.hue,c.saturation,100});
	apply_style(ui->value, modB);

	show_color(NULL, ui->cb);
}

static void update_value(GtkWidget*self, struct EditorCB *ui) {
	ui->c.value = gtk_range_get_value(GTK_RANGE(self));
	ui->cb->color_data = color_hsv_re(ui->c);
	ColorHSV c = ui->c;
	
	create_color_range_gradient((ColorHSV){c.hue,0,c.value}, (ColorHSV){c.hue,100,c.value});
	apply_style(ui->saturation, modB);

	show_color(NULL, ui->cb);
}

static void update_from_src(UNUSED GtkWidget* parent, GtkWidget* self, UNUSED int index, struct EditorCB *ui) {
	GtkWidget *area = gtk_widget_get_parent(gtk_widget_get_parent(ui->hue));

	if (area == self) {
		ColorHSV c = color_hsv(ui->cb->color_data);
		gtk_range_set_value(GTK_RANGE(ui->hue), c.hue);
		gtk_range_set_value(GTK_RANGE(ui->saturation), c.saturation);
		gtk_range_set_value(GTK_RANGE(ui->value), c.value);
	}

}

void hsv_editor(GtkWidget* notebook, STATE *cbd) {
	struct EditorCB *ui = malloc(sizeof(struct EditorCB));
	ui->cb = cbd;
	cbd->color_data = color_get_bg(cbd->color); // Get starting color
	ColorHSV c = color_hsv(cbd->color_data);
	ui->c = c;

	GtkWidget* content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
	gtk_box_set_homogeneous(GTK_BOX(content), FALSE);

	const char rainbow[] = "trough{background-image:linear-gradient(90deg, rgba(255,0,0,1) 0%, rgba(255,154,0,1) 10%, rgba(208,222,33,1) 20%, rgba(79,220,74,1) 30%, rgba(63,218,216,1) 40%, rgba(47,201,226,1) 50%, rgba(28,127,238,1) 60%, rgba(95,21,242,1) 70%, rgba(186,12,248,1) 80%, rgba(251,7,217,1) 90%, rgba(255,0,0,1) 100%)}";

	ui->hue = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 360, 5);
	gtk_range_set_value(GTK_RANGE(ui->hue), c.hue);
	apply_style(ui->hue, (char*)rainbow);

	ui->saturation = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 5);
	gtk_range_set_value(GTK_RANGE(ui->saturation), c.saturation);
	ui->value = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 5);
	gtk_range_set_value(GTK_RANGE(ui->value), c.value);

	update_hue(ui->hue, ui); update_saturation(ui->saturation, ui);/* update_b(ui->value, ui); */
	g_signal_connect(G_OBJECT(ui->hue), "value-changed", G_CALLBACK(update_hue), ui);
	g_signal_connect(G_OBJECT(ui->saturation), "value-changed", G_CALLBACK(update_saturation), ui);
	g_signal_connect(G_OBJECT(ui->value), "value-changed", G_CALLBACK(update_value), ui);

	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->hue, "HUE"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->saturation, "SATURATION"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->value, "VALUE"));

	// final
	g_signal_connect(G_OBJECT(notebook), "switch-page", G_CALLBACK(update_from_src), ui);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), content, gtk_label_new("HSV"));
}
