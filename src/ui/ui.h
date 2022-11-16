#ifndef UI_H
#define UI_H
#include "../app.h"

void apply_style (GtkWidget *w, char*style);
void show_color(UNUSED GtkWidget *widget, struct CallbackData *data); 
void add_new_color(struct CallbackData *data);
GtkWidget *create_color_row(const char *label, GtkWidget *panel); 
GtkWidget *create_menu(struct CallbackData *ui);
void remove_current_color(UNUSED GtkWidget*self, struct CallbackData *data);

#endif
