#include "tab2.h"
#include <gtk/gtk.h>
/* garfeild.c */

void entry_print(GtkWidget *gw, GtkWidget *entry)
{  
  g_print ("%s\n", gtk_entry_get_text(GTK_ENTRY(entry)));
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

  char *nameButton[2] = { "Clear", "Print" };
  char *nameInput[13] = { "Input 2.1", "Input 2.2" };

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
