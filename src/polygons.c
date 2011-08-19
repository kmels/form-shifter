/* 

This file is part of form-shifter.

form-shifter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

form-shifter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with form-shifter.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdlib.h>
#include "types.h"

Coordinate* get_coordinate(int x, int y, Coordinate *next){
  Coordinate *point = (Coordinate*) malloc(sizeof(Coordinate));
  point->x = x; 
  (*point).y = y;
  (*point).next = next;
  return point;
}

FilledPolygonList* get_polygon_node(FilledPolygon *polygon, FilledPolygonList *next){
  FilledPolygonList *node = (FilledPolygonList*) malloc(sizeof(FilledPolygonList));
  node->polygon = polygon;
  node->next = next;
  return node;
}

/* Default polygon, a house */
FilledPolygonList* polygons_get_house(){
  /* returns a composed polygon of this form
     /\
    /  \
    ----
    |--|
    |  |
    |__|
   */

  /* Roof polygon */ 
  FilledPolygon *house_roof = (FilledPolygon*) malloc(sizeof(FilledPolygon));

  Coordinate *roof_top = get_coordinate(200,50,NULL);
  Coordinate *roof_bottom_right = get_coordinate(350,100,roof_top);
  Coordinate *roof_bottom_left = get_coordinate(50,100,roof_bottom_right);

  house_roof->npoints = 3;
  house_roof->points = roof_bottom_left;
  //house_roof->color = NULL; //FIX ME!    
  
  /* Body polygon */
  FilledPolygon *house_body = (FilledPolygon*) malloc(sizeof(FilledPolygon));

  Coordinate *body_top_right = get_coordinate(350,100,NULL);
  Coordinate *body_bottom_right = get_coordinate(350,300,body_top_right); 
  Coordinate *body_bottom_left = get_coordinate(50,300,body_bottom_right); 
  Coordinate *body_top_left = get_coordinate(50,100,body_bottom_left); 

  house_body->points = body_top_left;
  house_body->npoints = 4;

  /* build list of polygons */
  FilledPolygonList *roof_node,*body_node;
  roof_node = get_polygon_node(house_roof,NULL);
  body_node = get_polygon_node(house_body,roof_node);

  return body_node;
}

/* Paints a polygon */ 
void polygons_paint_on_canvas(FilledPolygon *polygon, cairo_t *cr){
  if (polygon->npoints < 3) //coordinates needed to draw a polygon
    return;
    
  Coordinate *previous_point = polygon->points;
  Coordinate *next_point = previous_point->next;

  while (next_point != NULL){
    cairo_move_to(cr,previous_point->x,previous_point->y);
    cairo_line_to(cr,next_point->x,next_point->y);
    
    previous_point = next_point;
    next_point = previous_point->next;
  }
  
  //line between last and first
  cairo_move_to(cr,previous_point->x,previous_point->y);
  cairo_line_to(cr,polygon->points->x,polygon->points->y);
  
  cairo_stroke(cr);
}

/* Paints a list of polygons */
void polygons_list_paint_on_canvas(FilledPolygonList *polygon_list, cairo_t *cr){
  FilledPolygon *next_polygon = polygon_list->polygon;
  FilledPolygonList *next_polygon_node = polygon_list->next;

  polygons_paint_on_canvas(next_polygon,cr);
  
  while (next_polygon_node != NULL){
    polygons_paint_on_canvas(next_polygon_node->polygon,cr);
    next_polygon_node = next_polygon_node->next;    
  }
}
