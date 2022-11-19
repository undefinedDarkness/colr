#ifndef EDITOR_H
#define EDITOR_H

#include "../util.h"
#include "ui.h"
GtkWidget *label_scale(GtkWidget *scale, const char* label); 
void color_edit_menu(UNUSED GtkWidget *self, struct CallbackData *cbd);

#endif
