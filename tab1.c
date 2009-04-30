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
  
    rc = sqlite3_exec(db, "SELECT MODEL FROM input", callback, 0, &zErrMsg);
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
    GtkWidget *table; //таблица для frame`ов
    GtkWidget *table_in; 
    GtkWidget *entry;
    GtkWidget *sch_button, *clr_button;
    GtkWidget *all, *none, *input, *output, *work, *defend; //checkbuttons for param-s
    GtkWidget *input_label;
    GtkWidget *frame;
    GtkWidget *bbox; //button box
    GtkWidget *ch_bbox1, *ch_bbox2; //for checkbuttons
    GtkWidget *checkbutton; //enable\disable entries
    GtkWidget *hbox, *vbox; //for ch_boxes
    int i,j,k,m,n;
    gchar *frame_name[4] = {"Входные параметры", "Выходные параметры", "Рабочие функции", "Защитные фунции"};
    gchar *input_entry_name[3] = {"число фаз", "напряжение питающией сети,В", "частота питающей сети,Гц"};
/*    char *entry_name1[N] = {""};
    char *entry_name1[N] = {""};
    char *entry_name1[N] = {""};*/
    table = gtk_table_new (4, 2, FALSE);

/*  table_int = gtk_table_new (1, 3, TRUE); //input
    table_int = gtk_table_new (2, 3, TRUE); //work funcs
    table_int = gtk_table_new (3, 3, TRUE); //defend funcs 
    table_int = gtk_table_new (4, 3, TRUE); //output */
    sch_button = gtk_button_new_with_label ("Поиск"); 
//    gtk_widget_set_size_request (sch_button, 70, 35);
    clr_button = gtk_button_new_with_label ("Сброс"); 
//    gtk_widget_set_size_request (clr_button, 70, 35);
k=0;
while (k<4){
for (i=0; i<2; i++)
for (j=0; j<2; j++)
    {
        frame = gtk_frame_new(NULL);
        checkbutton = gtk_check_button_new_with_label (frame_name[k]);
        gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
        gtk_table_attach_defaults (GTK_TABLE (table), frame, 0+i, 1+i, 0+j, 1+j);
        table_in = gtk_table_new (k+1, 3, FALSE);
            if (k==0)
            {
                for(m=0; m<3; m++)
                {
                entry = gtk_entry_new ();
                vbox = gtk_vbox_new (FALSE, 0);
                hbox = gtk_hbox_new (FALSE, 0);
                checkbutton = gtk_check_button_new ();
                input_label = gtk_label_new (input_entry_name[m]);
                gtk_container_add (GTK_CONTAINER(vbox), input_label);
                gtk_container_add (GTK_CONTAINER(vbox), hbox);
                gtk_container_add (GTK_CONTAINER(hbox), checkbutton);
                gtk_container_add (GTK_CONTAINER(hbox), entry);
                gtk_table_attach_defaults (GTK_TABLE (table_in), vbox, 0+m, 1+m, 0, 1);
                g_signal_connect (G_OBJECT (sch_button), "clicked",
                      G_CALLBACK (sch_callback), (gpointer) entry); 
                g_signal_connect (G_OBJECT (clr_button), "clicked",
                      G_CALLBACK (clr_callback), (gpointer) entry);
                }
            }

            if (k==1)
            {
                for(m=0; m<3; m++)
                for(n=0; n<4; n++)
                {
                entry = gtk_entry_new ();
        //        input_label = gtk_label_new (input_entry_name[m]);
        //        gtk_container_add (GTK_CONTAINER(entry), input_label);
                gtk_table_attach_defaults (GTK_TABLE (table_in), entry, 0+m, 1+m, 0+n, 1+n);
                g_signal_connect (G_OBJECT (sch_button), "clicked",
                      G_CALLBACK (sch_callback), (gpointer) entry); 
                g_signal_connect (G_OBJECT (clr_button), "clicked",
                      G_CALLBACK (clr_callback), (gpointer) entry);
                }
            }

            if (k==2)
            {
                for(m=0; m<3; m++)
                for(n=0; n<2; n++)
                {
                entry = gtk_entry_new ();
        //        input_label = gtk_label_new (input_entry_name[m]);
        //        gtk_container_add (GTK_CONTAINER(entry), input_label);
                gtk_table_attach_defaults (GTK_TABLE (table_in), entry, 0+m, 1+m, 0+n, 1+n);
                g_signal_connect (G_OBJECT (sch_button), "clicked",
                      G_CALLBACK (sch_callback), (gpointer) entry); 
                g_signal_connect (G_OBJECT (clr_button), "clicked",
                      G_CALLBACK (clr_callback), (gpointer) entry);
                }
            }
            if (k==3)
            {
                for(m=0; m<3; m++)
                for(n=0; n<3; n++)
                {
                entry = gtk_entry_new ();
        //        input_label = gtk_label_new (input_entry_name[m]);
        //        gtk_container_add (GTK_CONTAINER(entry), input_label);
                gtk_table_attach_defaults (GTK_TABLE (table_in), entry, 0+m, 1+m, 0+n, 1+n);
                g_signal_connect (G_OBJECT (sch_button), "clicked",
                      G_CALLBACK (sch_callback), (gpointer) entry); 
                g_signal_connect (G_OBJECT (clr_button), "clicked",
                      G_CALLBACK (clr_callback), (gpointer) entry);
                }
            }

        gtk_container_add (GTK_CONTAINER(frame), table_in);
        k++;
    }
}
if (k==4)
{

}
//пока по-нубски
    ch_bbox1 = gtk_vbutton_box_new();
    ch_bbox2 = gtk_vbutton_box_new();
//checkbuttons
    all = gtk_check_button_new_with_label ("все");
    gtk_container_add (GTK_CONTAINER (ch_bbox1), all);

    none = gtk_check_button_new_with_label ("ни одного");
    gtk_container_add (GTK_CONTAINER (ch_bbox2), none);

    input = gtk_check_button_new_with_label ("входные");
    gtk_container_add (GTK_CONTAINER (ch_bbox1), input);

    output = gtk_check_button_new_with_label ("выходные");
    gtk_container_add (GTK_CONTAINER (ch_bbox2), output);

    work = gtk_check_button_new_with_label ("рабочие ф-ции");
    gtk_container_add (GTK_CONTAINER (ch_bbox1), work);

    defend = gtk_check_button_new_with_label ("защитные ф-ции");
    gtk_container_add (GTK_CONTAINER (ch_bbox2), defend);

    hbox = gtk_hbox_new(FALSE, 0);

    gtk_container_add (GTK_CONTAINER (hbox), ch_bbox1);
    gtk_container_add (GTK_CONTAINER (hbox), ch_bbox2);

    gtk_table_attach_defaults (GTK_TABLE (table), hbox, 0, 1, 2, 3);

    bbox = gtk_hbutton_box_new();
    gtk_container_add (GTK_CONTAINER (bbox), sch_button);
    gtk_container_add (GTK_CONTAINER (bbox), clr_button);
    gtk_table_attach_defaults (GTK_TABLE (table), bbox, 1, 2, 2, 3);
    
return table;
}
