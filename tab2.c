#include "tab2.h"
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <glib/gprintf.h>
/* garfeild.c */

//Индекс ID
#define ID 0
//Индекс TEXT
#define TEXT 1
#define NUMBER 0

#define ROWS 6
#define COLUMNS 3

#define AVIABLE 0
#define UNAVIABLE 1

sqlite3 *db;
//Названия кнопок
const gchar *nameButton[2] = { "Сброс", "Поиск" };
//Названия для GtkLabel и GtkEntry
const gchar *textForCombobox[2] = { "Есть", "Нет" };


//////////////////////////////////////////////////////////////////////////////
/* old stuff
 * GtkWidget *input[2];
 */
/////////////////////////////////////////////////////////////////////////////


GtkWidget* createEntry(GPtrArray *entries, GPtrArray *checkButtons, const gchar *name)
{
    GtkWidget *_entry;
    GtkWidget *_label;
    GtkWidget *_leftAlign;
    GtkWidget *_vbox;
    
    _entry = gtk_entry_new();
    g_ptr_array_add(entries, _entry);
    
    _label = gtk_check_button_new_with_label(name);
    g_ptr_array_add(checkButtons, _label);

    _leftAlign = gtk_alignment_new(0, 0.5, 0.1, 0);
    
    _vbox = gtk_vbox_new(FALSE, 5);
    
    gtk_container_add(GTK_CONTAINER(_leftAlign), GTK_WIDGET(_label));
    
    gtk_box_pack_start(GTK_BOX(_vbox), _leftAlign, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(_vbox), _entry, FALSE, FALSE, 0);

    return _vbox;
}

GtkWidget* createComboBox(GPtrArray *comboboxes, GPtrArray *checkButtons, const gchar* name)
{
    GtkWidget *_combobox;
    GtkWidget *_label;
    GtkWidget *_leftAlign;
    GtkWidget *_vbox;
    
    _combobox = gtk_combo_box_new_text();
    gtk_combo_box_append_text(GTK_COMBO_BOX(_combobox), textForCombobox[0]); 
    gtk_combo_box_append_text(GTK_COMBO_BOX(_combobox), textForCombobox[1]); 
    g_ptr_array_add(comboboxes, _combobox);
    
    _label = gtk_check_button_new_with_label(name);
    g_ptr_array_add(checkButtons, _label);

    _leftAlign = gtk_alignment_new(0, 0.5, 0.1, 0);
    
    _vbox = gtk_vbox_new(FALSE, 5);
    
    gtk_container_add(GTK_CONTAINER(_leftAlign), GTK_WIDGET(_label));
    
    gtk_box_pack_start(GTK_BOX(_vbox), _leftAlign, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(_vbox), _combobox, FALSE, FALSE, 0);

    return _vbox;
}

GtkWidget* createFrame(GPtrArray *entries, GPtrArray *checkButtons, const gchar name[], const gchar *names[], const int size)
{
    GtkWidget *_frame;
    GtkWidget *_label;
    GtkWidget *_vbox;
    int i;

    _frame = gtk_frame_new(NULL);
    _vbox = gtk_vbox_new(FALSE, 5);
    _label = gtk_check_button_new_with_label(name);
    g_ptr_array_add(checkButtons, _label);
    gtk_frame_set_label_widget(GTK_FRAME(_frame), GTK_WIDGET(_label));
    for ( i=0; i<size; i++ )
    {
        gtk_container_add(GTK_CONTAINER(_vbox), createEntry(entries, checkButtons, names[i]));

    }

    gtk_container_add(GTK_CONTAINER(_frame), GTK_WIDGET(_vbox));
    return _frame;
}

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

