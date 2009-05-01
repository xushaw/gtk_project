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

//void choice_callback (...)
//{
//
//}
/***********************************************************************/
GtkWidget* tab1 ()
{   
    GtkWidget *table; //таблица для frame`ов
    GtkWidget *table_in; 
    GtkWidget *entry;
    GtkWidget *sch_button, *clr_button;
    GtkWidget *label;
    GtkWidget *frame;
    GtkWidget *bbox; //button box
    GtkWidget *ch_bbox1, *ch_bbox2; //for checkbuttons
    GtkWidget *checkbutton; //enable\disable entries
    GtkWidget *hbox, *vbox, *ex_hbox; //for ch_boxes
    GtkWidget *scr_window;
    GtkWidget *opt, *ch; //choice
    GtkWidget *combo;
    int i,j,m,n,k;
    gchar *frame_name[6] = {"Входные параметры","Выходные параметры","Рабочие функции","Защитные фунции"};

    gchar *input_entry_name[3]={"число фаз","напряжение питающией сети,В", "частота питающей сети,Гц"};

    gchar *output_entry_name[11]={"мощность", "число фаз", "диапазон регулирования напряжения", 
    "дискретность регулирования напряжения", "диапазон регулирования частоты", "дискретность регулирования частоты",
    "диапазон регулирования скорости в разомкнутой системе", "диапазон регулирования скорости в замкнутой системе",
    "принцип управления", "способ модуляции", "тактовая частота ШИМ"};

    gchar *work_name[6]={"Задание частоты или технологической переменной","Установки задания частоты или технологической переменной","Команды пуск/стоп","Пуско-тормозная характеристика", "Автоматический повторный пуск при обнаружении ошибки","Регулятор технологической переменной"};

    gchar *defend_title[2]={"Защиты преобразователя и двигателя","Защита от неправильной работы тиристорного выпрямителя"};

    gchar *defend1_name[4]={"Защита от короткого замыкания на корпус","Максимально-токовая защита","Защита от обрыва и перекоса фаз","Защита от понижения или повышения напряжения в звене постоянного тока"};
    gchar *defend2_name[2]={"Тепловая защита","Защита от потери питания контроллером"};

    gchar *sch_filter[6]={"все","входные параметры","выходные параметры","рабочие функции","защитные функции","ни одного"};
    table = gtk_table_new (13, 3, FALSE);
    sch_button = gtk_button_new_with_label ("Поиск"); 
//    gtk_widget_set_size_request (sch_button, 70, 35);
    clr_button = gtk_button_new_with_label ("Сброс"); 
//    gtk_widget_set_size_request (clr_button, 70, 35);
//k=0;
//while (k<4){
//for (i=0; i<2; i++)
for (j=0; j<11; j++)
    {
       /* frame = gtk_frame_new(NULL);
        checkbutton = gtk_check_button_new_with_label (frame_name[j]);
        gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
        gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 0+j, 1+j);*/
        if (j==0)
            {
        frame = gtk_frame_new(NULL);
        checkbutton = gtk_check_button_new_with_label (frame_name[0]);
        gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
        gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 0, 1);

                table_in = gtk_table_new (1, 3, FALSE);
        gtk_container_add (GTK_CONTAINER(frame), table_in);
                for(m=0; m<3; m++)
                {
                entry = gtk_entry_new ();
                vbox = gtk_vbox_new (FALSE, 0);
                checkbutton = gtk_check_button_new_with_label (input_entry_name[m]);
                gtk_container_add (GTK_CONTAINER(vbox), checkbutton);
                gtk_container_add (GTK_CONTAINER(vbox), entry);
                gtk_table_attach_defaults (GTK_TABLE (table_in), vbox, 0+m, 1+m, 0, 1);
                g_signal_connect (G_OBJECT (sch_button), "clicked",
                      G_CALLBACK (sch_callback), (gpointer) entry); 
                g_signal_connect (G_OBJECT (clr_button), "clicked",
                      G_CALLBACK (clr_callback), (gpointer) entry);
                }
            }
        
            if (j==1)
            {

        frame = gtk_frame_new(NULL);
        checkbutton = gtk_check_button_new_with_label (frame_name[j]);
        gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
        gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 1, 4);
                table_in = gtk_table_new (4, 3, FALSE);
        gtk_container_add (GTK_CONTAINER(frame), table_in);
                k=0;
                for(m=0; m<3; m++)
                for(n=0; n<4; n++)
                {
                entry = gtk_entry_new ();
                vbox = gtk_vbox_new (FALSE, 0);
                checkbutton = gtk_check_button_new_with_label (output_entry_name[k]);
                gtk_container_add (GTK_CONTAINER(vbox), checkbutton);
                gtk_container_add (GTK_CONTAINER(vbox), entry);
                gtk_table_attach_defaults (GTK_TABLE (table_in), vbox, 0+m, 1+m, 0+n, 1+n);
                g_signal_connect (G_OBJECT (sch_button), "clicked",
                      G_CALLBACK (sch_callback), (gpointer) entry); 
                g_signal_connect (G_OBJECT (clr_button), "clicked",
                      G_CALLBACK (clr_callback), (gpointer) entry);
                k++;
                }
            }

            if (j==5)
            {

        frame = gtk_frame_new(NULL);
        checkbutton = gtk_check_button_new_with_label (frame_name[2]);
        gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
        gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 5, 7);
                table_in = gtk_table_new (2, 3, FALSE);
        gtk_container_add (GTK_CONTAINER(frame), table_in);
                k=0;
                for(m=0; m<2; m++)
                for(n=0; n<3; n++)
                {
                hbox = gtk_hbox_new (FALSE, 0);
                combo = gtk_combo_box_new_text();
                gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "есть");
                gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "нет");
                checkbutton = gtk_check_button_new_with_label (work_name[k]);
                gtk_container_add (GTK_CONTAINER(hbox), checkbutton);
                gtk_container_add (GTK_CONTAINER(hbox), combo);
                gtk_table_attach_defaults (GTK_TABLE (table_in), hbox, 0+m, 1+m, 0+n, 1+n);
                k++;
                }
            }
            if (j==7)
            {
        frame = gtk_frame_new(NULL);
        checkbutton = gtk_check_button_new_with_label (defend_title[0]);
        gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
        gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 7, 9);
                table_in = gtk_table_new (1, 2, FALSE);
        gtk_container_add (GTK_CONTAINER(frame), table_in);
                for(m=0; m<2; m++)
                {
                vbox = gtk_vbox_new (FALSE, 0);
                checkbutton = gtk_check_button_new_with_label (defend1_name[m]);
                gtk_container_add (GTK_CONTAINER(vbox), checkbutton);
                gtk_table_attach_defaults (GTK_TABLE (table_in), vbox, 0+m, 1+m, 0, 1);
                }
            }
            if (j==9)
            {

        frame = gtk_frame_new(NULL);
        checkbutton = gtk_check_button_new_with_label (defend_title[1]);
        gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
        gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 9, 11);
                table_in = gtk_table_new (1, 4, FALSE);
        gtk_container_add (GTK_CONTAINER(frame), table_in);
                for(m=0; m<4; m++)
                {
                vbox = gtk_vbox_new (FALSE, 0);
                checkbutton = gtk_check_button_new_with_label (defend2_name[m]);
                gtk_container_add (GTK_CONTAINER(vbox), checkbutton);
                gtk_table_attach_defaults (GTK_TABLE (table_in), vbox, 0+m, 1+m, 0, 1);
                }
            }

}
//checkbuttons
    hbox = gtk_hbox_new(FALSE, 0);

    for (i=0; i<6; i++)
    {
        checkbutton = gtk_check_button_new_with_label (sch_filter[i]);
        gtk_container_add (GTK_CONTAINER (hbox), checkbutton);
    }
  

    bbox = gtk_hbutton_box_new();
    gtk_container_add (GTK_CONTAINER (bbox), sch_button);
    gtk_container_add (GTK_CONTAINER (bbox), clr_button);
    gtk_table_attach_defaults (GTK_TABLE (table), bbox, 2, 3, 11, 12);
    gtk_table_attach_defaults (GTK_TABLE (table), hbox, 0, 3, 12, 13);
    
return table;
}
