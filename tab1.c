#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "tab1.h"
#include <sqlite3.h>
#include <glib.h>

sqlite3 *db;
int table_cnt, temptable;
GPtrArray *res1;
GPtrArray *stores;
gboolean fst = TRUE;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
     int i;
     NotUsed=0;

        for(i=0; i<argc; i++)
        {
            g_ptr_array_add(res1, argv[i]);
            g_print("CALLBACK:\t%s\n", (gchar*)g_ptr_array_index(res1, res1->len-1));
        }
     
        return 0;
}

static void sch_callback( GtkWidget *widget, GPtrArray *arr) 
{
    char *zErrMsg = 0;
    GPtrArray *tempRes, *tempRes2;
    int rc, i, j, k;
    gchar *str1, *str2, *str3, *str4, *str5, *str6, *temp, *temp2;
    gboolean flag = TRUE;
    gboolean duplex;
    table_cnt=0;
    fst = TRUE;
    str1 = g_strdup("");
    str2 = g_strdup("");
    str3 = g_strdup("");
    str4 = g_strdup("");
    str5 = g_strdup("");
    str6 = g_strdup("");
    //g_print("Begin\n");
    temp = g_strdup("SELECT MODEL FROM input WHERE ");

    tempRes = g_ptr_array_new();
    tempRes2 = g_ptr_array_new();

    if ( gtk_toggle_button_get_active(g_ptr_array_index(arr, 0)) == TRUE )
    {
        //g_print("First if\n");
            temp = g_strdup("SELECT MODEL FROM input WHERE ");
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
                 //   g_print("%s\n", str1);
                }
            }
    }

    if ( gtk_toggle_button_get_active(g_ptr_array_index(arr, 4)) == TRUE )
    {
        //g_print("First if\n");
        temp2 = g_strdup("SELECT MODEL FROM output WHERE ");
     //   g_print("%s\n", temp2);
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
                        "max>=\"", 
                        gtk_entry_get_text(g_ptr_array_index(arr,i)),
                        "\" AND ",
                        gtk_widget_get_name(g_ptr_array_index(arr,i)),
                        "min<=\"",
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
                      //      g_print("ELSE: %s\n", str2);
                        }
                    temp2 = g_strdup (str2);
                //    g_print("%s\n", str2);
                }
            }
    }

        if ( gtk_toggle_button_get_active(g_ptr_array_index(arr, 16)) == TRUE ) //work
        {
            flag = TRUE;
        //g_print("First if\n");
            temp = g_strdup("SELECT MODEL FROM work_functions WHERE ");
            //g_print("Dup\n");
            table_cnt++;
            for (i=17; i<23; i++)// ComboBox
            {
                //g_print("For %d\n", i);
                if ( GTK_WIDGET_SENSITIVE (g_ptr_array_index(arr, i)) == TRUE)
                {
                    //g_print("if\n");
                    if (flag == TRUE) 
                    {
                        str3 = g_strconcat (temp, gtk_widget_get_name(g_ptr_array_index(arr,i)), "=\"", 
                            gtk_combo_box_get_active_text(GTK_COMBO_BOX(g_ptr_array_index(arr,i))), "\"", NULL);
                        flag = FALSE;
                    }
                    else 
                    {
                        str3 = g_strconcat (temp," AND ", gtk_widget_get_name(g_ptr_array_index(arr,i)), 
                            "=\"", gtk_combo_box_get_active_text(GTK_COMBO_BOX(g_ptr_array_index(arr,i))), "\"", NULL);
                    }
                    temp = g_strdup (str3);
                  //  g_print("%s\n", str3);
                }
            }
        }

        if ( gtk_toggle_button_get_active(g_ptr_array_index(arr, 23)) == TRUE ) //defend4
        {
        //g_print("First if\n");
            flag = TRUE;
            temp = g_strdup("SELECT MODEL FROM defend_functions4 WHERE ");
            //g_print("Dup\n");
            table_cnt++;
            for (i=24; i<28; i++)// ComboBox
            {
                //g_print("For %d\n", i);
                if ( GTK_WIDGET_SENSITIVE (g_ptr_array_index(arr, i)) == TRUE)
                {
                    //g_print("if\n");
                    if (flag == TRUE) 
                    {
                        str4 = g_strconcat (temp, gtk_widget_get_name(g_ptr_array_index(arr,i)), "=\"", 
                            gtk_combo_box_get_active_text(GTK_COMBO_BOX(g_ptr_array_index(arr,i))), "\"", NULL);
                        flag = FALSE;
                    }
                    else 
                    {
                        str4 = g_strconcat (temp," AND ", gtk_widget_get_name(g_ptr_array_index(arr,i)), 
                            "=\"", gtk_combo_box_get_active_text(GTK_COMBO_BOX(g_ptr_array_index(arr,i))), "\"", NULL);
                    }
                    temp = g_strdup (str4);
                 //   g_print("%s\n", str4);
                }
            }
        }
        if ( gtk_toggle_button_get_active(g_ptr_array_index(arr, 28)) == TRUE )//defend2
        {
        //g_print("First if\n");
            flag = TRUE;
            temp = g_strdup("SELECT MODEL FROM defend_functions2 WHERE ");
            //g_print("Dup\n");
            table_cnt++;
            for (i=29; i<31; i++)// ComboBox
            {
                //g_print("For %d\n", i);
                if ( GTK_WIDGET_SENSITIVE (g_ptr_array_index(arr, i)) == TRUE)
                {
                    //g_print("if\n");
                    if (flag == TRUE) 
                    {
                        str5 = g_strconcat (temp, gtk_widget_get_name(g_ptr_array_index(arr,i)), "=\"", 
                            gtk_combo_box_get_active_text(GTK_COMBO_BOX(g_ptr_array_index(arr,i))), "\"", NULL);
                        flag = FALSE;
                    }
                    else 
                    {
                        str5 = g_strconcat (temp," AND ", gtk_widget_get_name(g_ptr_array_index(arr,i)), 
                            "=\"", gtk_combo_box_get_active_text(GTK_COMBO_BOX(g_ptr_array_index(arr,i))), "\"", NULL);
                    }
                    temp = g_strdup (str5);
                  //  g_print("%s\n", str5);
                }
            }
        }

        if ( gtk_toggle_button_get_active(g_ptr_array_index(arr, 31)) == TRUE ) //korr
        {
        //g_print("First if\n");
            flag = TRUE;
            temp = g_strdup("SELECT MODEL FROM korr WHERE ");
            //g_print("Dup\n");
            table_cnt++;
            for (i=32; i<35; i++)// ComboBox
            {
                //g_print("For %d\n", i);
                if ( GTK_WIDGET_SENSITIVE (g_ptr_array_index(arr, i)) == TRUE)
                {
                    //g_print("if\n");
                    if (flag == TRUE) 
                    {
                        str3 = g_strconcat (temp, gtk_widget_get_name(g_ptr_array_index(arr,i)), "=\"", 
                            gtk_combo_box_get_active_text(GTK_COMBO_BOX(g_ptr_array_index(arr,i))), "\"", NULL);
                        flag = FALSE;
                    }
                    else 
                    {
                        str3 = g_strconcat (temp," AND ", gtk_widget_get_name(g_ptr_array_index(arr,i)), 
                            "=\"", gtk_combo_box_get_active_text(GTK_COMBO_BOX(g_ptr_array_index(arr,i))), "\"", NULL);
                    }
                    temp = g_strdup (str3);
                 //   g_print("%s\n", str3);
                }
            }
        }
        g_print("%s\n",str1);
