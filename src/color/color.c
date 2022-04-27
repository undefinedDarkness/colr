#ifndef SCREENSHOT_PROGRAM
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#endif
#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "color.h"

struct Color color_from_hex(char *hex)  {
	struct Color re;
	sscanf(hex, "#%02x%02x%02x", &re.r, &re.g, &re.b);
	return re;
}

void color_to_hex(struct Color *color, char*space) {
	sprintf(space, "#%02x%02x%02x", color->r, color->g, color->b);
	/* return space; */
}

void color_to_rgb(struct Color *color,char*space) {
	sprintf(space, "rgb(%d,%d,%d)", color->r, color->g, color->b);
	/* return space; */
}

void color_to_hsv(struct Color *color, char*space) {
	double h, s, v;
	gtk_rgb_to_hsv(color->r/255., color->g/255., color->b/255., &h, &s, &v);
	sprintf(space, "hsv(%.0f,%.0f%%,%.0f%%)", h * 360, s * 100, v * 100);
	/* return space; */
}

// Apply a color to a widget's background
void color_set_bg(struct Color *color_data, GtkWidget *widget, char* space) {
	/* char* hex = malloc(8); */
	color_to_hex(color_data, space);
	GdkRGBA color;
	gdk_rgba_parse(&color, space); // TODO: Dont use this :(
	gtk_widget_override_background_color(widget, GTK_STATE_NORMAL, &color);
	/* free(hex); */
}

struct Color color_get_bg(GtkWidget *self) {
	GdkRGBA color;
	gtk_style_context_get_background_color(gtk_widget_get_style_context(self), GTK_STATE_FLAG_NORMAL, &color);
	/* g_print("rgb(%f,%f,%f)", color.red, color.green, color.blue); */
	struct Color color_data = {
		.r = color.red * 255,
		.b = color.blue * 255,
		.g = color.green * 255
	};
	return color_data;
}

// Conform color to rgb limits
static int color_conform(int c) {
	if (c > 255) {
		return 255;
	} else if (c < 0) {
		return 0;
	} else {
		return c;
	}
}

// Apply modification to all colors
struct Color color_apply(struct Color *c, int amount) {
	struct Color out = {
		.r = color_conform(c->r + amount),
		.g = color_conform(c->g + amount),
		.b = color_conform(c->b + amount)
	};
	return out;
}

char *create_color_range_gradient(struct Color c, enum ColorChannel channel,char*buffer) {
	/* char *buffer = malloc(80); */
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

void color_get_dominant(const char*path, struct Color *c) {
	// Get a image and scale it down to 1x1 to get the dominant color
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(path, 1, 1, FALSE, NULL);
	unsigned char *pixels = gdk_pixbuf_get_pixels(pixbuf);
	c->r = (uint8_t)pixels[0];
	c->g = (uint8_t)pixels[1];
	c->b = (uint8_t)pixels[2];
}

#ifndef SCREENSHOT_PROGRAM
int color_pick (struct Color *color) {
	int status = 0;
	Display *display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(display);

	Cursor cursor = XCreateFontCursor(display, 130);
	XGrabPointer(display, root, 0,  ButtonPressMask, GrabModeAsync, GrabModeAsync, root, cursor, CurrentTime);

	XWindowAttributes gwa;
	XGetWindowAttributes(display, root, &gwa);

	XGrabKeyboard(display, root, 0, GrabModeAsync, GrabModeAsync, CurrentTime);

	for(;;) {
		XEvent e;
		XNextEvent(display, &e);
		XImage *image = XGetImage(display, root, 0, 0, gwa.width, gwa.height, AllPlanes, ZPixmap);
		if (e.type == ButtonPress && e.xbutton.button == Button1) {
				unsigned long pixel = XGetPixel(image, e.xbutton.x_root, e.xbutton.y_root);
				// printf("%d,%d,%d ", (pixel >> 0x10) & 0xFF, (pixel >> 0x08) & 0xFF, pixel & 0xFF);
				color->r = (pixel >> 0x10) & 0xFF;
				color->g = (pixel >> 0x08) & 0xFF;
				color->b = pixel & 0xFF;
				break;
		} else if (e.type == ButtonPress ||
				(e.type == KeyPress && (e.xkey.keycode == 53))) {
			status = -1;
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

	return status;
}
#else
int color_pick (struct Color *color) {
	int status = 0;
	FILE *process = popen(SCREENSHOT_PROGRAM, "r");
	if (process == NULL) {
		fprintf(stderr, "Failed to run command: %s - Are you using any shell script?", SCREENSHOT_PROGRAM);
		exit(1);
	}

	if (fscanf(process, "#%02x%02x%02x", &color->r, &color->g, &color->b) != 3) {
		status = -1;
	}

	pclose(process);

	return status;
}
#endif
