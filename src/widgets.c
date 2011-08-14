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

/* adds the toolbar to a container */
void widgets_get_canvas(GtkWidget **canvas){
  *canvas = gtk_drawing_area_new();
  gtk_widget_set_size_request(*canvas, 500, 500);
}

void widgets_get_toolbar(GtkWidget **toolbar){
  //GtkWidget
  
}
