#include "editor.h"

static char *create_color_range_gradient(ColorHSV a, ColorHSV b) {
	Color c1 = color_hsv_re(a);
	Color c2 = color_hsv_re(b);
	/* printf("\nHSV: %.0f, %.0f, %.0f => RGB: %d, %d, %d\n", a.hue, a.saturation, a.value, c1.r, c1.b, c1.g); */
	/* printf("HSV: %.0f, %.0f, %.0f => RGB: %d, %d, %d\n", b.hue, b.saturation, b.value, c2.r, c2.b, c2.g); */
	sprintf(modB, "trough{background-image:linear-gradient(to right,rgb(%d,%d,%d),rgb(%d,%d,%d));}", c1.r, c1.g, c1.b, c2.r, c2.g, c2.b);
	return modB;
}

// Internal State Structure.
struct EditorCB {
	GtkWidget *hue;
	GtkWidget *saturation;
	GtkWidget *value;
	/* char *buffer; */
	ColorHSV c;
	struct CallbackData *cb;
};

// TODO: Share this code
static GtkWidget *label_scale(GtkWidget *scale, const char* label) {
	GtkWidget * container = BOX;
	GtkWidget *lw = gtk_label_new(label);
	gtk_label_set_yalign(GTK_LABEL(lw), 0.7);
	gtk_container_add(GTK_CONTAINER(container), lw);
	gtk_box_pack_start(GTK_BOX(container), scale, 1, 1, 0);
	return container;
}
void editor_on_selection(GtkWidget *self, UNUSED int resp, struct EditorCB *ui) {
	add_new_color(ui->cb);
	/* free(ui->buffer); */
	free(ui);
	gtk_widget_destroy(self);
}

static void update_hue(GtkWidget *self, struct EditorCB *ui) {
	// Update HSV representation and RGB representation
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
	// Update HSV representation and RGB representation
	ui->c.saturation = gtk_range_get_value(GTK_RANGE(self));
	ui->cb->color_data = color_hsv_re(ui->c);
	ColorHSV c = ui->c;

	/* create_color_range_gradient((ColorHSV){0,c.saturation,c.value}, (ColorHSV){360,c.saturation,c.value}, ui->buffer); */
	/* apply_style(ui->hue, ui->buffer); */
	
	create_color_range_gradient((ColorHSV){c.hue,c.saturation,0}, (ColorHSV){c.hue,c.saturation,100});
	apply_style(ui->value, modB);

	show_color(NULL, ui->cb);
}

static void update_value(GtkWidget*self, struct EditorCB *ui) {
	// Update HSV representation and RGB representation
	ui->c.value = gtk_range_get_value(GTK_RANGE(self));
	ui->cb->color_data = color_hsv_re(ui->c);
	ColorHSV c = ui->c;

	/* create_color_range_gradient((ColorHSV){0,c.saturation,c.value}, (ColorHSV){360,c.saturation,c.value}, ui->buffer); */
	/* apply_style(ui->hue, ui->buffer); */
	
	create_color_range_gradient((ColorHSV){c.hue,0,c.value}, (ColorHSV){c.hue,100,c.value});
	apply_style(ui->saturation, modB);

	show_color(NULL, ui->cb);
}

void color_edit_menu(UNUSED GtkWidget *self, struct CallbackData *cbd) {
	struct EditorCB *ui = malloc(sizeof(struct EditorCB));
	ui->cb = cbd;
	cbd->color_data = color_get_bg(cbd->color); // Get starting color
	ColorHSV c = color_hsv(cbd->color_data);
	ui->c = c;

	/* ui->buffer = malloc(100); */
	GtkWidget *dialog = gtk_dialog_new_with_buttons(
			"Edit Color", 
			NULL, 
			GTK_DIALOG_MODAL, 
			"Select",
			GTK_RESPONSE_ACCEPT,
			NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog), 250, 180);
	GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_box_set_homogeneous(GTK_BOX(content), TRUE);

	// TODO: Look into using the gtk color scale widget

	// Rainbow background gradient
	// Thanks SO
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
	g_signal_connect(G_OBJECT(dialog),      "response",      G_CALLBACK(editor_on_selection), ui);

	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->hue, "HUE"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->saturation, "SATURATION"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->value, "VALUE"));
	gtk_widget_show_all(dialog);
}