/*
void entry_print(GtkWidget *gw, GtkWidget *entry)
{ 
    char *zErrMsg = 0;
    int rc;
    //Массив частей запроса 'SELECT что-то FROM garfeild что-то-еще="значение";'
    const gchar *selectSubStr[4] = { "SELECT ", " FROM garfeild WHERE ", "=\"", "\";" };
    int k = 0;
    
    
    //DEBUG: Проверяем, действительно ли можно вычислить длинну.
    //g_fprintf(stderr, "%d\n",strlen(selectSubStr[0]));
    //g_fprintf(stderr, "%d\n",strlen(nameInput[TEXT]));
    //g_fprintf(stderr, "%d\n",strlen(selectSubStr[1]));
    //g_fprintf(stderr, "%d\n",strlen(gtk_widget_get_name(GTK_WIDGET(entry))));
    //g_fprintf(stderr, "%d\n",strlen(selectSubStr[2]));
    //g_fprintf(stderr, "%d\n",strlen(gtk_entry_get_text(GTK_ENTRY(entry))));
    //g_fprintf(stderr, "%d\n",strlen(selectSubStr[3])); 
    
    //k = длинна строки 'SELECT text FROM garfeild WHERE id="число";'
    //Сравниваем названия виджета entry с элементом массива nameInput, соответствующего ID.
    if ( g_strcmp0(gtk_widget_get_name(GTK_WIDGET(entry)),nameInput[ID]) == 0 )
    {
        //DEBUG: Проверка вхождения в цикл для первого if
        //g_fprintf(stderr, "ID!\n");
        k = strlen(selectSubStr[0]) + 
            strlen(nameInput[TEXT]) + 
            strlen(selectSubStr[1]) + 
            strlen(gtk_widget_get_name(GTK_WIDGET(entry))) + 
            strlen(selectSubStr[2]) + 
            strlen(gtk_entry_get_text(GTK_ENTRY(entry))) + 
            strlen(selectSubStr[3]); 
    }
    //Сравниваем названия виджета entry с элементом массива nameInput, соответствующего TEXT.
    else if ( g_strcmp0(gtk_widget_get_name(GTK_WIDGET(entry)),nameInput[TEXT]) == 0 )
    {
        //DEBUG
        //g_fprintf(stderr, "TEXT!\n");
        k = strlen(selectSubStr[0]) + 
            strlen(nameInput[ID]) + 
            strlen(selectSubStr[1]) + 
            strlen(gtk_widget_get_name(GTK_WIDGET(entry))) + 
            strlen(selectSubStr[2]) + 
            strlen(gtk_entry_get_text(GTK_ENTRY(entry))) + 
            strlen(selectSubStr[3]); 
    }
    //DEBUG: Проверяем, посчиталась ли длинна конечно строки.
    //g_fprintf(stderr, "%d\n", k);
    
    //Создаем указатель str на gchar длинной k. sizeof(gchar) для определения размера памяти под один элемент gchar.
    gchar *str = (gchar*)malloc(k*sizeof(gchar));
    //DEBUG: Проверка на падение после вызова malloc.
    //g_fprintf(stderr, "MALLOC!\n");
    
    //Устанавливаем нулевой размер для всех элементов str.
    memset(str, 0, k*sizeof(char));
    //DEBUG: Проверка на выполнение memset.
    //g_fprintf(stderr, "MEMSET\n");
    
    //Копируем первую часть запроса в конечную строку str.
    strcat(str, selectSubStr[0]);
    //DEBUG: Вывод полученной строки.
    //g_fprintf(stderr, "CAT1: %s\n", str);

    //Проеверяем, какой виджет entry используется, и копируем название другого виджета в str.
    if ( g_strcmp0(gtk_widget_get_name(GTK_WIDGET(entry)),nameInput[ID]) == 0 )
        strcat(str, nameInput[TEXT]);
    else if ( g_strcmp0(gtk_widget_get_name(GTK_WIDGET(entry)),nameInput[TEXT]) == 0 )
        strcat(str, nameInput[ID]);
    //DEBUG: Вывод полученной строки.
    //g_fprintf(stderr, "CAT2: %s\n", str);
    
    //Копируем вторую часть запроса в str.
    strcat(str, selectSubStr[1]);
    //DEBUG: Вывод полученной строки.
    //g_fprintf(stderr, "CAT3: %s\n", str);
    
    //Копируем название entry в запрос str.
    strcat(str, gtk_widget_get_name(GTK_WIDGET(entry)));
    //DEBUG: Вывод полученной строки.
    //g_fprintf(stderr, "CAT4: %s\n", str);
    
    //Копируем третью часть запроса в str.
    strcat(str, selectSubStr[2]);
    //DEBUG: Вывод полученной строки.
    //g_fprintf(stderr, "CAT5: %s\n", str);
    
    //Копируем текст из entry, который используем в качестве параметра запроса.
    strcat(str, gtk_entry_get_text(GTK_ENTRY(entry)));
    //DEBUG: Вывод полученной строки.
    //g_fprintf(stderr, "CAT6: %s\n", str);
    
    //Копируем четвертую, последнюю часть запроса в str.
    strcat(str, selectSubStr[3]);
    //DEBUG: Вывод полученной строки.
    //g_fprintf(stderr, "CAT7: %s\n", str);
    
    //Проверочный вывод полученно строки.
    g_print ("%s\n", str);
    rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        g_fprintf(stderr,"SQL error: %s\n", zErrMsg);
    }
}
*/


