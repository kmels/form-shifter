#ifndef POLYGONS
#define POLYGONS

#include "types.h"

FilledPolygon* polygons_get_house();
FilledPolygonList* get_polygon_node(FilledPolygon *polygon, FilledPolygonList *next);
void polygons_paint_on_canvas(FilledPolygon *polygon, cairo_t *cr);
void polygons_list_paint_on_canvas(FilledPolygonList *polygons_list, cairo_t *cr);
void polygons_list_paint_selected_points_on_canvas(FilledPolygonList *polygons_list, cairo_t *cr);
void polygons_scale_selected (GtkButton *button, gpointer user_data);
void polygons_rotate_selected (GtkButton *button, gpointer user_data);
FilledPolygonDimensions get_polygon_dimensions(FilledPolygon *polygon);
void polygon_get_coordinates_array(Coordinate coordinates[],FilledPolygon *polygon);
void polygons_paint_points_on_canvas(FilledPolygon *polygon, cairo_t *cr);
gboolean polygons_list_contains_polygon(FilledPolygonList *polygons_list,FilledPolygon *polygon);
gboolean polygons_are_equal(FilledPolygon *polygon1,FilledPolygon *polygon2);
#endif
