#include "tab2.h"
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <glib/gprintf.h>
/* garfeild.c */

sqlite3 *db;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
     int i;
     NotUsed=0;

    for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void entry_print(GtkWidget *gw, GtkWidget *entry)
{ 
    char *zErrMsg = 0;
    int rc;
    
    int k = sizeof("SELECT ALL * FROM garfeild WHERE id=\"") + sizeof(gtk_entry_get_text(GTK_ENTRY(entry))) + sizeof("\";"); 
    //g_print("%d\n", k*sizeof(char));
    char *str = (char*)malloc(k*sizeof(char));
    //g_print("MALLOC!\n");
    
    memset(str, 0, k*sizeof(char));
    //g_print("MEMSET\n");
    strcat(str, "SELECT ALL * FROM garfeild WHERE id=\"");
    //g_print("CAT1\n");
    //g_print("%s\n", str);
    //g_print("%d\n", strlen(str));
    strcat(str, gtk_entry_get_text(GTK_ENTRY(entry)));
    //g_print("CAT2\n");
    //g_print("%s\n", str);
    //g_print("%d\n", strlen(str));
    strcat(str, "\";");
    //g_print("CAT3\n");
    //g_print("%s\n", str);
    //g_print("%d\n", strlen(str));

    g_print ("%s\n", str);
    rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        g_fprintf(stderr,"SQL error: %s\n", zErrMsg);
    }
}

void entry_clear(GtkWidget *gw, GtkWidget *entry)
{ 
  gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void entry_enter(GtkWidget *gw, GtkWidget *button)
{
  g_signal_emit_by_name(G_OBJECT(button), "clicked");
}

GtkWidget* tab2()
{
  GtkWidget *hboxs[2];
  GtkWidget *tableBox;
  GtkWidget *button[2];
  GtkWidget *input[2];
  GtkWidget *labelInput[2];
  GtkWidget *rightAlignment[2];
  GtkWidget *buttonBox;
  int j;

  char *nameButton[2] = { "Сброс", "Поиск" };
  char *nameInput[13] = { "Данные 1", "Данные 2" };

  //initiating buttons and labels
   buttonBox = gtk_hbutton_box_new();
   gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonBox), GTK_BUTTONBOX_END);
 
   tableBox = gtk_table_new(2, 2, FALSE);
   gtk_table_set_row_spacings(GTK_TABLE(tableBox), 5);
   gtk_table_set_col_spacings(GTK_TABLE(tableBox), 5);
   
   for(j=0; j<2; j++)  {
       button[j] = gtk_button_new_with_label(nameButton[j]);
       gtk_widget_set_size_request(button[j], 70, 35);
       
       labelInput[j] = gtk_label_new(nameInput[j]);
       input[j] = gtk_entry_new();
       
       rightAlignment[j] = gtk_alignment_new(1, 0.5, 0.1, 0);
       
       hboxs[j] = gtk_hbox_new(FALSE, 5);
       
       gtk_container_add(GTK_CONTAINER(rightAlignment[j]), labelInput[j]);
       
       gtk_box_pack_start(GTK_BOX(hboxs[j]), rightAlignment[j], FALSE, FALSE, 0);
       gtk_box_pack_start(GTK_BOX(hboxs[j]), input[j], FALSE, FALSE, 0);
   }
   
   for(j=0; j<2; j++)
       gtk_table_attach_defaults(GTK_TABLE(tableBox), hboxs[j], 0+j, 1+j, 0, 1);
   
   for(j=0; j<2; j++)
       gtk_box_pack_start(GTK_BOX(buttonBox), button[j], FALSE, FALSE, 0);
   
   gtk_table_attach_defaults(GTK_TABLE(tableBox), buttonBox, 1, 2, 1, 2);
   
   for(j=0; j<2; j++)  {
       g_signal_connect(G_OBJECT(button[0]), "clicked", 
               G_CALLBACK(entry_clear), (gpointer) input[j]);
       
       g_signal_connect(G_OBJECT(button[1]), "clicked", 
               G_CALLBACK(entry_print), (gpointer) input[j]);
       
       g_signal_connect(G_OBJECT(input[j]), "activate",
               G_CALLBACK(entry_enter), (gpointer) button[1]);
   }
   
   gtk_container_set_border_width(GTK_CONTAINER(tableBox), 5);
   return tableBox;
}
