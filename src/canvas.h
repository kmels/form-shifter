#ifndef CANVAS
#define CANVAS

#include "types.h"

void canvas_paint_initial_state();
void canvas_repaint();
gboolean canvas_redraw(GtkWidget *widget, gpointer data);
gboolean canvas_handle_mouse(GtkWidget *widget, void *e, gpointer *t);
gboolean canvas_coordinate_is_within_polygon(double x, double y,FilledPolygon *polygon);
#endif
