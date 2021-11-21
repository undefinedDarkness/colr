#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "color.h"

char *color_to_hex(struct Color *color, char*space) {
	sprintf(space, "#%02x%02x%02x", color->r, color->g, color->b);
	return space;
}

char *color_to_rgb(struct Color *color,char*space) {
	sprintf(space, "rgb(%d,%d,%d)", color->r, color->g, color->b);
	return space;
}

char *color_to_hsv(struct Color *color, char*space) {
	double h, s, v;
	gtk_rgb_to_hsv(color->r/255., color->g/255., color->b/255., &h, &s, &v);
	sprintf(space, "hsv(%.0f,%.0f%%,%.0f%%)", (h * 255) > 0 ? (h * 255) + 10 : 0, s * 100, v * 100);
	return space;
}

// Apply a color to a widget's background
void color_set_bg(struct Color *color_data, GtkWidget *widget) {
	char* hex = malloc(8);
	color_to_hex(color_data, hex);
	GdkRGBA color;
	gdk_rgba_parse(&color, hex); // TODO: Dont use this :(
	gtk_widget_override_background_color(widget, GTK_STATE_NORMAL, &color);
	free(hex);
}

struct Color color_apply(struct Color *c, int amount) {
	struct Color out = {
		.r = c->r + amount,
		.g = c->g + amount,
		.b = c->b + amount
	};
	return out;
}

struct Color pick_color () {
	Display *display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(display);

	Cursor cursor = XCreateFontCursor(display, 130);
	XGrabPointer(display, root, 0,  ButtonPressMask, GrabModeAsync, GrabModeAsync, root, cursor, CurrentTime);

	XWindowAttributes gwa;
	XGetWindowAttributes(display, root, &gwa);

	XGrabKeyboard(display, root, 0, GrabModeAsync, GrabModeAsync, CurrentTime);
	struct Color color;

	for(;;) {
		XEvent e;
		XNextEvent(display, &e);
		XImage *image = XGetImage(display, root, 0, 0, gwa.width, gwa.height, AllPlanes, ZPixmap);
		if (e.type == ButtonPress && e.xbutton.button == Button1) {
				unsigned long pixel = XGetPixel(image, e.xbutton.x_root, e.xbutton.y_root);
				// printf("%d,%d,%d ", (pixel >> 0x10) & 0xFF, (pixel >> 0x08) & 0xFF, pixel & 0xFF);
				color.r = (pixel >> 0x10) & 0xFF;
				color.g = (pixel >> 0x08) & 0xFF;
				color.b = pixel & 0xFF;
				break;
		} else if (e.type == ButtonPress ||
				(e.type == KeyPress && (e.xkey.keycode == 53))) {
			break;
		}
		XDestroyImage(image);
	}

	/* will be done on connection close anw */
	XUngrabPointer(display, CurrentTime);
	XUngrabKeyboard(display, CurrentTime);

	XFreeCursor(display, cursor);
	XDestroyWindow(display, root);
	XCloseDisplay(display);
	return color;
}