/////////////////////////////////////////////////////////////////////////////
/* old stuff
void entry_print2(GtkWidget *gw, gpointer array)
{ 
    char *zErrMsg = 0;
    int rc;
    //Массив частей запроса 'SELECT что-то FROM garfeild что-то-еще="значение";'
    //                                             
    const gchar *selectSubStr[4] = { 
        "SELECT id FROM garfeild WHERE ",   //0
        "=\"",                              //1
        "\" AND ",                          //2
        "\";"};                             //3
    int k = 0;
    
    GtkWidget *entry[2];
    entry[0] = gtk_entry_new();;
    
    //DEBUG: Проверяем, действительно ли можно вычислить длинну.
    g_fprintf(stderr, "%d\n",strlen(selectSubStr[0]));
    g_fprintf(stderr, "%d\n",strlen(selectSubStr[1]));
    g_fprintf(stderr, "%d\n",strlen(selectSubStr[2]));
    g_fprintf(stderr, "%d\n",strlen(selectSubStr[3])); 
    g_fprintf(stderr, "%d\n",strlen(gtk_widget_get_name(GTK_WIDGET(entry[NUMBER]))));
    g_fprintf(stderr, "%d\n",strlen(gtk_entry_get_text(GTK_ENTRY(entry[NUMBER]))));
    g_fprintf(stderr, "%d\n",strlen(gtk_widget_get_name(GTK_WIDGET(entry[TEXT]))));
    g_fprintf(stderr, "%d\n",strlen(gtk_entry_get_text(GTK_ENTRY(entry[TEXT]))));
    
    //k = длинна строки 'SELECT id FROM garfeild WHERE number="число" AND text="текст";'
    //Сравниваем названия виджета entry с элементом массива nameInput, соответствующего ID.
        //DEBUG: Проверка вхождения в цикл для первого if
        //g_fprintf(stderr, "ID!\n");
        k = strlen(selectSubStr[0]) + 
            strlen(gtk_widget_get_name(GTK_WIDGET(entry[NUMBER]))) + 
            2*strlen(selectSubStr[1]) + 
            strlen(gtk_entry_get_text(GTK_ENTRY(entry[NUMBER]))) + 
            strlen(selectSubStr[2]) +
            strlen(gtk_widget_get_name(GTK_WIDGET(entry[TEXT]))) +
            strlen(selectSubStr[3]);
    //DEBUG: Проверяем, посчиталась ли длинна конечно строки.
    g_fprintf(stderr, "%d\n", k);
    */
    /* strcat version

    //Создаем указатель str на gchar длинной k. sizeof(gchar) для определения размера памяти под один элемент gchar.
    gchar *str = (gchar*)malloc(k*sizeof(gchar));
    //DEBUG: Проверка на падение после вызова malloc.
    //g_fprintf(stderr, "MALLOC!\n");
    
    //Устанавливаем нулевой размер для всех элементов str.
     memset(str, 0, k*sizeof(char));
    //DEBUG: Проверка на выполнение memset.
    //g_fprintf(stderr, "MEMSET\n");
    
    //Копируем первую часть запроса в конечную строку str.
    strcat(str, selectSubStr[0]);
    
    strcat(str, gtk_widget_get_name(GTK_WIDGET(entry[NUMBER])));

    strcat(str, selectSubStr[1]);

    strcat(str, gtk_entry_get_text(GTK_ENTRY(entry[NUMBER])));

    strcat(str, selectSubStr[2]);

    strcat(str, gtk_widget_get_name(GTK_WIDGET(entry[TEXT])));

    strcat(str, selectSubStr[1]);

    strcat(str, gtk_entry_get_text(GTK_ENTRY(entry[TEXT])));

    strcat(str, selectSubStr[3]);
    */
    /* old stuff part 2
    gchar *str;

    str = g_strconcat(
            selectSubStr[0], 
            gtk_widget_get_name(GTK_WIDGET(entry[NUMBER])), 
            selectSubStr[1], 
            gtk_entry_get_text(GTK_ENTRY(entry[NUMBER])), 
            selectSubStr[2], 
            gtk_widget_get_name(GTK_WIDGET(entry[TEXT])), 
            selectSubStr[1], 
            gtk_entry_get_text(GTK_ENTRY(entry[TEXT])), 
            selectSubStr[3],
            NULL
            );


    //Проверочный вывод полученно строки.
    g_print ("%s\n", str);
    rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        g_fprintf(stderr,"SQL error: %s\n", zErrMsg);
    }
    g_free(str);
}
*/
/////////////////////////////////////////////////////////////////////////////


