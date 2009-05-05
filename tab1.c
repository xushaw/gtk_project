#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "tab1.h"
#include <sqlite3.h>
#include <glib.h>

sqlite3 *db;
int table_cnt;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
     int i;
     NotUsed=0;

    for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static void sch_callback( GtkWidget *widget, GPtrArray *arr) 
{
    char *zErrMsg = 0;
    int rc, i;
    gchar *str1, *str2, *temp, *temp2;
    gboolean flag = TRUE;
    table_cnt=0;
    //g_print("Begin\n");
    if ( gtk_toggle_button_get_active(g_ptr_array_index(arr, 0)) == TRUE )
    {
        //g_print("First if\n");
            temp = g_strdup("SELECT MODEL FROM input ");
            //g_print("Dup\n");
            table_cnt++;
            for (i=1; i<4; i++)
            {
                //g_print("For %d\n", i);
                if ( GTK_WIDGET_SENSITIVE (g_ptr_array_index(arr, i)) == TRUE)
                {
                    //g_print("if\n");
                    if (flag == TRUE) 
                    {
                        str1 = g_strconcat (temp, gtk_widget_get_name(g_ptr_array_index(arr,i)), "=\"", 
                            gtk_entry_get_text(g_ptr_array_index(arr,i)), "\"", NULL);
                        flag = FALSE;
                    }
                    else 
                    {
                        str1 = g_strconcat (temp," AND ", gtk_widget_get_name(g_ptr_array_index(arr,i)), 
                            "=\"", gtk_entry_get_text(g_ptr_array_index(arr,i)), "\"", NULL);
                    }
                    temp = g_strdup (str1);
                    g_print("%s\n", str1);
                }
            }
    }

    if ( gtk_toggle_button_get_active(g_ptr_array_index(arr, 4)) == TRUE )
    {
        //g_print("First if\n");
        temp2 = g_strdup("SELECT MODEL FROM output ");
        g_print("%s\n", temp2);
        //g_print("Dup\n");
        table_cnt++;
        flag = TRUE;
        for (i=5; i<16; i++)
        {
            if ( GTK_WIDGET_SENSITIVE (g_ptr_array_index(arr, i)) == TRUE)
            {
                //g_print("if\n");
                if (flag == FALSE) 
                {
                    str2 = g_strconcat(temp2, " AND ", NULL);
                    temp2 = g_strdup(str2);
                }
                else { 
                    flag = FALSE;
                }
                
                if (g_strcmp0("power_", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0 ||
                    g_strcmp0("range_reg_U_", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0 || 
                    g_strcmp0("diskr_reg_U_", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0 || 
                    g_strcmp0("range_reg_f_", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0 || 
                    g_strcmp0("diskr_reg_f_", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0 || 
                    g_strcmp0("range_reg_v_razsys_", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0 || 
                    g_strcmp0("range_reg_v_zamsys_", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0)
                    {
                        str2 = g_strconcat (temp2,
                        gtk_widget_get_name(g_ptr_array_index(arr,i)),
                        "max<=\"", //max<=
                        gtk_entry_get_text(g_ptr_array_index(arr,i)),
                        "\" AND ",
                        gtk_widget_get_name(g_ptr_array_index(arr,i)),
                        "min>=\"",
                        gtk_entry_get_text(g_ptr_array_index(arr,i)),
                        "\"",
                        NULL);
                     //   g_print("IF: %s\n", str2);
                    }
                    else if (g_strcmp0("phase_number", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0 ||
                        g_strcmp0("control_way", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0 || 
                        g_strcmp0("mod_way", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0 || 
                        g_strcmp0("takt_f", gtk_widget_get_name(g_ptr_array_index(arr, i)) ) == 0)
                        {
                            str2 = g_strconcat (temp2, gtk_widget_get_name(g_ptr_array_index(arr,i)), "=\"", 
                            gtk_entry_get_text(g_ptr_array_index(arr,i)), "\"", NULL);
                            g_print("ELSE: %s\n", str2);
                        }
                    temp2 = g_strdup (str2);
                //    g_print("%s\n", str2);
                }
            }
    }
        rc = sqlite3_exec(db, str1, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }
    
        rc = sqlite3_exec(db, str2, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }

/*        rc = sqlite3_exec(db, str3, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }

        rc = sqlite3_exec(db, str4, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }*/ 
}

static void clr_callback( GtkWidget *widget, GPtrArray *array )
{
    int i;
    for (i=0; i < array->len; i++)
    {
        if (g_strcmp0 (GTK_OBJECT_TYPE_NAME (g_ptr_array_index(array, i)), "GtkEntry") == 0)
        {
            gtk_entry_set_text (GTK_ENTRY (g_ptr_array_index(array, i)), "");
        }
        else if (g_strcmp0 (GTK_OBJECT_TYPE_NAME (g_ptr_array_index(array, i)),"GtkComboBox") == 0)
            gtk_combo_box_set_active (GTK_COMBO_BOX(g_ptr_array_index(array, i)), -1);
    }
}

static void state_callback( GtkToggleButton *checkbutton, GtkWidget *entry )
{
    gboolean val;
    val = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbutton));
    if (val == TRUE)
    {
        //button is pressed
        gtk_widget_set_sensitive (GTK_WIDGET(entry), TRUE);
    }
    else if (val == FALSE)
    {
        //button is raised
        gtk_widget_set_sensitive (GTK_WIDGET(entry), FALSE);

    }
    
}
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
    GPtrArray *array, *arr2;
    int i,j,m,n,k;
    gchar *frame_name[6] = {"Входные параметры","Выходные параметры","Рабочие функции","Защитные фунции"};

    gchar *input_entry_name[3]={"число фаз","напряжение питающией сети,В", "частота питающей сети,Гц"};

    gchar *output_entry_name[11]={"мощность", "число фаз", "диапазон регулирования напряжения", 
    "дискретность регулирования напряжения", "диапазон регулирования частоты", "дискретность регулирования частоты",
    "диапазон регулирования скорости в разомкнутой системе", "диапазон регулирования скорости в замкнутой системе",
    "принцип управления", "способ модуляции", "тактовая частота ШИМ"};

    gchar *work_name[6]={"Задание частоты или технологической переменной",
    "Установки задания частоты или технологической переменной",
    "Команды пуск/стоп","Пуско-тормозная характеристика",
    "Автоматический повторный пуск при обнаружении ошибки","Регулятор технологической переменной"};

    gchar *defend_title[2]={"Защиты преобразователя и двигателя",
    "Защита от неправильной работы тиристорного выпрямителя"};

    gchar *defend1_name[4]={"Защита от короткого замыкания на корпус",
    "Максимально-токовая защита","Защита от обрыва и перекоса фаз",
    "Защита от понижения или повышения напряжения в звене постоянного тока"};
    gchar *defend2_name[2]={"Тепловая защита","Защита от потери питания контроллером"};

    gchar *sch_filter[6]={"все","входные параметры","выходные параметры","рабочие функции","защитные функции",
    "ни одного"};
    gchar *base_frame[5] = {"input", "output", "work_functions","defend_functions2", "defend_functions4"};
    gchar *base_input[3] = {"phase_number","U_in","freq_in"};
    gchar *base_output[11] = {"power_","phase_number","range_reg_U_", "diskr_reg_U_", "range_reg_f_", "diskr_reg_f_",
    "range_reg_v_razsys_", "range_reg_v_zamsys_", "control_way", "mod_way", "takt_f"};

table = gtk_table_new (13, 3, FALSE);
array = g_ptr_array_new ();
//arr2 = g_ptr_array_new ();

    for (j=0; j<11; j++)
    {
        if (j==0) //input
        {
            frame = gtk_frame_new(NULL);
            checkbutton = gtk_check_button_new_with_label (frame_name[0]);
            g_ptr_array_add (array, (gpointer) checkbutton);
            gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
            gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 0, 1);
            table_in = gtk_table_new (1, 3, FALSE);
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
            g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                  G_CALLBACK (state_callback), (gpointer) table_in);
            gtk_container_add (GTK_CONTAINER(frame), table_in);
                for(m=0; m<3; m++)
                {
                    entry = gtk_entry_new ();
                    gtk_widget_set_name (GTK_WIDGET(entry), base_input[m]);
                    g_ptr_array_add (array, (gpointer) entry);
                    vbox = gtk_vbox_new (FALSE, 0);
                    checkbutton = gtk_check_button_new_with_label (input_entry_name[m]);
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                    g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                      G_CALLBACK (state_callback), (gpointer) entry);
                    gtk_container_add (GTK_CONTAINER(vbox), checkbutton);
                    gtk_container_add (GTK_CONTAINER(vbox), entry);
                    gtk_table_attach_defaults (GTK_TABLE (table_in), vbox, 0+m, 1+m, 0, 1);
                }
         }  
        
        if (j==1) //output
        {
            frame = gtk_frame_new(NULL);
            checkbutton = gtk_check_button_new_with_label (frame_name[j]);
//            gtk_widget_set_name (GTK_WIDGET(checkbutton), base_output[j]);
            g_ptr_array_add (array, (gpointer) checkbutton);
            gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
            gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 1, 4);
            table_in = gtk_table_new (4, 3, FALSE);
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
            g_signal_connect (G_OBJECT (checkbutton), "toggled",
                              G_CALLBACK (state_callback), (gpointer) table_in);
            gtk_container_add (GTK_CONTAINER(frame), table_in);
                k=0;
                for(m=0; m<3; m++)
                for(n=0; n<4; n++)
                if (n!=3 || m!=2)
                {
                    printf ("n=%d\n", n);
                    printf ("m=%d\n", m);
                    entry = gtk_entry_new ();
                    g_ptr_array_add (array, (gpointer) entry);
                    gtk_widget_set_name (GTK_WIDGET(entry), base_output[k]);
                    vbox = gtk_vbox_new (FALSE, 0);
                    checkbutton = gtk_check_button_new_with_label (output_entry_name[k]);
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                    g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                      G_CALLBACK (state_callback), (gpointer) entry);
                    gtk_container_add (GTK_CONTAINER(vbox), checkbutton);
                    gtk_container_add (GTK_CONTAINER(vbox), entry);
                    gtk_table_attach_defaults (GTK_TABLE (table_in), vbox, 0+m, 1+m, 0+n, 1+n);
                    k++;
                }
            }

            if (j==5) //work
            {
                frame = gtk_frame_new(NULL);
                checkbutton = gtk_check_button_new_with_label (frame_name[2]);
            //    gtk_widget_set_name (GTK_WIDGET(checkbutton), frame_name[2]);
                g_ptr_array_add (array, (gpointer) checkbutton);
                gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
                gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 5, 7);
                table_in = gtk_table_new (2, 3, FALSE);
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                  G_CALLBACK (state_callback), (gpointer) table_in);
                gtk_container_add (GTK_CONTAINER(frame), table_in);
                k=0;
                for(m=0; m<2; m++)
                for(n=0; n<3; n++)
                {
                    hbox = gtk_hbox_new (FALSE, 0);
                    combo = gtk_combo_box_new_text();
                    g_ptr_array_add (array, (gpointer) combo);
                    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "есть");
                    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "нет");
                    checkbutton = gtk_check_button_new_with_label (work_name[k]);
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                    g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                      G_CALLBACK (state_callback), (gpointer) combo);
                    gtk_container_add (GTK_CONTAINER(hbox), checkbutton);
                    gtk_container_add (GTK_CONTAINER(hbox), combo);
                    gtk_table_attach_defaults (GTK_TABLE (table_in), hbox, 0+m, 1+m, 0+n, 1+n);
                    k++;
                }
            }
            if (j==7) //defend
            {
                frame = gtk_frame_new(NULL);
                checkbutton = gtk_check_button_new_with_label (defend_title[0]);
         //       gtk_widget_set_name (GTK_WIDGET(checkbutton), defend_title[0]);
                g_ptr_array_add (array, (gpointer) checkbutton);
                gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 7, 9);
                table_in = gtk_table_new (2, 2, FALSE);
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                  G_CALLBACK (state_callback), (gpointer) table_in);
                gtk_container_add (GTK_CONTAINER(frame), table_in);
                k=0;
                for(m=0; m<2; m++)
                for(n=0; n<2; n++)
                {
                    hbox = gtk_hbox_new (FALSE, 0);
                    combo = gtk_combo_box_new_text();
              //      gtk_widget_set_name (GTK_WIDGET(combo), defend1_name[m]);
                    g_ptr_array_add (array, (gpointer) combo);
                    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "есть");
                    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "нет");
                    checkbutton = gtk_check_button_new_with_label (defend1_name[k]);
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                    g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                      G_CALLBACK (state_callback), (gpointer) combo);
                    gtk_container_add (GTK_CONTAINER(hbox), checkbutton);
                    gtk_container_add (GTK_CONTAINER(hbox), combo);
                    gtk_table_attach_defaults (GTK_TABLE (table_in), hbox, 0+m, 1+m, 0+n, 1+n);
                    k++;
                }
            }
            if (j==9) //defend2
            {
                frame = gtk_frame_new(NULL);
                checkbutton = gtk_check_button_new_with_label (defend_title[1]);
          //      gtk_widget_set_name (GTK_WIDGET(checkbutton), defend_title[1]);
                g_ptr_array_add (array, (gpointer) checkbutton);
                gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
                gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 9, 11);
                table_in = gtk_table_new (1, 2, FALSE);
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                  G_CALLBACK (state_callback), (gpointer) table_in);
                gtk_container_add (GTK_CONTAINER(frame), table_in);
                for(m=0; m<2; m++)
                {
                    hbox = gtk_hbox_new (FALSE, 0);
                    combo = gtk_combo_box_new_text();
                    g_ptr_array_add (array, (gpointer) combo);
            //        gtk_widget_set_name (GTK_WIDGET(combo), defend2_name[m]);
                    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "есть");
                    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "нет");
                    checkbutton = gtk_check_button_new_with_label (defend2_name[m]);
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                    g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                      G_CALLBACK (state_callback), (gpointer) combo);
                    gtk_container_add (GTK_CONTAINER(hbox), checkbutton);
                    gtk_container_add (GTK_CONTAINER(hbox), combo);
                    gtk_table_attach_defaults (GTK_TABLE (table_in), hbox, 0+m, 1+m, 0, 1);
                }
            }
    }

    for (i=0; i<array->len; i++)
    {
        g_print("%d: %s\n" ,i, gtk_widget_get_name(g_ptr_array_index(array, i)));
    }
