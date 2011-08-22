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
#include <stdio.h>
#include "types.h"
#include "global.h"
#include "widgets.h"
#include "polygons.h"
#include <stdlib.h>
#include <strings.h>
#include <string.h>

/* from global */
GtkWidget *scale_x_input,*scale_y_input,*degrees_input,*shears_axis_input,*shears_input;

FormShifterToolItemType current_tool;

static FormShifterToolItem toolbar_item_icons[] = {
  { "icons/icon_select.png", "Select", FORM_SHIFTER_SELECT_TOOL },
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
  gtk_combo_box_set_active(GTK_COMBO_BOX(units_combobox),0);

  percentage_checkbutton = gtk_check_button_new_with_label("Keep proportions"); //keep proportions check button
  scale_button = gtk_button_new_with_label("OK");
  g_signal_connect(scale_button, "clicked", G_CALLBACK(polygons_scale_selected), NULL);   

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
  
  GtkWidget *degrees_hbox,*degrees_label,*degrees_button,*animation_time_hbox,*animation_time_label,*animation_time_input;

  /* create widgets */
  degrees_hbox = gtk_hbox_new (FALSE, 1);  
  degrees_label = gtk_label_new("Degrees: ");
  degrees_input = gtk_spin_button_new_with_range(1,360,1); //from 1 to 360 by 1
  
  animation_time_hbox = gtk_hbox_new (FALSE, 1);
  animation_time_label = gtk_label_new("Animation time (seconds): ");
  animation_time_input = gtk_spin_button_new_with_range(1,60,1); //from 1 to 60 by 1

  degrees_button = gtk_button_new_with_label("OK");
  g_signal_connect(degrees_button, "clicked", G_CALLBACK(polygons_rotate_selected), NULL);   

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

void get_shears_tab(GtkWidget **container, GtkWidget **label){
  *container = gtk_vbox_new(FALSE,1);
  *label = gtk_label_new("Shears");
  
  GtkWidget *shears_axis_label,*shears_axis_hbox,*shears_input_label,*shears_input_hbox,*shears_button;

  /* create widgets */
  shears_axis_hbox = gtk_hbox_new (FALSE, 1);
  shears_axis_label = gtk_label_new("Axis: ");
  shears_axis_input = gtk_combo_box_text_new();
  gtk_combo_box_append_text(GTK_COMBO_BOX(shears_axis_input),"x");
  gtk_combo_box_append_text(GTK_COMBO_BOX(shears_axis_input),"y");
  gtk_combo_box_set_active(GTK_COMBO_BOX(shears_axis_input),0);
  
  shears_input_hbox = gtk_hbox_new (FALSE, 1);
  shears_input_label = gtk_label_new("Pixels: ");
  shears_input = gtk_spin_button_new_with_range(-9999,9999,0.01); //from -9999 to 9999 by 0.01
  gtk_spin_button_set_value (GTK_SPIN_BUTTON(shears_input),1);

  shears_button = gtk_button_new_with_label("OK");
  //g_signal_connect(degrees_button, "clicked", G_CALLBACK(polygons_rotate_selected), NULL);   

  //axis pack
  gtk_box_pack_start (GTK_BOX (shears_axis_hbox), shears_axis_label, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (shears_axis_hbox), shears_axis_input, FALSE, TRUE, 0);

  //input pack
  gtk_box_pack_start (GTK_BOX (shears_input_hbox), shears_input_label, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (shears_input_hbox), shears_input , FALSE, TRUE, 0);
  
  //pack boxes and button
  gtk_box_pack_start (GTK_BOX (*container), shears_axis_hbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), shears_input_hbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (*container), shears_button, FALSE, TRUE, 0);
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
  
  //shears
  GtkWidget *shears_tab,*shears_label;
  get_shears_tab(&shears_tab,&shears_label);
  gtk_notebook_append_page(GTK_NOTEBOOK(tabs),shears_tab,shears_label);

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
  FilledPolygonList *current_polygon_node = selected_polygons;
  int width = -1;
  int height = -1;

  FilledPolygonDimensions current_polygon_dimensions;

  //iterate over polygons
  while (current_polygon_node != NULL){    
    current_polygon_dimensions = get_polygon_dimensions(current_polygon_node->polygon);
    
    if (current_polygon_dimensions.width > width)
      width = current_polygon_dimensions.width;
    
    if (current_polygon_dimensions.height > height)
      height = current_polygon_dimensions.height;

    current_polygon_node = current_polygon_node->next;
  }
  
  
  char *s = NULL;  

  /* set scale input values */
  asprintf (&s, "%d", width);
  gtk_entry_set_text(GTK_ENTRY(scale_x_input),s);

  asprintf (&s, "%d", height);
  gtk_entry_set_text(GTK_ENTRY(scale_y_input),s);
}