//        g_ptr_array_add(res1, "garfeild_vs_falg");
//        g_ptr_array_add(res1, "Привет!");
        rc = sqlite3_exec(db, str1, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }
        g_free(str1);
        //Копируем полученный массив в запасной tempRes и очищаем старый.
        //for(i=0; i<res1->len; i++)
            //g_ptr_array_add(tempRes, g_ptr_array_index(res1, i));

        for(i=0; i<4; i++)
            g_print("res1:\t%s\n", (gchar*)g_ptr_array_index(res1, i));
        g_print("len: %d\n", res1->len);
        //for(i=0; i<tempRes->len; i++)
            //g_print("tempRes:\t%s\n", (gchar*)g_ptr_array_index(tempRes, i));
        //for(i=0; i<tempRes2->len; i++)
            //g_print("tempRes2:\t%s\n", (gchar*)g_ptr_array_index(tempRes2, i));
       
        //g_ptr_array_free(res1, TRUE);
        //res1 = g_ptr_array_new();
        
        g_print("%s\n",str2);
//        g_ptr_array_add(res1, "garfeild_vs_falg");
        rc = sqlite3_exec(db, str2, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }
        g_free(str2);
/*        //Сравниваем i-ый элемент массива res1 и j-ый элемент массива tempRes
        for(i=0; i<res1->len; i++)
        {
            for(j=0; j<tempRes->len; j++)
            {
                if ( g_strcmp0( g_ptr_array_index(res1, i), g_ptr_array_index(tempRes, j) ) == 0 )
                {   
                    //Задаем FALSE, т.к. не знаем еще есть ли значение res1 в tempRes2
                    duplex = FALSE;
                    //Если во втором запасном массиве tempRes2 что-то есть, то проверяем, есть ли значение i-го элемента res1 в tempRes2
                    if ( tempRes2->len != 0 )
                    {
                        for( k=0; k<tempRes2->len; k++ )
                        {
                            if ( g_strcmp0( g_ptr_array_index(tempRes2, k), g_ptr_array_index(res1, i) ) == 0 )
                                duplex = TRUE;
                            
                            if ( duplex == FALSE )
                                g_ptr_array_add(tempRes2, g_ptr_array_index(res1, i));
                        }
                    }
                    else
                        g_ptr_array_add(tempRes2, g_ptr_array_index(res1, i));
                }
            }
        }

        for(i=0; i<res1->len; i++)
            g_print("res1:\t%s\n", (gchar*)g_ptr_array_index(res1, i));
        for(i=0; i<tempRes->len; i++)
            g_print("tempRes:\t%s\n", (gchar*)g_ptr_array_index(tempRes, i));
        for(i=0; i<tempRes2->len; i++)
            g_print("tempRes2:\t%s\n", (gchar*)g_ptr_array_index(tempRes2, i));
        
        g_ptr_array_free(tempRes, TRUE);
        tempRes = g_ptr_array_new();
        g_ptr_array_free(res1, TRUE);
        res1 = g_ptr_array_new();

        for(i=0; i<tempRes2->len; i++)
            g_ptr_array_add(tempRes, g_ptr_array_index(tempRes2, i));
        tempRes2 = g_ptr_array_new();
*/
        //g_ptr_array_free(res1, TRUE);
        //res1 = g_ptr_array_new();
        
        for(i=0; i<4; i++)
            g_print("res1:\t%s\n", (gchar*)g_ptr_array_index(res1, i));
        g_print("len: %d\n", res1->len);
        
        g_print("%s\n",str3);
