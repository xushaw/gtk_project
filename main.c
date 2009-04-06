#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "tab1.h"
#include "tab2.h"
#include <sqlite3.h>

sqlite3 *db;

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *notebook;
  GtkWidget *vbox;
  GtkWidget *table;
  GtkWidget *label;
  
  char *nameLabel[2] = { "Вкладка 1", "Вкладка 2"};
  int i=0;
  int rc;
//sqlite addon:
        rc = sqlite3_open("base.db", &db);

        if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
        }
//-------
    
  gtk_init(&argc, &argv);

  //initiating window, hbox and tabs
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  vbox = gtk_vbox_new(TRUE, 10);
  notebook = gtk_notebook_new();


  for(i=0; i<2; i++)  {
    label = gtk_label_new(nameLabel[i]);
    //Твой таб.
    if (i == 0)
        table = tab1();
    //Мой таб.
    else if (i == 1)
        table = tab2();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table, label);

  }
 
  g_signal_connect_swapped(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  //let's show them all
  gtk_container_add(GTK_CONTAINER(vbox), notebook);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
  gtk_widget_show_all(window);
  
  //let's run main loop
  gtk_main(); 

  return 0;
}
