#ifndef WIDGETS
#define WIDGETS

extern GtkWidget *main_window;
extern GtkWidget *canvas;
extern GtkWidget *toolbar;

extern GtkWidget *scale_x_input,*scale_y_input,*degrees_input,*shears_axis_input,*shears_input;

void widgets_get_canvas(GtkWidget **canvas);
void widgets_get_right_toolbar(GtkWidget **toolbar);
void widgets_get_bottom_toolbar(GtkWidget **toolbar);
void widgets_update_input_values();
#endif
