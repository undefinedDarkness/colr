#ifndef SCREENSHOT_PROGRAM
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#endif
#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "color.h"

#include <math.h>

/* -- CONVERSION --*/

struct Color color_from_hex(char *hex)  {
	struct Color re;
	sscanf(hex, "#%02x%02x%02x", &re.r, &re.g, &re.b);
	return re;
}

void color_to_hex(struct Color color, char*space) {
	sprintf(space, "#%02x%02x%02x", color.r, color.g, color.b);
}

void color_to_rgb(struct Color color,char*space) {
	sprintf(space, "rgb(%d,%d,%d)", color.r, color.g, color.b);
}

void color_to_hsv(struct Color color, char*space) {
	double h, s, v;
	gtk_rgb_to_hsv(color.r/255., color.g/255., color.b/255., &h, &s, &v);
	sprintf(space, "hsv(%.0f,%.0f%%,%.0f%%)", h * 360, s * 100, v * 100);
}

ColorHSV color_hsv(struct Color color) {
	double h, s, v;
	gtk_rgb_to_hsv(color.r/255., color.g/255., color.b/255., &h, &s, &v);
	ColorHSV clr = {
		h * 360, s * 100, v * 100
	};
	return clr;
}

Color color_hsv_re(ColorHSV T) {
	double r, g, b;
	gtk_hsv_to_rgb(T.hue/360., T.saturation/100., T.value/100., &r, &g, &b);
	Color c = {
		.r = r * 255,
		.g = g * 255,
		.b = b * 255
	};
	return c;
}

/* -- GTK --*/

// Apply a color to a widget's background
void color_set_bg(struct Color *color_data, GtkWidget *widget) {
	/* color_to_hex(color_data, space); */
	/* GdkRGBA color; */
	/* gdk_rgba_parse(&color, space); // TODO: Dont use this :( */
	GdkRGBA color = {
		.red = color_data->r / 255.,
		.blue = color_data->b / 255.,
		.green = color_data->g / 255.,
		.alpha = 1
	};
	gtk_widget_override_background_color(widget, GTK_STATE_FLAG_NORMAL, &color);
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
#define color_conform(c) c > 255 ? 255 : ((c < 0) ? 0 : c)

// Apply modification to all colors
struct Color color_apply(struct Color *c, int amount) {
	struct Color out = {
		.r = color_conform(c->r + amount),
		.g = color_conform(c->g + amount),
		.b = color_conform(c->b + amount)
	};
	return out;
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
		if (e.type == ButtonPress && e.xbutton.button == Button1) {
				XImage *image = XGetImage(display, root, 0, 0, gwa.width, gwa.height, AllPlanes, ZPixmap);
				unsigned long pixel = XGetPixel(image, e.xbutton.x_root, e.xbutton.y_root);
				// printf("%d,%d,%d ", (pixel >> 0x10) & 0xFF, (pixel >> 0x08) & 0xFF, pixel & 0xFF);
				color->r = (pixel >> 0x10) & 0xFF;
				color->g = (pixel >> 0x08) & 0xFF;
				color->b = pixel & 0xFF;
				XDestroyImage(image);
				break;
		} else if (e.type == ButtonPress ||
				(e.type == KeyPress && (e.xkey.keycode == 53))) {
			status = -1;
			break;
		}
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
