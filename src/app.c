#include "app.h"
#include "util.h"
#include "ui/ui.h"
#include "ui/editor.h"
#include "../resources/resources.h"

static void add_new_color_from_pick(UNUSED GtkWidget *self, struct CallbackData *ui) {
	if (color_pick(&ui->color_data) == -1) {
		return;
	}
	add_new_color(ui);
}

static void add_new_color_from_widget(GtkWidget *self, struct CallbackData *ui) {
	ui->color_data = color_get_bg(self);
	add_new_color(ui);
}

/* -- Init Subroutines {{{ */

static void init_resource() {
	GResource *res = index_get_resource();
	g_resources_register(res);
	
	GtkIconTheme *theme = gtk_icon_theme_get_default();
	gtk_icon_theme_add_resource_path (theme, "/undefinedDarkness/colr/icons");
}

static void init_styling() {
	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_resource(provider, "/undefinedDarkness/colr/app.css"); // Loaded from resources file.
	GdkScreen *display = gdk_display_get_default_screen(gdk_display_get_default());
	gtk_style_context_add_provider_for_screen(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

/* }}} */

int main(int argc, char ** argv) {
	// Initialize GTK
	gtk_init(&argc, &argv);
	
	init_resource();

	// Build User Interface
	STATE state = init_ui();
	init_styling();

	// Attack Signals
	g_signal_connect(G_OBJECT(state.color),       "clicked", G_CALLBACK(color_edit_menu), &state);
	g_signal_connect(G_OBJECT(state.color_light), "clicked", G_CALLBACK(add_new_color_from_widget), &state);
	g_signal_connect(G_OBJECT(state.color_dark),  "clicked", G_CALLBACK(add_new_color_from_widget), &state);
	g_signal_connect(G_OBJECT(state.UI->picker), "clicked", G_CALLBACK(add_new_color_from_pick), &state);

	// Show Windows	
	gtk_widget_show_all(state.UI->window);

	// Start Runtime
    gtk_main();
}
