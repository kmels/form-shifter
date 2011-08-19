#ifndef POLYGONS
#define POLYGONS

#include "types.h"

FilledPolygonList* polygons_get_house();
void polygons_paint_on_canvas(FilledPolygon *polygon, cairo_t *cr);
void polygons_list_paint_on_canvas(FilledPolygonList *polygons_list, cairo_t *cr);
void polygons_list_paint_selected_points_on_canvas(FilledPolygonList *polygons_list, cairo_t *cr);
#endif