void entry_print2(GtkWidget *gw, GPtrArray *array)
{ 
    char *zErrMsg = 0;
    int rc;
    //Массив частей запроса 'SELECT что-то FROM garfeild что-то-еще="значение";'
    const gchar *selectSubStr[4] = { 
        "SELECT id FROM garfeild WHERE ",   //0
        "=\"",                              //1
        "\" AND ",                          //2
        "\";"};                             //3
    //Конечная строка запроса.
    gchar *str;
    //Добавляем необходимые подстроки.
    str = g_strconcat(
            selectSubStr[0], 
            gtk_widget_get_name(GTK_WIDGET(g_ptr_array_index(array ,NUMBER))), 
            selectSubStr[1], 
            gtk_entry_get_text(GTK_ENTRY(g_ptr_array_index(array, NUMBER))), 
            selectSubStr[2], 
            gtk_widget_get_name(GTK_WIDGET(g_ptr_array_index(array, TEXT))), 
            selectSubStr[1], 
            gtk_entry_get_text(GTK_ENTRY(g_ptr_array_index(array, TEXT))), 
            selectSubStr[3],
            NULL
            );

    //Проверочный вывод полученно строки.
    g_print ("%s\n", str);
    rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        g_fprintf(stderr,"SQL error: %s\n", zErrMsg);
    }
    g_free(str);
}

void input_clear(GtkWidget *gw, GPtrArray *input)
{ 
    int i;

    for(i=0; i< input->len; i++)    {
        if( i<10)
            gtk_entry_set_text(GTK_ENTRY(g_ptr_array_index(input, i)), "");
        else if (i>=10 && i<input->len)
            gtk_combo_box_set_active(GTK_COMBO_BOX(g_ptr_array_index(input, i)), -1);
    }
}


/////////////////////////////////////////////////////////////////////////////
/* old stuff
 * void entry_clear(GtkWidget *gw, GtkWidget *entry)
 * {
 *      gtk_entry_set_text(GTK_ENTRY(entry), "");
 * }
 */
/////////////////////////////////////////////////////////////////////////////


void entry_enter(GtkWidget *gw, GtkWidget *button)
{
    g_signal_emit_by_name(G_OBJECT(button), "clicked");
}

