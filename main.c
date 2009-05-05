#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "tab1.h"
#include "tab2.h"
#include <sqlite3.h>

sqlite3 *db;

static void menuitem_response (gchar *);

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *notebook;
  GtkWidget *vbox;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *menu_f, *menu_h, *menu_bar, *file_menu, *help_menu, *menu_items_f, *menu_items_h;
  GtkWidget *scr_window;
  
  char *nameLabel[2] = { "Вкладка 1", "Вкладка 2"};
  char buf[512]; // скока надо точно,я не знаю
  int i=0;
  int rc;
  char *label_f[3] = {"сохранить","сохранить как...","выход"};
  char *label_h[1] = {"о программе"};
//sqlite addon:
        rc = sqlite3_open("123.db", &db);

         if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
        }
//-------
    
  gtk_init(&argc, &argv);

  //initiating window, hbox and tabs
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 1000, 400);
//    scr_window = gtk_scrolled_window_new (NULL, NULL);
//    gtk_container_add (GTK_CONTAINER(window), scr_window);
//    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scr_window),
//                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
//    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scr_window),
  //                        GTK_SHADOW_IN);

  vbox = gtk_vbox_new(FALSE, 0);//true, 10
  notebook = gtk_notebook_new();
  menu_f = gtk_menu_new();
  menu_h = gtk_menu_new();
  g_print("Привет!\n");

    for (i=0; i<3; i++)
    {
    sprintf(buf, label_f[i], i);
    menu_items_f = gtk_menu_item_new_with_label (buf);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_f), menu_items_f);

    g_signal_connect_swapped (G_OBJECT (menu_items_f), "activate",
                              G_CALLBACK (menuitem_response),  (gpointer) g_strdup (buf));
    gtk_widget_show (menu_items_f);
    }

    for (i=0; i<1; i++)
    {
    sprintf(buf, label_h[i], i);
    menu_items_h = gtk_menu_item_new_with_label (buf);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_h), menu_items_h);

    g_signal_connect_swapped (G_OBJECT (menu_items_h), "activate",
                              G_CALLBACK (menuitem_response),  (gpointer) g_strdup (buf));
    gtk_widget_show (menu_items_h);
    }

    file_menu = gtk_menu_item_new_with_label ("Файл");
    help_menu = gtk_menu_item_new_with_label ("Справка");
    gtk_widget_show (file_menu);
    gtk_widget_show (help_menu);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_menu), menu_f);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (help_menu), menu_h);
    menu_bar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (vbox), menu_bar, FALSE, FALSE, 0);//true false
    gtk_widget_show (menu_bar);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), file_menu);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), help_menu);

/*******************************************************************/
  for(i=0; i<2; i++)  {
    label = gtk_label_new(nameLabel[i]);
    scr_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scr_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    //Твой таб.
    if (i == 0)
        table = tab1();
    //Мой таб.
    else if (i == 1)
        table = tab2();

  gtk_scrolled_window_add_with_viewport (
                     GTK_SCROLLED_WINDOW (scr_window), table);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scr_window, label);
  }
 
    g_signal_connect_swapped(G_OBJECT(window), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

  //let's show them all
  gtk_container_add(GTK_CONTAINER(vbox), notebook);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);//vbox to win
  gtk_widget_show_all(window);
  
  //let's run main loop
  gtk_main(); 

  return 0;
}

static void menuitem_response( gchar *string )
{
            printf ("%s\n", string);
}
