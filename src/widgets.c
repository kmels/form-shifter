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
#include "types.h"
#include "global.h"
#include "widgets.h"
#include <stdio.h>

/* from global */
GtkWidget *scale_x_input,*scale_y_input;

FormShifterToolItemType current_tool;

static FormShifterToolItem toolbar_item_icons[] = {
  { "icons/icon_select.png", "Select", FORM_SHIFTER_MOUSE_TOOL },
  { "icons/icon_polygon.png", "Polygon", FORM_SHIFTER_POLYGON_TOOL }
};

/* adds the toolbar to a container */
void widgets_get_canvas(GtkWidget **canvas){
  *canvas = gtk_drawing_area_new();
  gtk_widget_set_size_request(*canvas, 700, 550);
}

void get_scale_tab(GtkWidget **container, GtkWidget **label){
  GtkWidget *x_label,*y_label,*percentage_checkbutton,*scale_button,*x_hbox,*y_hbox,*units_combobox;
  *label = gtk_label_new("Scale");
  *container = gtk_vbox_new (FALSE, 1);
    
  //gtk_frame_set_shadow_type (GTK_FRAME (*container), GTK_SHADOW_IN);    

  /* create widgets */
  x_hbox = gtk_hbox_new (FALSE, 1);  
  x_label = gtk_label_new("x: ");
  scale_x_input = gtk_spin_button_new_with_range(1,9999,1); //from 1 to 9999 by 1

  y_hbox = gtk_hbox_new (FALSE, 1);
  y_label = gtk_label_new("y: ");
  scale_y_input = gtk_spin_button_new_with_range(1,9999,1); //from 1 to 9999 by 1
      
  units_combobox = gtk_combo_box_text_new();
  gtk_combo_box_append_text(GTK_COMBO_BOX(units_combobox),"pixels");
  gtk_combo_box_append_text(GTK_COMBO_BOX(units_combobox),"percentage");

  percentage_checkbutton = gtk_check_button_new_with_label("Keep proportions"); //keep proportions check button
  scale_button = gtk_button_new_with_label("OK");
  
  //pack x
  gtk_box_pack_start (GTK_BOX (x_hbox), x_label, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (x_hbox), scale_x_input, FALSE, TRUE, 0);

  //pack y
  gtk_box_pack_start (GTK_BOX (y_hbox), y_label, FALSE, TRUE, 0);  
  gtk_box_pack_start (GTK_BOX (y_hbox), scale_y_input, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (y_hbox), units_combobox, FALSE, TRUE, 0); 

  //pack main scale box
  gtk_box_pack_start (GTK_BOX (*container), x_hbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), y_hbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), percentage_checkbutton, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), scale_button, FALSE, TRUE, 0);
  
  /* disable it, there are no points selected */
  //gtk_widget_set_sensitive(*container,FALSE);
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

void widgets_get_right_toolbar(GtkWidget **toolbar){
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

void assign_current_tool(GtkWidget *widget, gpointer data){  
  current_tool = (FormShifterToolItemType) data;
  gtk_widget_grab_focus(widget);
}

void widgets_get_bottom_toolbar(GtkWidget **toolbar){
  GtkToolItem *new_tool_item;
  GtkWidget *new_tool_item_icon;

  *toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(*toolbar), GTK_TOOLBAR_ICONS);
  
  int i;
  int nicons = sizeof toolbar_item_icons / sizeof (*toolbar_item_icons);
  for (i = 0; i < nicons; i++){
    new_tool_item_icon = gtk_image_new_from_file(toolbar_item_icons[i].path_to_image);    
    new_tool_item = gtk_tool_button_new(new_tool_item_icon,toolbar_item_icons[i].name);
    
    FormShifterToolItemType tool_type = toolbar_item_icons[i].type;
    switch(tool_type){
      /*case XPainter_UNDO_TOOL:
	g_signal_connect(new_tool_item, "clicked", G_CALLBACK(undo), NULL);   
	break;
	case XPainter_REDO_TOOL:
	g_signal_connect(new_tool_item, "clicked", G_CALLBACK(redo), NULL);   
	break;*/
      default: 
	g_signal_connect(new_tool_item, "clicked", G_CALLBACK(assign_current_tool), (gpointer) tool_type);
	break;
    }
    gtk_toolbar_insert(GTK_TOOLBAR(*toolbar), new_tool_item, -1);
    gtk_widget_set_can_focus ((GtkWidget*) new_tool_item, TRUE);
  }
}

void widgets_update_input_values(){  
  int smallest_x = -1;
  int largest_x = -1;
  int smallest_y = -1;
  int largest_y = -1;

  FilledPolygonList *current_polygon_node = selected_polygons;

  Coordinate *current_point;

  //iterate over polygons
  while (current_polygon_node != NULL){
    //iterate over points
    current_point = current_polygon_node->polygon->points;

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
    
    current_polygon_node = current_polygon_node->next;
  }

  int width = largest_x - smallest_x;
  int height = largest_y - smallest_y;
  
  char *s = NULL;  

  /* set scale input values */
  asprintf (&s, "%d", width);
  gtk_entry_set_text(GTK_ENTRY(scale_x_input),s);

  asprintf (&s, "%d", height);
  gtk_entry_set_text(GTK_ENTRY(scale_y_input),s);
}
