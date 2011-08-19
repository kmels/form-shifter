#ifndef CANVAS
#define CANVAS

void canvas_repaint_last_state();
void canvas_repaint();
gboolean canvas_redraw(GtkWidget *widget, gpointer data);
gboolean canvas_handle_mouse(GtkWidget *widget, void *e, gpointer *t);
#endif
