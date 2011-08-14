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
#include "canvas.h"
#include "widgets.h"

GtkWidget *main_window;
GtkWidget *canvas;
GtkWidget *toolbar;

int main( int argc,
          char *argv[] )
{
  GtkWidget *main_vbox;
  

  gtk_init (&argc, &argv);
  
  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect (GTK_OBJECT (main_window), "destroy", GTK_SIGNAL_FUNC (gtk_main_quit), "WM destroy");
  gtk_window_set_title (GTK_WINDOW(main_window), "Form Shifter");
  gtk_widget_set_usize (GTK_WIDGET(main_window), 500, 700);
  
  main_vbox = gtk_vbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (main_vbox), 1);
  gtk_container_add (GTK_CONTAINER (main_window), main_vbox);
  gtk_widget_show (main_vbox);
  
  /* top menu bar */
  widgets_get_canvas(&canvas);
  gtk_box_pack_start (GTK_BOX (main_vbox), canvas, FALSE, TRUE, 0);
  g_signal_connect(canvas, "expose-event", G_CALLBACK(canvas_redraw), NULL);
  
  /* toolbar */
  //widgets_get_toolbar(&toolbar);
  //gtk_box_pack_start (GTK_BOX (main_vbox), toolbar, FALSE, TRUE, 0);

  gtk_widget_show_all (main_window);

  gtk_main ();
      
  return(0);
}
