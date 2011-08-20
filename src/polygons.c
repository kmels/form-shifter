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
#include "global.h"
#include "canvas.h"

GtkWidget *scale_x_input,*scale_y_input;

Color get_color(guint16 red, guint16 green, guint16 blue, guint16 alpha){
  Color color;
  color.red = red; color.green = green; color.blue = blue; color.alpha = alpha;
  return color;
}

Coordinate* get_coordinate(double x, double y, Coordinate *next){
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

/* Compares two polygons, return true iff they contain they've the same coordinates */
gboolean polygons_are_equal(FilledPolygon *polygon1,FilledPolygon *polygon2){
  Coordinate *comparing_point1 = polygon1->points;
  Coordinate *comparing_point2 = polygon2->points;
  
  while(comparing_point1 != NULL && comparing_point2 != NULL){
    if (comparing_point1->x != comparing_point2->x || comparing_point1->y != comparing_point2->y)
      return FALSE;

      comparing_point1 = comparing_point1->next;
      comparing_point2 = comparing_point2->next;
  }  
  
  return TRUE;
}

/* Checks if a polygon is contained in a polygon list */
gboolean polygons_list_contains_polygon(FilledPolygonList *polygons_list,FilledPolygon *polygon){   
  FilledPolygonList *current_polygon_node = polygons_list;

  while (current_polygon_node != NULL){
    if (polygons_are_equal(current_polygon_node->polygon,polygon))
      return TRUE;
    
    current_polygon_node = current_polygon_node->next;
  }

  return FALSE;
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
  house_roof->color = get_color(0,0,255,255);

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
  house_body->color = get_color(255,0,0,255);

  /* build list of polygons */
  FilledPolygonList *roof_node,*body_node;
  body_node = get_polygon_node(house_body,NULL);
  roof_node = get_polygon_node(house_roof,body_node);
  
  return roof_node;
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

  //now fill
  double x,y;
  Color color = polygon->color;

  cairo_set_source_rgba(cr,(double)color.red / 255, (double)color.green / 255, (double)color.blue / 255,(double)color.alpha/255);
  for (x=0; x < canvas->allocation.width; x++){
    for (y=0; y < canvas->allocation.height; y++){
      if (canvas_coordinate_is_within_polygon(x,y,polygon)){
	cairo_move_to(cr,x,y);
	cairo_rectangle(cr,x,y,1,1);
	cairo_fill(cr);
	}
    }
  }

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
  
  int i; 
  for (i = 0; i < polygon->npoints ; i++){
    cairo_rectangle(cr,next_point->x-3,next_point->y-3,6,6);    
    next_point = next_point->next;
  }  
  
  cairo_fill(cr);
}

/* Paints selected points of polygons */
void polygons_list_paint_selected_points_on_canvas(FilledPolygonList *selected_polygons,cairo_t *cr){
  if (selected_polygons==NULL){ //if there are not any selected polygons
    return;
  }  
  
  cairo_set_source_rgba(cr,0,0,0,1);
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
  double smallest_x = -1;
  double largest_x = -1;
  double smallest_y = -1;
  double largest_y = -1;

  Coordinate *current_point = polygon->points;
  printf("GET POLYGON DIMENSIONS DE: \n");
  print_polygon(polygon);

  while (current_point != NULL){      
    if (current_point->x < smallest_x || smallest_x<0)
      smallest_x = current_point->x;
    
    if (current_point->x > largest_x || largest_x<0)
      largest_x = current_point->x;
    
    if (current_point->y < smallest_y || smallest_y<0)
	smallest_y = current_point->y;
    
    if (current_point->y > largest_y || largest_y<0)
      largest_y = current_point->y;
    
    current_point = current_point->next;      
  }
  printf("RESULTS: %f, %f .. %f, %f\n",smallest_x,largest_x,smallest_y,largest_y);
  dimensions.width = largest_x - smallest_x;
  dimensions.height = largest_y - smallest_y;
  return dimensions;
}

/* Duplicates a polygon*/
FilledPolygon *polygon_duplicate(FilledPolygon *polygon){
  int i; 
  FilledPolygon *duplicated_polygon = (FilledPolygon*) malloc(sizeof(FilledPolygon));
  duplicated_polygon->color = polygon->color;
  duplicated_polygon->npoints = polygon->npoints;
  
  //source current point
  Coordinate *polygon_source_current = polygon->points;

  //build first polygon target point
  Coordinate *polygon_target_first_point = get_coordinate(polygon_source_current->x,polygon_source_current->y,NULL); //create duplicate for first point
  duplicated_polygon->points = polygon_target_first_point; //assign the first point to duplicate
    
  Coordinate *polygon_target_current_point = duplicated_polygon->points;
  polygon_source_current = polygon_source_current->next;
  
  for (i =0; i < polygon->npoints-1; i++){ //until npoints -1, we already copied the first
    Coordinate *new_point = get_coordinate(polygon_source_current->x,polygon_source_current->y,NULL);
    polygon_target_current_point->next = new_point;
    //update pointers
    polygon_source_current = polygon_source_current->next;
    polygon_target_current_point = polygon_target_current_point->next;
  }
  
  return duplicated_polygon;
}

/* Scales a polygon, returns a new one*/
FilledPolygon *polygon_scale(FilledPolygon *source_polygon, double factor_x, double factor_y){
  if (source_polygon->npoints < 3) //coordinates needed to draw a polygon
    return NULL;
  
  FilledPolygon *polygon = polygon_duplicate(source_polygon);
  
  Coordinate *current_point = polygon->points;
  
  while (current_point != NULL){
    current_point->x = (int) current_point->x * factor_x;
    current_point->y = (int) current_point->y * factor_y;    
    
    current_point = current_point->next;
  }
  
  return polygon;
}

/* Scales selected polygons */
void  polygons_scale_selected (GtkButton *button, gpointer user_data){
  int scale_x_target = atoi(gtk_entry_get_text(GTK_ENTRY(scale_x_input)));
  int scale_y_target = atoi(gtk_entry_get_text(GTK_ENTRY(scale_y_input)));
    
  //scaling factors
  double factor_x,factor_y;

  /* iterate each selected polygon, and scale */
  FilledPolygonList *current_scaling_polygon_node = selected_polygons;

  //printf ("selected first point: %d,%d\n",selected_polygons->polygon->points->x,selected_polygons->polygon->points->y);

  while (current_scaling_polygon_node != NULL){    
    FilledPolygon *polygon_source = current_scaling_polygon_node->polygon;

    //FIXME pixels or percentage?
    FilledPolygonDimensions dimensions = get_polygon_dimensions(polygon_source);
        
    //if pixels
    factor_x = scale_x_target/dimensions.width;
    factor_y = scale_y_target/dimensions.height;
    
    //printf("source antes: %p, first: %d,%d ",polygon_source,polygon_source->points->x,polygon_source->points->y);
    //find it within all polygons, and reset
    canvas_all_polygons_replace_existing_polygon(current_scaling_polygon_node->polygon,polygon_scale(polygon_source,factor_x,factor_y));
    
    current_scaling_polygon_node->polygon = polygon_scale(polygon_source,factor_x,factor_y);
    //
    //printf("source despues: %p, first: %d,%d ",polygon_source,polygon_source->points->x,polygon_source->points->y);
    
    current_scaling_polygon_node = current_scaling_polygon_node->next;
    //printf("end scaling \n\n\n\n\n");
  }
  
  //printf ("selected first point: %d,%d\n",selected_polygons->polygon->points->x,selected_polygons->polygon->points->y);

  canvas_repaint();
}

void polygon_get_coordinates_array(Coordinate array[],FilledPolygon *polygon){
  //Coordinate array[polygon->npoints];

  Coordinate *current_point = polygon->points;
  int current_index = 0;

  while (current_point != NULL){
    array[current_index].x = current_point->x;
    array[current_index].y = current_point->y;
    current_point = current_point->next;      
    
    current_index++;
  }
  
}
