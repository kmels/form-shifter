#ifndef CANVAS
#define CANVAS

#include "types.h"

void canvas_paint_initial_state();
void canvas_repaint();
gboolean canvas_redraw(GtkWidget *widget, gpointer data);
gboolean canvas_handle_mouse(GtkWidget *widget, void *e, gpointer *t);
gboolean canvas_coordinate_is_within_polygon(double x, double y,FilledPolygon *polygon);
void canvas_all_polygons_replace_existing_polygon(FilledPolygon *polygon_to_replace, FilledPolygon *replaced_polygon);
#endif
