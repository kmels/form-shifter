#include <gtk/gtk.h>

GtkWidget *window;

int main( int argc,
          char *argv[] )
{
  GtkWidget *main_vbox;
  //GtkWidget *menubar;
  //GtkWidget *toolbar;

  gtk_init (&argc, &argv);
  
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect (GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (gtk_main_quit), "WM destroy");
  gtk_window_set_title (GTK_WINDOW(window), "Form Shifter");
  gtk_widget_set_usize (GTK_WIDGET(window), 500, 700);
  
  main_vbox = gtk_vbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (main_vbox), 1);
  gtk_container_add (GTK_CONTAINER (window), main_vbox);
  gtk_widget_show (main_vbox);
  
  /* top menu bar */
  //get_main_menu (window, &menubar);
  //gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, TRUE, 0);
  
  /* toolbar */
  //get_toolbar(window, &toolbar);
  
  gtk_widget_show_all (window);

  gtk_main ();
      
  return(0);
}
