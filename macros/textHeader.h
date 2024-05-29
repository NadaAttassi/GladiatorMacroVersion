#ifndef TEXTHEADER_H_INCLUDED
#define TEXTHEADER_H_INCLUDED
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    GtkWidget *text_view;
    gchar *text;
} TextViewObj;

TextViewObj* init_text_view(const gchar *text) {
    TextViewObj *tv_obj = malloc(sizeof(TextViewObj));
    if (!tv_obj) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    tv_obj->text = NULL;
    if (text) {
        tv_obj->text = g_strdup(text);
    }

    return tv_obj;
}

void create_text_view(TextViewObj *tv_obj) {
    if (!tv_obj) return;

    tv_obj->text_view = gtk_text_view_new();
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_obj->text_view));
    if (tv_obj->text) {
        gtk_text_buffer_set_text(buffer, tv_obj->text, -1);
    }
        // Désactiver l'édition du texte dans le widget GtkTextView
    gtk_text_view_set_editable(GTK_TEXT_VIEW(tv_obj->text_view), FALSE);
}



#endif // TEXTHEADER_H_INCLUDED
