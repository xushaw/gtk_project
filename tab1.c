#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "tab1.h"
#include <sqlite3.h>

sqlite3 *db;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
     int i;
     NotUsed=0;

    for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static void sch_callback( GtkWidget *widget, GtkWidget *entry ) 
{
    char *zErrMsg = 0;
    int rc;

    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  
    rc = sqlite3_exec(db, "SELECT ALL * FROM hello", callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK )
                            {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    }
}

static void clr_callback( GtkWidget *widget, GtkWidget *entry )
{
    gtk_entry_set_text (GTK_ENTRY (entry), "");
}
/***********************************************************************/
GtkWidget* tab1 ()
{   

    GtkWidget *table;
    GtkWidget *entry;
    GtkWidget *sch_button, *clr_button;
/*    int rc; 

    rc = sqlite3_open("base.db", &db);
    if( rc ){
                  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                                sqlite3_close(db);
                                exit(1);
    }*/

    int i,j;
    sch_button = gtk_button_new_with_label ("Поиск"); 
    gtk_widget_set_size_request (sch_button, 70, 35);

    clr_button = gtk_button_new_with_label ("Сброс"); 
    gtk_widget_set_size_request (clr_button, 70, 35);

    table = gtk_table_new (5, 2, TRUE); 

    gtk_table_attach_defaults (GTK_TABLE (table), sch_button, 0, 1, 3, 4);
    gtk_table_attach_defaults (GTK_TABLE (table), clr_button, 1, 2, 3, 4);
        
for (i=0; i<2; i++)
    for (j=0; j<3; j++)
    {    
    entry = gtk_entry_new ();
    gtk_table_attach_defaults (GTK_TABLE (table), entry, 0+i, 1+i, 0+j, 1+j);
    g_signal_connect (G_OBJECT (sch_button), "clicked",
                      G_CALLBACK (sch_callback), (gpointer) entry); 
    g_signal_connect (G_OBJECT (clr_button), "clicked",
                      G_CALLBACK (clr_callback), (gpointer) entry);

    gtk_widget_show(entry);
    }
return table;
}
