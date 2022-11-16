#ifndef UTIL_H
#define UTIL_H
#include "app.h"

void paste_color_to_clipboard(UNUSED GtkWidget* parent, struct Color *c);
void button_cursor(GtkWidget *btn, char*cursor);void paste_label_to_clipboard(UNUSED GtkWidget* parent, GtkWidget *source);
void paste_label_to_clipboard(UNUSED GtkWidget* parent, GtkWidget *source);int starts_with(char *check, char *full);
void on_file_drop(
		UNUSED GtkWidget *self, 
		UNUSED GdkDragContext *ctx,
		UNUSED int x, UNUSED int y,
		GtkSelectionData *sel,
		UNUSED int info,
		UNUSED int time,
		struct CallbackData *ui);
void free_2nd(UNUSED GtkWidget *widget, struct CallbackData *data);void attach_menu(UNUSED GtkWidget* self, GdkEvent *event, GtkWidget *menu);
void remove_current_color(UNUSED GtkWidget*self, struct CallbackData *data);
void save_to_disk(UNUSED GtkWidget*self, GtkWidget*sidebar);
void parse_colors_from_file(const char* path, struct CallbackData *ui);

#endif
