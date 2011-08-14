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
#include "polygons.h"

gboolean canvas_drawn;
GtkWidget *canvas;

/* Draws all the polygons according to their last state */
void canvas_repaint_last_state(){
  cairo_t *cr = gdk_cairo_create(canvas->window);
  FilledPolygonList *house = polygons_get_house();
  polygons_list_paint_on_canvas(house,cr);
}

/* called on canvas expose event */
gboolean canvas_redraw(GtkWidget *widget, gpointer data){
  if (!canvas_drawn){
    canvas_drawn = TRUE;
    
    return FALSE;
  } else{
    
    //if (canvas_history.current_index < 0){ //then we have no surface on our history, paint a white canvas as an initial one
      
    canvas_repaint_last_state(); //draw the last state of the polygon (either rotated, translated, etc)
    
    //}
    
    return TRUE;
  }
}
