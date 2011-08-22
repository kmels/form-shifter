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
#include "types.h"

GtkWidget *main_window, *canvas, *right_toolbar,*bottom_toolbar,*vbox;

int main( int argc,
          char *argv[] )
{
  GtkWidget *main_hbox;
  
  gtk_init (&argc, &argv);
  
  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect (GTK_OBJECT (main_window), "destroy", GTK_SIGNAL_FUNC (gtk_main_quit), "WM destroy");
  gtk_window_set_title (GTK_WINDOW(main_window), "Form Shifter");  
  
  main_hbox = gtk_hbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (main_hbox), 1);
  gtk_container_add (GTK_CONTAINER (main_window), main_hbox);
  gtk_widget_show (main_hbox);
  
  vbox = gtk_vbox_new (FALSE, 1);
  /* canvas  */
  widgets_get_canvas(&canvas);
  g_signal_connect(canvas, "expose-event", G_CALLBACK(canvas_redraw), NULL);
  gtk_box_pack_start (GTK_BOX (vbox), canvas, FALSE, TRUE, 0);
  
  gtk_widget_add_events(canvas, GDK_ALL_EVENTS_MASK);
  g_signal_connect(canvas, "button-press-event", G_CALLBACK(canvas_handle_mouse), GINT_TO_POINTER(MOUSE_CLICK));
  g_signal_connect(canvas, "button-release-event", G_CALLBACK(canvas_handle_mouse), GINT_TO_POINTER(MOUSE_CLICK));
  g_signal_connect(canvas, "motion-notify-event",G_CALLBACK(canvas_handle_mouse), GINT_TO_POINTER(MOUSE_MOTION));

  /* bottom toolbar */
  widgets_get_bottom_toolbar(&bottom_toolbar);
  gtk_box_pack_start (GTK_BOX (vbox), bottom_toolbar, FALSE, TRUE, 0);
  
  gtk_box_pack_start (GTK_BOX (main_hbox), vbox, FALSE, TRUE, 0);

  /* right toolbar */
  widgets_get_right_toolbar(&right_toolbar);
  gtk_box_pack_start (GTK_BOX (main_hbox), right_toolbar, FALSE, TRUE, 0);

  gtk_widget_show_all (main_window);
  
  gtk_main ();
      
  return(0);
}