//        g_ptr_array_add(res1, "garfeild_vs_falg");
        rc = sqlite3_exec(db, str3, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }
        g_free(str3);

        for(i=0; i<4; i++)
            g_print("res1:\t%s\n", (gchar*)g_ptr_array_index(res1, i));
        g_print("len: %d\n", res1->len);
        
        g_print("%s\n",str4);
//        g_ptr_array_add(res1, "garfeild_vs_falg");
        rc = sqlite3_exec(db, str4, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }
        g_free(str4);

        for(i=0; i<4; i++)
            g_print("res1:\t%s\n", (gchar*)g_ptr_array_index(res1, i));
        g_print("len: %d\n", res1->len);
        
        g_print("%s\n",str5);
//        g_ptr_array_add(res1, "garfeild_vs_falg");
        rc = sqlite3_exec(db, str5, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }
        g_free(str5);

        g_print("%s\n",str6);
//        g_ptr_array_add(res1, "garfeild_vs_falg");
        rc = sqlite3_exec(db, str6, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }
        g_free(str6);

        g_print("End\n");


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

void set_column_xu(GtkWidget *tree, const char *labelColumn[], int size)
{
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;
  int i;
 
  for ( i=0; i < size; i++ )
  {
 
          renderer = gtk_cell_renderer_text_new ();
          column = gtk_tree_view_column_new_with_attributes (labelColumn[i], renderer,
                  "text", i,
                  NULL);
 
      gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
  }
}
 