GtkWidget* tab2()
{
    GtkWidget *tableBox;
    GtkWidget *button[2];
    GtkWidget *buttonBox;
    GPtrArray *input;
    GPtrArray *checkButtons;
    int i, j;
    
    const gchar *nameEntries[5] = { 
        "Производительность ядра",                              //1 
        "Разрядность ядра",                                     //2
        "Количество программируемых 32-х разрядных таймеров",   //3
        "Тип универсального асинхронного порта",                //4
        "Тип корпуса"                                           //5
    };
    
    const gchar *nameCombobox[5] = {
        "Наличиие сторожевого таймера",                         //1
        "Наличие интерфейса внешней памяти",                    //2
        "Наличие отладочного интерфейса",                       //3
        "Наличие контроллера DMA",                              //4
        "Наличие PLL"                                           //5
    };
    
    const gchar *namesACP[3] = {
        "АЦП: количество разрядов",                             //3
        "АЦП: количество каналов",                              //4
        "АЦП: скорость преобразования/производительность"       //5
    };
    
    const gchar *namesRAM[2] = {
        "Размер памяти команд",                                 //1
        "Размер памяти даннх",                                  //2
    };
   
    //initiating buttons and labels
    buttonBox = gtk_hbutton_box_new();
    //Установка форматирования кнопок:
    gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonBox), GTK_BUTTONBOX_END);
    //g_print("BUTTONS: Created\n");
    
    tableBox = gtk_table_new(ROWS, COLUMNS, FALSE);
    gtk_table_set_row_spacings(GTK_TABLE(tableBox), 5);
    gtk_table_set_col_spacings(GTK_TABLE(tableBox), 5);
    //g_print("TABLEBOX: Created\n");

    input = g_ptr_array_new();
    //g_print("INPUT: Created\n");
    checkButtons = g_ptr_array_new();
    //g_print("CHECKBUTTONS: Created\n");


    for(j=0; j<2; j++)  {
        button[j] = gtk_button_new_with_label(nameButton[j]);
        gtk_widget_set_size_request(button[j], 70, 35);
        //g_print("BUTTON: Created\n");
    }
    
    for (i=0; i<3; i++) {
        if (i==0)   {
            gtk_table_attach_defaults(GTK_TABLE(tableBox), GTK_WIDGET(createFrame(input, checkButtons, "АЦП", namesACP, 3)), 0, 1, 0, 3 );

            gtk_table_attach_defaults(GTK_TABLE(tableBox), GTK_WIDGET(createFrame(input, checkButtons, "RAM", namesRAM, 2)), 0, 1, 3, 5);
        }
        else if(i==1)
        {
            for(j=0; j<5; j++)
                gtk_table_attach_defaults(GTK_TABLE(tableBox), GTK_WIDGET(createEntry(input, checkButtons, nameEntries[j])), i, i+1, j, j+1);
        }
        else if(i==2)
        {
            for(j=0; j<5; j++)
                gtk_table_attach_defaults(GTK_TABLE(tableBox), GTK_WIDGET(createComboBox(input, checkButtons, nameCombobox[j])), i, i+1, j, j+1);
        }
    }
    
    for(j=0; j<2; j++)
        gtk_box_pack_start(GTK_BOX(buttonBox), button[j], FALSE, FALSE, 0);
    gtk_table_attach_defaults(GTK_TABLE(tableBox), buttonBox, 2, 3, 5, 6);
    
    for(j=0; j<input->len; j++) { 
        if (j<10)   {
        g_signal_connect(G_OBJECT(g_ptr_array_index(input, j)), "activate",
                G_CALLBACK(entry_enter), (gpointer) button[1]);
        }
    }
    
    g_signal_connect(G_OBJECT(button[0]), "clicked", 
            G_CALLBACK(input_clear), (gpointer) input);

    g_signal_connect(G_OBJECT(button[1]), "clicked", 
            G_CALLBACK(entry_print2), (gpointer)input);
    
   gtk_container_set_border_width(GTK_CONTAINER(tableBox), 5);
   return tableBox;
}
