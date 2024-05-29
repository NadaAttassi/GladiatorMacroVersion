
#include <gtk/gtk.h>

#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED



typedef struct {
    GtkWidget *window; // Pointeur vers le widget de la fen�tre
    gchar *titre; // Titre de la fen�tre
    gchar *icon; // Chemin vers l�ic�ne de la fen�tre
    gchar*name;
    gchar* bgcolor;
    guint hauteur;
    guint largeur;
    gint posx;
    gint posy;
    gint position_init;// Position initiale de la fen�tre
    guint border_width;
    gboolean modifiable; // Indique si la taille de la fen�tre peut �tre modifi�e
    // Ajoutez d'autres champs si n�cessaire
} Fenetre;

Fenetre* initialiser_win(gchar* titre, gchar* icon_path,gchar *name,gchar*bgcolor, guint h, guint w, gint pos, gint x, gint y,guint border, gboolean resizable)
{
    Fenetre* win = malloc(sizeof(Fenetre));
    if (!win) {
        printf("\nErreur d'allocation de m�moire.");
        exit(-1);
    }

    win->window = NULL; // Initialisez � NULL avant de cr�er la fen�tre

    // Copiez le titre
    if (titre) {
        win->titre=malloc(50*sizeof(gchar));

        win->titre = g_strdup(titre);

    } else {
        win->titre = NULL;
    }

    // Copiez le chemin de l'ic�ne
    if (icon_path) {
        win->icon=malloc(200*sizeof(gchar));
        win->icon = g_strdup(icon_path);
    } else {
        win->icon = NULL;
    }
    //Copier le nom de la fenetre

    if (name) {
        win->name=malloc(25*sizeof(gchar));
        win->name = g_strdup(name);
    } else {
        win->name = NULL;
    }
    if (bgcolor) {
        win->bgcolor=malloc(20*sizeof(gchar));
        win->bgcolor = g_strdup(bgcolor);
    } else {
        win->bgcolor = NULL;
    }
    win->hauteur = h;
    win->largeur = w;
    win->position_init = pos;
    win->posx = x;
    win->posy = y;
    win->border_width=border;
    win->modifiable = resizable;

    return win;
}

void create_window_n(Fenetre *W) {
    // Cr�ez la fen�tre
    W->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // Ajoutez le titre s'il existe
    if (W->titre) {

        gtk_window_set_title(GTK_WINDOW(W->window), W->titre);
    }

    // Ajoutez l'ic�ne s'il existe
    if (W->icon) {
        gtk_window_set_icon_from_file(GTK_WINDOW(W->window), W->icon, NULL);
    }
    //Ajout de couleur de fond
    if(W->bgcolor)
    {
        GdkRGBA color;
        // Utilisez gdk_rgba_parse pour d�finir la couleur de fond de la fen�tre
        if (gdk_rgba_parse(&color,W->bgcolor)) {
            gtk_widget_override_background_color(W->window, GTK_STATE_NORMAL, &color);
        } else {
            g_warning("\nImpossible de d�finir la couleur de fond.");
        }
    }

    // Si la hauteur et la largeur sont sup�rieures � 0, configurez la taille par d�faut
    if (W->hauteur > 0 && W->largeur > 0) {
        gtk_window_set_default_size(GTK_WINDOW(W->window), W->largeur, W->hauteur);
    }

    // Configurez la position initiale de la fen�tre
    switch (W->position_init) {
        case 1:
            gtk_window_set_position(GTK_WINDOW(W->window), GTK_WIN_POS_NONE);
            break;
        case 2:
            gtk_window_set_position(GTK_WINDOW(W->window), GTK_WIN_POS_CENTER);
            break;
        case 3:
            gtk_window_set_position(GTK_WINDOW(W->window), GTK_WIN_POS_MOUSE);
            break;
        case 4:
            gtk_window_set_position(GTK_WINDOW(W->window), GTK_WIN_POS_CENTER_ALWAYS);
            break;
        case 5:
            gtk_window_set_position(GTK_WINDOW(W->window), GTK_WIN_POS_CENTER_ON_PARENT);
            break;
        default:
            gtk_window_move(GTK_WINDOW(W->window), W->posx, W->posy);
            break;
    }
    //Definir l'epaisseur de la bordure
    gtk_container_set_border_width(GTK_CONTAINER(W->window),W->border_width);

    // Si redimensionnable, autorisez le redimensionnement
    if (W->modifiable) {
        gtk_window_set_resizable(GTK_WINDOW(W->window), TRUE);
    } else {
        gtk_window_set_resizable(GTK_WINDOW(W->window), FALSE);
    }
}

Fenetre* add_window()
{
    Fenetre *ma_fenetre = initialiser_win("Instagram", NULL, "F1", "#0000", 3840, 2160, 2, 0, 0, 1, TRUE);
    create_window_n(ma_fenetre);
    return((Fenetre*)ma_fenetre);
}

#endif // WINDOW_H_INCLUDED
