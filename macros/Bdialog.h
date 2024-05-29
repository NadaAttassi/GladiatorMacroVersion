#ifndef TEST_GTK_BDIALOG_H
#define TEST_GTK_BDIALOG_H


#include <gtk/gtk.h>
typedef struct {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWindow *transient;
    gchar *title;
    gchar *icon;
    gint modal;
    gint height;
    gint width;
    gint position;

    struct {
        gchar *couleur;
        gfloat degre;
    } bgcolor;
} DialogObject;

DialogObject *init_dialog(GtkWindow *parent, gchar* titre, gchar* icon, gint modal, gint height, gint width, gint pos, gchar* color, gfloat color_opc) {
    DialogObject *DO = (DialogObject*)malloc(sizeof(DialogObject));
    if (!DO) {
        printf("\nErreur d'allocation!!!!!!\n");
        exit(0);
    }

    DO->icon = NULL;
    DO->title = NULL;
    DO->bgcolor.couleur = NULL;

    if (titre) {
        DO->title = (gchar*)malloc(30 * sizeof(gchar));
        if (DO->title) {
            strcpy(DO->title, titre);
        } else {
            printf("\nErreur d'allocation\n");
        }
    }

    if (icon) {
        DO->icon = (gchar*)malloc(30 * sizeof(gchar));
        if (DO->icon) {
            strcpy(DO->icon, icon);
        } else {
            printf("\nErreur d'allocation\n");
        }
    }

    if (color) {
        DO->bgcolor.couleur = (gchar*)malloc(30 * sizeof(gchar));
        if (DO->bgcolor.couleur) {
            strcpy(DO->bgcolor.couleur, color);
            DO->bgcolor.degre = color_opc;
        } else {
            printf("\nErreur d'allocation\n");
        }
    }

    DO->transient = parent;
    DO->modal = modal;
    DO->height = height;
    DO->width = width;
    DO->position = pos;

    DO->content_area = NULL;
    DO->dialog = NULL;
    return DO;
}

void create_dialog(DialogObject *DO) {
    DO->dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(DO->dialog), DO->title);
    if (DO->icon != NULL) {
        gtk_window_set_icon_from_file(GTK_WINDOW(DO->dialog), DO->icon, NULL);
    }
    DO->content_area = gtk_dialog_get_content_area(GTK_DIALOG(DO->dialog));
    gtk_window_set_transient_for(GTK_WINDOW(DO->dialog), GTK_WINDOW(DO->transient));
    gtk_widget_set_size_request(GTK_WIDGET(DO->dialog), DO->width, DO->height);
    if (DO->modal == 1) {
        gtk_window_set_modal(GTK_WINDOW(DO->dialog), TRUE);
    }
}

void ajouter_widget_dialog(GtkWidget* dialog, GtkWidget* widget) {
    GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content_area), widget);
    gtk_widget_show(widget);
}
#endif