/*void set_table_info(GPtrArray *results)
{
    GtkTreeIter iter;
 
    gtk_tree_store_append (store, &iter, NULL); 
    gtk_tree_store_set (store, &iter,
            MP_NAME, g_ptr_array_index(results, MP_NAME),
            CORE_PERF, g_ptr_array_index(results ,CORE_PERF),                //1
            CORE_DIGIT,  g_ptr_array_index(results, CORE_DIGIT),            //2
            COUNT_TIMERS, g_ptr_array_index(results, COUNT_TIMERS),          //3
            ASYNC_PORT_TYPE, g_ptr_array_index(results, ASYNC_PORT_TYPE),   //4
            CASE_TYPE, g_ptr_array_index(results, CASE_TYPE),               //5
            GUARD_TIMER, g_ptr_array_index(results, GUARD_TIMER),           //6
            INTERFACE_RAM, g_ptr_array_index(results, INTERFACE_RAM),       //7
            INTERFACE_DEBUG, g_ptr_array_index(results, INTERFACE_DEBUG),   //8
            DMA, g_ptr_array_index(results, DMA),                           //9
            PLL, g_ptr_array_index(results, PLL),                           //0
            ADC_DIGIT, g_ptr_array_index(results, ADC_DIGIT),               //1
            ADC_CHANNELS, g_ptr_array_index(results, ADC_CHANNELS),         //2
            ADC_PERF, g_ptr_array_index(results, ADC_PERF),                 //3
            RAM_COMMAND, g_ptr_array_index(results, RAM_COMMAND),           //4
            RAM_DATA, g_ptr_array_index(results, RAM_DATA),                 //5
//            OTHER, g_ptr_array_index(results, OTHER),                       //6
            -1);
}*/

/*void set_table_info(GtkTreeStore *store, const char *names[], gboolean flag)
{
    GtkTreeIter iter;
 
    gtk_tree_store_append (store, &iter, NULL);
    gtk_tree_store_set (store, &iter,
            CORE_PERF, names[CORE_PERF],                //1
            CORE_DIGIT,  names[CORE_DIGIT],             //2
            COUNT_TIMERS, names[COUNT_TIMERS],          //3
            ASYNC_PORT_TYPE, names[ASYNC_PORT_TYPE],    //4
            CASE_TYPE, names[CASE_TYPE],                //5
            GUARD_TIMER, names[GUARD_TIMER],            //6
            INTERFACE_RAM, names[INTERFACE_RAM],        //7
            INTERFACE_DEBUG, names[INTERFACE_DEBUG],    //8
            DMA, names[DMA],                            //9
            PLL, names[PLL],                            //0
            ADC_DIGIT, names[ADC_DIGIT],                //1
            ADC_CHANNELS, names[ADC_CHANNELS],          //2
            ADC_PERF, names[ADC_PERF],                  //3
            RAM_COMMAND, names[RAM_COMMAND],            //4
            RAM_DATA, names[RAM_DATA],                  //5
            OTHER, names[OTHER],                        //6
            -1);
}
*/

GtkWidget* setup_table_xu(GtkTreeStore *store, const gchar *labelColumn[], int size)
{
  GtkWidget *tree;
 
  tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
 
  set_column_xu(tree, labelColumn, size);
 
  g_object_unref (G_OBJECT (store));
 
  g_print("setup\n");
  return tree;
 
}


