/***************************************************************************
 *   NAME version VERSION                                                   *
 *   Copyright (C) 2007-2009 by Anton "Garfeild" Kolchunov                 *
 *   kolchunovad (at) gmail.com or garfeild.ubuntu (at) gmail.com          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

//#include <gtk/gtk.h>
//#include <glib.h>
//#include <glib/gprintf.h>
#include "gui.h"
/* gui.c */

void toggle_action(GtkWidget *checkButton, GtkWidget *widget)
{
    gboolean val;
    val = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkButton));
    if( val == TRUE && !GTK_WIDGET_SENSITIVE(widget) )
    {
        gtk_widget_set_sensitive(GTK_WIDGET(widget), TRUE);
    }
    else if (val == FALSE)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(widget), FALSE);
    }


}

GtkWidget* createEntry(GPtrArray *entries, GPtrArray *all, const gchar *name, const gchar *wname)
{
    GtkWidget *_entry;
    GtkWidget *_label;
    GtkWidget *_leftAlign;
    GtkWidget *_vbox;
    
    _entry = gtk_entry_new();
    gtk_widget_set_name(GTK_WIDGET(_entry), wname);
    g_ptr_array_add(entries, _entry);
    g_ptr_array_add(all, _entry);
    
    _label = gtk_check_button_new_with_label(name);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(_label), TRUE);

    _leftAlign = gtk_alignment_new(0, 0.5, 0.1, 0);
    
    _vbox = gtk_vbox_new(FALSE, 5);
    
    gtk_container_add(GTK_CONTAINER(_leftAlign), GTK_WIDGET(_label));
    
    //gtk_box_pack_start(GTK_BOX(_vbox), _leftAlign, TRUE, TRUE, 0);
    //gtk_box_pack_start(GTK_BOX(_vbox), _entry, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(_vbox), _leftAlign);
    gtk_container_add(GTK_CONTAINER(_vbox), _entry);

    g_signal_connect(G_OBJECT(_label), "toggled",
            G_CALLBACK(toggle_action), (gpointer) _entry);
    return _vbox;
}

GtkWidget* createComboBox(GPtrArray *comboboxes, GPtrArray *all, const gchar *name, const gchar *wname)
{
    GtkWidget *_combobox;
    GtkWidget *_label;
    GtkWidget *_leftAlign;
    GtkWidget *_vbox;
    const gchar *textForCombobox[2] = { "Есть", "Нет" };
    
    _combobox = gtk_combo_box_new_text();
    gtk_widget_set_name(GTK_WIDGET(_combobox), wname);
    gtk_combo_box_append_text(GTK_COMBO_BOX(_combobox), textForCombobox[0]); 
    gtk_combo_box_append_text(GTK_COMBO_BOX(_combobox), textForCombobox[1]); 
    g_ptr_array_add(comboboxes, _combobox);
    g_ptr_array_add(all, _combobox);
    
    _label = gtk_check_button_new_with_label(name);

    _leftAlign = gtk_alignment_new(0, 0.5, 0.1, 0);
    
    _vbox = gtk_vbox_new(FALSE, 5);
    
    gtk_container_add(GTK_CONTAINER(_leftAlign), GTK_WIDGET(_label));
    
    //gtk_box_pack_start(GTK_BOX(_vbox), _leftAlign, FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(_vbox), _combobox, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(_vbox), _leftAlign);
    gtk_container_add(GTK_CONTAINER(_vbox), _combobox);

    g_signal_connect(G_OBJECT(_label), "toggled",
            G_CALLBACK(toggle_action), (gpointer) _combobox);

    g_signal_emit_by_name(_label, "toggled", (gpointer)_combobox);
    
    return _vbox;
}

GtkWidget* createFrame(GPtrArray *entries, GPtrArray *all, const gchar name[], const gchar *names[], const gchar *wnames[], const int size)
{
    GtkWidget *_frame;
    GtkWidget *_label;
    GtkWidget *_vbox;
    int i;

    _frame = gtk_frame_new(NULL);
    _vbox = gtk_vbox_new(FALSE, 5);
    _label = gtk_check_button_new_with_label(name);
    g_ptr_array_add(all, _label);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(_label), TRUE);
    gtk_frame_set_label_widget(GTK_FRAME(_frame), GTK_WIDGET(_label));
    for ( i=0; i<size; i++ )
    {
        gtk_container_add(GTK_CONTAINER(_vbox), createEntry(entries, all, names[i], wnames[i]));

    }

    gtk_container_add(GTK_CONTAINER(_frame), GTK_WIDGET(_vbox));
    gtk_container_set_border_width(GTK_CONTAINER(_frame), 5);
    
    g_signal_connect(G_OBJECT(_label), "toggled",
            G_CALLBACK(toggle_action), (gpointer) _vbox);
    return _frame;
}
