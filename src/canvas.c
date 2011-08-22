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
  cairo_t *cr = gdk_cairo_create(canvas->window);
  FilledPolygon *house = polygons_get_house();
  polygons_paint_on_canvas(house,cr);
  all_polygons = get_polygon_node(house,NULL);
  
  /* update input values */
  widgets_update_input_values();
}

/* Draws all the polygons according to their last state */
void canvas_repaint(){
  cairo_t *cr = gdk_cairo_create(canvas->window);  
  
  //paint white canvas
  cairo_set_source_rgba(cr,1,1,1,1); 
  cairo_paint(cr);

  polygons_list_paint_on_canvas(all_polygons,cr);
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
    selected_polygons = (FilledPolygonList*) malloc(sizeof(FilledPolygonList));
    selected_polygons->polygon = polygon;
    selected_polygons->next = NULL;    

    return;
  }

  //then, there's already a selected one
  FilledPolygonList *selected_polygon_node = selected_polygons;
  //printf("%p == %p ??\n");
  
  while (selected_polygon_node->next != NULL){
    selected_polygon_node = selected_polygon_node->next;    
  }
  
  //add to the last one
  selected_polygon_node->next = (FilledPolygonList*) malloc(sizeof(FilledPolygonList));
  selected_polygon_node->next->polygon = polygon;
  selected_polygon_node->next->next = NULL;

  FilledPolygonList *dummie = selected_polygons;

  while (dummie != NULL){
    dummie = dummie->next;    
  }
  return;  
}

/* Given a coordinate, adds a polygon to selected_polygons list if polygon is within coordinates */
void canvas_select_polygon_from_coordinates(int x,int y){
  FilledPolygonList *next_polygon_node = all_polygons;
  
  gboolean found_polygon = FALSE;

  while (next_polygon_node != NULL){
    
    if (canvas_coordinate_is_within_polygon(x,y,next_polygon_node->polygon)){ //found polygon in given coordinate      
      //mark polygon as selected
      found_polygon = TRUE;
      
      if (!polygons_list_contains_polygon(selected_polygons,next_polygon_node->polygon))
	canvas_add_polygon_to_selected_list(next_polygon_node->polygon);
      break;
    }

    next_polygon_node = next_polygon_node->next;    
  }

  if (!found_polygon){ //then, clear selected polygons
    selected_polygons = NULL;    
  }
  
  canvas_repaint();
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

/* Replaces given polygon in all_polygons list */
void canvas_all_polygons_replace_existing_polygon(FilledPolygon *polygon_to_replace, FilledPolygon *replaced_polygon){
  FilledPolygonList *current_polygon_node = all_polygons;

  while (current_polygon_node != NULL){
    if (polygons_are_equal(current_polygon_node->polygon,polygon_to_replace)){ //is this one the one we're replacing?
      current_polygon_node->polygon = replaced_polygon;
    }
    
    current_polygon_node = current_polygon_node->next;
  }
}
