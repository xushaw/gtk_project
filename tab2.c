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

sqlite3 *db;
//Названия кнопок
const gchar *nameButton[2] = { "Сброс", "Поиск" };
//Названия для GtkLabel и GtkEntry
const gchar *nameInput[2] = { "id", "text" };

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
       //Задаем имя для виджета GtkEntry таким же, как и текст соседнего GtkLabel. 
       gtk_widget_set_name(input[j], nameInput[j]);
       
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