/***********************************************************************/
GtkWidget* tab1 ()
{   
    GtkWidget *table; //таблица для frame`ов
    GtkWidget *table_in; 
    GtkWidget *entry;
    GtkWidget *sch_button, *clr_button;
    GtkWidget *frame;
    GtkWidget *bbox; //button box
    GtkWidget *checkbutton; //enable\disable entries
    GtkWidget *hbox, *vbox; //for ch_boxes
    GtkWidget *combo;
    GtkWidget *notebook;
    GtkWidget *tableOut;
    GtkWidget *scr_window;
    GtkTreeStore *tempStore;
    GPtrArray *array;
    int i,j,m,n,k;
    int size;
    gchar *frame_name[6] = {
        "Входные параметры",
        "Выходные параметры",
        "Рабочие функции",
        "Защиты преобразователя и двигателя", 
        "Защиты от неправильной работы тиристорного выпрямителя" ,
        "Режимы коррекции"};

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
    gchar *korr_name[3] = {"Коррекция выходного напряжения\nв зависимости от напряжения питающей сети",
    "Коррекция интенсивности(при разгоне) и рабочей частоты\n(в установившимся режиме) при превышении тока двигетеля",
    "Коррекция интенсивности торможения при\nпревышении напряжения на звене постоянного тока"};

    gchar *base_korr[3] = {"k_int_i_drive","k_int_torm_U_lim","k_U_out_depend_U_in"};

/*    gchar *sch_filter[6]={"все","входные параметры","выходные параметры","рабочие функции",
    "защитные функции", "ни одного"};*/
    gchar *base_input[3] = {"phase_number","U_in","freq_in"};
    gchar *base_output[11] = {"power_","phase_number","range_reg_U_", "diskr_reg_U_", "range_reg_f_", "diskr_reg_f_",
    "range_reg_v_razsys_", "range_reg_v_zamsys_", "control_way", "mod_way", "takt_f"};
    gchar *base_work[6] = {"zad_fraq_tp","ustan_zad_freq_tp", "command_pusk_stop", "pusk_stop_har",
    "auto_pusk_stop_error", "reg_techn_var"};
    gchar *base_defend2[2] = {"power_leave_MP_defend","teplo_defend"};
    gchar *base_defend4[4] = {"inc_decr_U_in_i_const_zveno","obriv_perekos_phase_defend","defend_from_kz",
    "max_i_defend"};


    const gchar *input_entry_name_table[4]={"Модель", "число фаз","напряжение питающией сети,В", "частота питающей сети,Гц"};

    const gchar *output_entry_name_table[12]={"Модель", "мощность", "число фаз", "диапазон регулирования напряжения", 
    "дискретность регулирования напряжения", "диапазон регулирования частоты", "дискретность регулирования частоты",
    "диапазон регулирования скорости в разомкнутой системе", "диапазон регулирования скорости в замкнутой системе",
    "принцип управления", "способ модуляции", "тактовая частота ШИМ"};
 
    const gchar *work_name_table[7]={"Модель", "Задание частоты или технологической переменной",
    "Установки задания частоты или технологической переменной",
    "Команды пуск/стоп","Пуско-тормозная характеристика",
    "Автоматический повторный пуск при обнаружении ошибки","Регулятор технологической переменной"};

    const gchar *defend1_name_table[5]={"Модель", "Защита от короткого замыкания на корпус",
    "Максимально-токовая защита","Защита от обрыва и перекоса фаз",
    "Защита от понижения или повышения напряжения в звене постоянного тока"};
 
    const gchar *defend2_name_table[3]={"Модель", "Тепловая защита","Защита от потери питания контроллером"};

    const gchar *korr_name_table[4] = {"Модель", "Коррекция выходного напряжения\nв зависимости от напряжения питающей сети",
    "Коррекция интенсивности(при разгоне) и рабочей частоты\n(в установившимся режиме) при превышении тока двигетеля",
    "Коррекция интенсивности торможения при\nпревышении напряжения на звене постоянного тока"};

table = gtk_table_new (15, 3, FALSE);
notebook = gtk_notebook_new();
array = g_ptr_array_new ();
res1 = g_ptr_array_new();
    for (j=0; j<13; j++)
    {
        if (j==0) //input
        {
            frame = gtk_frame_new(NULL);
            checkbutton = gtk_check_button_new_with_label (frame_name[0]);
            g_ptr_array_add (array, (gpointer) checkbutton);
            gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
            gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 0, 1);
            table_in = gtk_table_new (1, 3, FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(table_in), FALSE);
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), FALSE);
            g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                  G_CALLBACK (state_callback), (gpointer) table_in);
            g_signal_emit_by_name(checkbutton, "toggled", (gpointer) table_in);
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
            gtk_widget_set_sensitive(GTK_WIDGET(table_in), FALSE);
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), FALSE);
            g_signal_connect (G_OBJECT (checkbutton), "toggled",
                              G_CALLBACK (state_callback), (gpointer) table_in);
            g_signal_emit_by_name(checkbutton, "toggled", (gpointer) table_in);
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
                g_ptr_array_add (array, (gpointer) checkbutton);
                gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
                gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 5, 7);
                table_in = gtk_table_new (2, 3, FALSE);
                gtk_widget_set_sensitive(GTK_WIDGET(table_in), FALSE);
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), FALSE);
                g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                  G_CALLBACK (state_callback), (gpointer) table_in);
                g_signal_emit_by_name(checkbutton, "toggled", (gpointer) table_in);
                gtk_container_add (GTK_CONTAINER(frame), table_in);
                k=0;
                for(m=0; m<2; m++)
                for(n=0; n<3; n++)
                {
                    hbox = gtk_hbox_new (FALSE, 0);
                    combo = gtk_combo_box_new_text();
                    gtk_widget_set_name (GTK_WIDGET(combo), base_work[k]);
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
            if (j==7) //defend4
            {
                frame = gtk_frame_new(NULL);
                checkbutton = gtk_check_button_new_with_label (defend_title[0]);
         //       gtk_widget_set_name (GTK_WIDGET(checkbutton), defend_title[0]);
                g_ptr_array_add (array, (gpointer) checkbutton);
                gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 7, 9);
                table_in = gtk_table_new (2, 2, FALSE);
                gtk_widget_set_sensitive(GTK_WIDGET(table_in), FALSE);
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), FALSE);
                g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                  G_CALLBACK (state_callback), (gpointer) table_in);
                gtk_container_add (GTK_CONTAINER(frame), table_in);
                k=0;
                for(m=0; m<2; m++)
                for(n=0; n<2; n++)
                {
                    hbox = gtk_hbox_new (FALSE, 0);
                    combo = gtk_combo_box_new_text();
                    gtk_widget_set_name (GTK_WIDGET(combo), base_defend4[k]);
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
                g_ptr_array_add (array, (gpointer) checkbutton);
                gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
                gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 9, 11);
                table_in = gtk_table_new (1, 2, FALSE);
                gtk_widget_set_sensitive(GTK_WIDGET(table_in), FALSE);
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), FALSE);
                g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                  G_CALLBACK (state_callback), (gpointer) table_in);
                gtk_container_add (GTK_CONTAINER(frame), table_in);
                for(m=0; m<2; m++)
                {
                    hbox = gtk_hbox_new (FALSE, 0);
                    combo = gtk_combo_box_new_text();
                    g_ptr_array_add (array, (gpointer) combo);
                    gtk_widget_set_name (GTK_WIDGET(combo), base_defend2[m]);
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

            if (j==11) //korr
            {
                frame = gtk_frame_new(NULL);
                checkbutton = gtk_check_button_new_with_label ("Режимы коррекции");
                g_ptr_array_add (array, (gpointer) checkbutton);
                gtk_frame_set_label_widget(GTK_FRAME(frame), checkbutton);
                gtk_table_attach_defaults (GTK_TABLE (table), frame, 0, 3, 11, 13);
                table_in = gtk_table_new (2, 2, FALSE);
                gtk_widget_set_sensitive(GTK_WIDGET(table_in), FALSE);
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), FALSE);
                g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                  G_CALLBACK (state_callback), (gpointer) table_in);
                gtk_container_add (GTK_CONTAINER(frame), table_in);
                k=0;
                for(m=0; m<2; m++)
                for(n=0; n<2; n++)
                if (n!=1 || m!=1)
                
                {
                    hbox = gtk_hbox_new (FALSE, 0);
                    combo = gtk_combo_box_new_text();
                    g_ptr_array_add (array, (gpointer) combo);
                    gtk_widget_set_name (GTK_WIDGET(combo), base_korr[k]);
                    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "есть");
                    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "нет");
                    checkbutton = gtk_check_button_new_with_label (korr_name[k]);
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
                    g_signal_connect (G_OBJECT (checkbutton), "toggled",
                                      G_CALLBACK (state_callback), (gpointer) combo);
                    gtk_container_add (GTK_CONTAINER(hbox), checkbutton);
                    gtk_container_add (GTK_CONTAINER(hbox), combo);
                    gtk_table_attach_defaults (GTK_TABLE (table_in), hbox, 0+m, 1+m, 0+n, 1+n);
                    k++;
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