//****************************************************************************************
/*    for (i=0; i<array->len; i++)
    {
        gboolean val;
        gchar str_1;
        val = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (g_ptr_array_index(array, 0)));
        if (val == TRUE)
        {
            g_ptr_array_add (arr2, (gpointer) g_ptr_array_index (array, i));
            //loop by k
            str = g_strconcat("SELECT MODEL FROM ", base_frame[k], NULL);
//            str_1 = func (arr2, "SELECT MODEL FROM" table[i]); 
//            g_print ("%d\n", val);
        }
        
    }   */

/*    printf("----------------\n");
    for (i=0; i<arr2->len; i++)
    {
        g_print("%s\n" ,GTK_OBJECT_TYPE_NAME (g_ptr_array_index(arr2, i)));
    }*/

sch_button = gtk_button_new_with_label ("Поиск"); 
gtk_widget_set_size_request (sch_button, 70, 35);
clr_button = gtk_button_new_with_label ("Сброс"); 
gtk_widget_set_size_request (clr_button, 70, 35);

g_signal_connect (G_OBJECT (sch_button), "clicked",
                  G_CALLBACK (sch_callback), (gpointer) array);//array 
g_signal_connect (G_OBJECT (clr_button), "clicked",
                  G_CALLBACK (clr_callback), (gpointer) array);

//checkbuttons
hbox = gtk_hbox_new(FALSE, 0);

    for (i=0; i<6; i++)
    {
        checkbutton = gtk_check_button_new_with_label (sch_filter[i]);
        gtk_container_add (GTK_CONTAINER (hbox), checkbutton);
    }

bbox = gtk_hbutton_box_new();
gtk_box_pack_start(GTK_BOX (bbox), clr_button, TRUE, FALSE, 0);
gtk_box_pack_start(GTK_BOX (bbox), sch_button, TRUE, FALSE, 0);
gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
gtk_table_attach_defaults (GTK_TABLE (table), bbox, 2, 3, 11, 12);
gtk_table_attach_defaults (GTK_TABLE (table), hbox, 0, 3, 12, 13);
    
return table;
}
