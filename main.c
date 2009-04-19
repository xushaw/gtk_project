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
  GtkWidget *menu_f, *menu_e, *menu_h, *menu_bar, *file_menu, *edit_menu, *help_menu, *menu_items_f, *menu_items_e, *menu_items_h;
  
  char *nameLabel[2] = { "Вкладка 1", "Вкладка 2"};
  char buf[512]; // скока надо точно,я не знаю
  int i=0;
  int rc;
  char *label_f[4] = {"сохранить","сохранить как...","печать","выход"};
  char *label_e[6] = {"отменить","вернуть","вырезать","копировать", "вставить", "удалить"};
  char *label_h[3] = {"1", "2", "3"};
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
  vbox = gtk_vbox_new(FALSE, 0);//true, 10
  notebook = gtk_notebook_new();
  menu_f = gtk_menu_new();
  menu_e = gtk_menu_new();
  menu_h = gtk_menu_new();

    for (i=0; i<4; i++)
    {
    sprintf(buf, label_f[i], i);
    menu_items_f = gtk_menu_item_new_with_label (buf);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_f), menu_items_f);

    g_signal_connect_swapped (G_OBJECT (menu_items_f), "activate",
                              G_CALLBACK (menuitem_response),  (gpointer) g_strdup (buf));
    gtk_widget_show (menu_items_f);
    }

    for (i=0; i<6; i++)
    {
    sprintf(buf, label_e[i], i);
    menu_items_e = gtk_menu_item_new_with_label (buf);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_e), menu_items_e);

    g_signal_connect_swapped (G_OBJECT (menu_items_e), "activate",
                              G_CALLBACK (menuitem_response),  (gpointer) g_strdup (buf));
    gtk_widget_show (menu_items_e);
    }
<<<<<<< HEAD:main.c

=======
>>>>>>> 3231d261ba3a8861efeabfcb54ac269c92a3be07:main.c

    for (i=0; i<3; i++)
    {
    sprintf(buf, label_h[i], i);
    menu_items_h = gtk_menu_item_new_with_label (buf);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_h), menu_items_h);

    g_signal_connect_swapped (G_OBJECT (menu_items_h), "activate",
                              G_CALLBACK (menuitem_response),  (gpointer) g_strdup (buf));
    gtk_widget_show (menu_items_h);
    }

    file_menu = gtk_menu_item_new_with_label ("Файл");
    edit_menu = gtk_menu_item_new_with_label ("Правка");
    help_menu = gtk_menu_item_new_with_label ("Справка");
    gtk_widget_show (file_menu);
    gtk_widget_show (edit_menu);
    gtk_widget_show (help_menu);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_menu), menu_f);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (edit_menu), menu_e);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (help_menu), menu_h);
    menu_bar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (vbox), menu_bar, TRUE, FALSE, 2);
    gtk_widget_show (menu_bar);
<<<<<<< HEAD:main.c
<<<<<<< HEAD:main.c
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), root_menu);
=======
=======
>>>>>>> 3231d261ba3a8861efeabfcb54ac269c92a3be07:main.c
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), file_menu);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), edit_menu);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), help_menu);

<<<<<<< HEAD:main.c
>>>>>>> 3f1bff366916a59063563f0997c93c93c89c27d9:main.c
=======
>>>>>>> 3231d261ba3a8861efeabfcb54ac269c92a3be07:main.c
/*******************************************************************/
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

static void menuitem_response( gchar *string )
{
            printf ("%s\n", string);
}
