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
#include "widgets.h"

GtkWidget *scale_x_input,*scale_y_input;

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

/* Paints polygon's points */ 
void polygons_paint_points_on_canvas(FilledPolygon *polygon, cairo_t *cr){
  if (polygon->npoints < 3) //coordinates needed to draw a polygon
    return;
    
  Coordinate *next_point = polygon->points;

  while (next_point != NULL){
    cairo_rectangle(cr,next_point->x-3,next_point->y-3,6,6);    
    next_point = next_point->next;
  } 
  
  cairo_fill(cr);
}

/* Paints selected points of polygons */
void polygons_list_paint_selected_points_on_canvas(FilledPolygonList *selected_polygons,cairo_t *cr){
  FilledPolygon *next_polygon = selected_polygons->polygon;
  FilledPolygonList *next_polygon_node = selected_polygons->next;

  polygons_paint_points_on_canvas(next_polygon,cr);
  
  while (next_polygon_node != NULL){
    polygons_paint_points_on_canvas(next_polygon_node->polygon,cr);
    next_polygon_node = next_polygon_node->next;    
  }
}

/* Get polygon width */
FilledPolygonDimensions get_polygon_dimensions(FilledPolygon *polygon){
  FilledPolygonDimensions dimensions;  
  int smallest_x = -1;
  int largest_x = -1;
  int smallest_y = -1;
  int largest_y = -1;

  Coordinate *current_point = polygon->points;

  while (current_point != NULL){      
    if (current_point->x < smallest_x || smallest_x<0)
      smallest_x = current_point->x;
    
    if (current_point->x > largest_x || largest_x<0)
      largest_x = current_point->x;
    
    if (current_point->y < smallest_y || smallest_y<0)
	smallest_y = current_point->x;
    
    if (current_point->y > largest_y || largest_y<0)
      largest_y = current_point->y;
    
    current_point = current_point->next;      
  }

  dimensions.width = largest_x - smallest_x;
  dimensions.height = largest_y - smallest_y;
  //  printf("x: %d,%d, y: %d,%d\n",smallest_x,largest_x,smallest_y,largest_y);
  return dimensions;
}

/* Scales selected polygons */
void  polygons_scale_selected (GtkButton *button, gpointer user_data){
  int scale_x = atoi(gtk_entry_get_text(GTK_ENTRY(scale_x_input)));
  int scale_y = atoi(gtk_entry_get_text(GTK_ENTRY(scale_y_input)));
  
  printf("%d,%d \n",scale_x,scale_y);
}
