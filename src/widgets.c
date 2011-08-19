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
  gtk_widget_set_size_request(*canvas, 700, 600);
}

void get_scale_tab(GtkWidget **container, GtkWidget **label){
  GtkWidget *x_label,*y_label,*x_input,*y_input,*percentage_checkbutton,*scale_button,*x_hbox,*y_hbox,*units_combobox;
  *label = gtk_label_new("Scale");
  *container = gtk_vbox_new (FALSE, 1);
    
  //gtk_frame_set_shadow_type (GTK_FRAME (*container), GTK_SHADOW_IN);    

  /* create widgets */
  x_hbox = gtk_hbox_new (FALSE, 1);  
  x_label = gtk_label_new("x: ");
  x_input = gtk_spin_button_new_with_range(1,9999,1); //from 1 to 9999 by 1

  y_hbox = gtk_hbox_new (FALSE, 1);
  y_label = gtk_label_new("y: ");
  y_input = gtk_spin_button_new_with_range(1,9999,1); //from 1 to 9999 by 1
      
  units_combobox = gtk_combo_box_text_new();
  gtk_combo_box_append_text(GTK_COMBO_BOX(units_combobox),"pixels");
  gtk_combo_box_append_text(GTK_COMBO_BOX(units_combobox),"percentage");

  percentage_checkbutton = gtk_check_button_new_with_label("Keep proportions"); //keep proportions check button
  scale_button = gtk_button_new_with_label("OK");
  
  //pack x
  gtk_box_pack_start (GTK_BOX (x_hbox), x_label, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (x_hbox), x_input, FALSE, TRUE, 0);

  //pack y
  gtk_box_pack_start (GTK_BOX (y_hbox), y_label, FALSE, TRUE, 0);  
  gtk_box_pack_start (GTK_BOX (y_hbox), y_input, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (y_hbox), units_combobox, FALSE, TRUE, 0); 

  //pack main scale box
  gtk_box_pack_start (GTK_BOX (*container), x_hbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), y_hbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), percentage_checkbutton, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), scale_button, FALSE, TRUE, 0);
}

void get_rotate_tab(GtkWidget **container, GtkWidget **label){
  *container = gtk_vbox_new(FALSE,1);
  *label = gtk_label_new("Rotate");
  
  GtkWidget *degrees_hbox,*degrees_label,*degrees_input,*degrees_button,*animation_time_hbox,*animation_time_label,*animation_time_input;

  /* create widgets */
  degrees_hbox = gtk_hbox_new (FALSE, 1);  
  degrees_label = gtk_label_new("Degrees: ");
  degrees_input = gtk_spin_button_new_with_range(1,360,1); //from 1 to 360 by 1
  
  animation_time_hbox = gtk_hbox_new (FALSE, 1);
  animation_time_label = gtk_label_new("Animation time (seconds): ");
  animation_time_input = gtk_spin_button_new_with_range(1,60,1); //from 1 to 60 by 1

  degrees_button = gtk_button_new_with_label("OK");
  
  //degrees pack
  gtk_box_pack_start (GTK_BOX (degrees_hbox), degrees_label, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (degrees_hbox), degrees_input, FALSE, TRUE, 0);
  //animation time pack
  gtk_box_pack_start (GTK_BOX (animation_time_hbox), animation_time_label, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (animation_time_hbox), animation_time_input, FALSE, TRUE, 0);
  
  gtk_box_pack_start (GTK_BOX (*container), degrees_hbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), animation_time_hbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), degrees_button, FALSE, TRUE, 0);
}

void widgets_get_toolbar(GtkWidget **toolbar){
  *toolbar = gtk_vbox_new(FALSE,1);

  GtkWidget *tabs = gtk_notebook_new();
  gtk_widget_set_size_request(tabs, 300, 600);

  //scale
  GtkWidget *scale_tab,*scale_label;
  get_scale_tab(&scale_tab,&scale_label);
  gtk_notebook_append_page(GTK_NOTEBOOK(tabs),scale_tab,scale_label);
  
  //rotate
  GtkWidget *rotate_tab,*rotate_label;
  get_rotate_tab(&rotate_tab,&rotate_label);
  gtk_notebook_append_page(GTK_NOTEBOOK(tabs),rotate_tab,rotate_label);
  
  gtk_box_pack_start(GTK_BOX(*toolbar), tabs, FALSE, TRUE, 0);
}
