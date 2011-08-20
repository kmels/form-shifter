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

#include <gtk/gtk.h>
#include <stdlib.h>
#include "types.h"
#include "global.h"
#include "polygons.h"
#include "widgets.h"

gboolean canvas_drawn;
GtkWidget *canvas;
FilledPolygonList *selected_polygons;
FilledPolygonList *all_polygons;

/* Draws canvas initial state i.e. with 2 polygons forming a house */
void canvas_paint_initial_state(){
  printf("paint initial state\n");
  cairo_t *cr = gdk_cairo_create(canvas->window);
  FilledPolygonList *house = polygons_get_house();
  polygons_list_paint_on_canvas(house,cr);
  all_polygons = house;
  //polygons_list_paint_selected_points_on_canvas(all_polygons,cr);

  /* update input values */
  widgets_update_input_values();
}

/* Draws all the polygons according to their last state */
void canvas_repaint(){
  cairo_t *cr = gdk_cairo_create(canvas->window);  
  
  polygons_list_paint_on_canvas(selected_polygons,cr);
  polygons_list_paint_selected_points_on_canvas(selected_polygons,cr);

  /* update input values */
  widgets_update_input_values();
}

/* called on canvas expose event */
gboolean canvas_redraw(GtkWidget *widget, gpointer data){
  if (!canvas_drawn){
    canvas_drawn = TRUE;
    
    return FALSE;
  } else{
    
    //if (canvas_history.current_index < 0){ //then we have no surface on our history, paint a white canvas as an initial one
      
    canvas_paint_initial_state(); //draw the last state of the polygon (either rotated, translated, etc)
    
    //}
    
    return TRUE;
  }
}

inline gboolean canvas_coordinate_is_within_polygon(double x, double y,FilledPolygon *polygon) {  
  int      i, j = polygon->npoints-1;
  gboolean  oddNodes=FALSE;
    
  Coordinate coordinates[polygon->npoints];

  polygon_get_coordinates_array(coordinates,polygon);

  for (i=0; i < polygon->npoints; i++) {
    if ((((coordinates[i].y < y) && (coordinates[j].y >=y ))
	 || ((coordinates[j].y < y) && (coordinates[i].y>=y)))
	&&  ((coordinates[i].x <=x) || (coordinates[j].x <=x))) {
      if (coordinates[i].x+(y-coordinates[i].y)/(coordinates[j].y-coordinates[i].y)*(coordinates[j].x-coordinates[i].x)<x) {
        oddNodes=!oddNodes;
      }
    }

    j=i;
  }
  
  return oddNodes;
}

/* Adds a polygon to the selected polygons list */
void canvas_add_polygon_to_selected_list(FilledPolygon *polygon){    
  if (selected_polygons==NULL){
    printf("NIL\n");
    selected_polygons = (FilledPolygonList*) malloc(sizeof(FilledPolygonList));
    selected_polygons->polygon = polygon;
    selected_polygons->next = NULL;
    polygons_paint_points_on_canvas(polygon, gdk_cairo_create(canvas->window));
    return;
  }

  //then, there's already a selected one
  FilledPolygonList *selected_polygon_node = selected_polygons;

  while (selected_polygon_node->next != NULL){
    selected_polygon_node = selected_polygon_node->next;    
  }
  
  //add to the last one
  selected_polygon_node->next = (FilledPolygonList*) malloc(sizeof(FilledPolygonList));
  selected_polygon_node->polygon = polygon;
  selected_polygon_node->next->next = NULL;
  polygons_paint_points_on_canvas(polygon, gdk_cairo_create(canvas->window));
  return;  
}

/* Given a coordinate, adds a polygon to selected_polygons list if polygon is within coordinates */
void canvas_select_polygon_from_coordinates(int x,int y){
  FilledPolygonList *next_polygon_node = all_polygons;
  
  gboolean found_polygon = FALSE;

  while (next_polygon_node != NULL){
    
    if (canvas_coordinate_is_within_polygon(x,y,next_polygon_node->polygon)){
      //mark polygon as selected
      found_polygon = TRUE;
      canvas_add_polygon_to_selected_list(next_polygon_node->polygon);      
      break;
    }

    next_polygon_node = next_polygon_node->next;    
  }

  if (!found_polygon){ //then, clear selected polygons
    printf("CLEARING!\n");
    selected_polygons = NULL;
    //canvas_repaint();
  }
}

gboolean canvas_handle_mouse(GtkWidget *widget, void *e, gpointer *t){
  gint type = GPOINTER_TO_INT(t);

  switch(type){
  case MOUSE_CLICK: {
    GdkEventButton *event = (GdkEventButton*) e;
    
    switch(event->type){
    case GDK_BUTTON_PRESS:{
      switch(current_tool){      
      case FORM_SHIFTER_SELECT_TOOL: {
	
	if (event->button==1){ //left click
	  canvas_select_polygon_from_coordinates(event->x,event->y);
	}
      } break;
      default: break;
      }
    }break;
    case GDK_BUTTON_RELEASE:{
    }break;
    default: break;
    }
  }break;

    
  case MOUSE_MOTION: {
    
  }
  }
  return TRUE;
}