/*checkbuttons**************************************************
hbox = gtk_hbox_new(FALSE, 0);

    for (i=0; i<6; i++)
    {
        checkbutton = gtk_check_button_new_with_label (sch_filter[i]);
        gtk_container_add (GTK_CONTAINER (hbox), checkbutton);
    }*/

stores = g_ptr_array_new();

for (i=0; i<6; i++)
{   
    if (i==0)
    {
        tempStore = gtk_tree_store_new(4,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING);
        g_ptr_array_add(stores, tempStore);
        size = 4;
        tableOut = setup_table_xu(tempStore, input_entry_name_table, size);
    }
    else if (i==1)
    {
        tempStore = gtk_tree_store_new(12,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING);
        g_ptr_array_add(stores, tempStore);
        size = 12;
        tableOut = setup_table_xu(tempStore, output_entry_name_table, size);
    }
    else if (i==2)
    {
        tempStore = gtk_tree_store_new(7,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING);
        g_ptr_array_add(stores, tempStore);
        size = 7;
        tableOut = setup_table_xu(tempStore, work_name_table, size);
    }
    else if (i==3)
    {
        size = 5;
        tempStore = gtk_tree_store_new(size,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING);
        g_ptr_array_add(stores, tempStore);
        tableOut = setup_table_xu(tempStore, defend1_name_table, size);
    }
    else if (i==4)
    {
        size = 3;
        tempStore = gtk_tree_store_new(size,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING);
        g_ptr_array_add(stores, tempStore);
        tableOut = setup_table_xu(tempStore, defend2_name_table, size);
    }
    else if (i==5)
    {
        size = 4;
        tempStore = gtk_tree_store_new(size,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING,
                G_TYPE_STRING);
        g_ptr_array_add(stores, tempStore);
        tableOut = setup_table_xu(tempStore, korr_name_table, size);
    }

    scr_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scr_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scr_window), tableOut);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scr_window, gtk_label_new(frame_name[i]));
}


bbox = gtk_hbutton_box_new();
gtk_box_pack_start(GTK_BOX (bbox), clr_button, TRUE, FALSE, 0);
gtk_box_pack_start(GTK_BOX (bbox), sch_button, TRUE, FALSE, 0);
gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
gtk_table_attach_defaults (GTK_TABLE (table), bbox, 2, 3, 13, 14);
gtk_table_attach_defaults(GTK_TABLE(table), notebook, 0, 3, 14, 15);
//gtk_table_attach_defaults (GTK_TABLE (table), hbox, 0, 3, 12, 13);
    
return table;
}
