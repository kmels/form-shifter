#ifndef CANVAS
#define CANVAS

void canvas_paint_last_state();
gboolean canvas_redraw(GtkWidget *widget, gpointer data);
gboolean canvas_handle_mouse(GtkWidget *widget, void *e, gpointer *t);
#endif
