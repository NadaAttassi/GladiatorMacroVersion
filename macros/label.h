#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Ajout de la biblioth�que pour la fonction strcpy


#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED
// *** Label
typedef struct
{
    GtkWidget* label; // Pointeur sur le widget de label
    gchar *text; // Le texte à afficher
    gchar *name; // Le nom du label
    gint selectable; // 1 Si selectable, Sinon par défaut false
    gint wrap; // 1: TRUE, Sinon par défaut false
    gint height; // Hauteur de label
    gint width; // Longueur de label
    gint x; // Coordonnée x du label
    gint y; // Coordonnée y du label
} LabelObj;


LabelObj* init_label(gchar* text, gchar *nom, gint selec, gint wrap, gint width, gint height, gint x, gint y)
{
    LabelObj *LO = (LabelObj*)malloc(sizeof(LabelObj));

    // Test d'allocation
    if (!LO)
    {
        printf("\nErreur d'allocation!!!!!!\n");
        exit(0);
    }

    // Allocation réussie
    LO->text = NULL;
    LO->name = NULL;

    // Texte
    if (text)
    {
        LO->text = (gchar*)malloc((strlen(text) + 1) * sizeof(gchar));
        if (LO->text)
            strcpy(LO->text, text);
        else
            printf("\nErreur d'allocation\n");
    }

    // Nom
    if (nom)
    {
        LO->name = (gchar*)malloc((strlen(nom) + 1) * sizeof(gchar));
        if (LO->name)
            strcpy(LO->name, nom);
        else
            printf("\nErreur d'allocation\n");
    }

    LO->selectable = selec;
    LO->wrap = wrap;
    LO->height = height;
    LO->width = width;
    LO->x = x; // Coordonnée x
    LO->y = y; // Coordonnée y
    LO->label = NULL;

    return LO;
}


void create_label22(LabelObj *LO)
{
    // Label
    LO->label = gtk_label_new(LO->text);

    // Nom (si disponible)
    if (LO->name != NULL)
        gtk_widget_set_name(GTK_WIDGET(LO->label), LO->name);

    // Selectable
    if (LO->selectable == 1)
        gtk_label_set_selectable(GTK_LABEL(LO->label), TRUE);

    // Wrap
    if (LO->wrap == 1)
    {
        gtk_label_set_line_wrap(GTK_LABEL(LO->label), TRUE);
        gtk_label_set_line_wrap_mode(GTK_LABEL(LO->label), PANGO_WRAP_WORD_CHAR);
    }
}

#endif // label_H_INCLUDED


