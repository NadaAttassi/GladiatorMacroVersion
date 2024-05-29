
#ifndef TEST_GTK_BUTTONSIMPLEHEADER_H
#define TEST_GTK_BUTTONSIMPLEHEADER_H


#include <gtk/gtk.h>

//*********************************************STRUCTURES***********************************************
typedef struct {
    GtkWidget* button; // bouton widget
    gchar* name;       // Nom du bouton
    gchar* label;      // Le texte sur le bouton
    gchar* lien;       // Chemin icone du bouton
    guint width;       // longueur du bouton
    guint height;      // largeur du bouton
    gchar* font;       // Police de caract�res
    gchar* textColor;  // Couleur du texte
    gchar* bgColor;    // Couleur de fond du GtkGrid
    gboolean bold;     // Texte en gras
    gboolean italic;   // Texte en italique
    gint x;            // Position x du bouton
    gint y;            // Position y du bouton
} ButtonSimple;

//************************************************PROTOTYPES********************************************************
//button simple
/*1*/ButtonSimple* init_button_simple(gchar* nom, gchar* etiq, gchar* lien, gint h, gint w, gchar* font,
                                      gchar* textColor, gchar* bgColor, gboolean bold, gboolean italic, gint x, gint y);
/*2*/void creer_button_Simple(ButtonSimple* b);

//************************************BUTTON SIMPLE******************************
// INITIALISER LE BOUTON
ButtonSimple* init_button_simple(gchar* nom, gchar* etiq, gchar* lien, gint h, gint w, gchar* font,
                                 gchar* textColor, gchar* bgColor, gboolean bold, gboolean italic, gint x, gint y) {
    ButtonSimple* b = (ButtonSimple*)malloc(sizeof(ButtonSimple));

    if (!b) {
        printf("\nErreur d'allocation !!\n");
        exit(0);
    }

    b->name = NULL;
    b->label = NULL;
    b->lien = NULL;
    b->font = NULL;
    b->textColor = NULL;
    b->bgColor = NULL;
    b->bold = FALSE;
    b->italic = FALSE;
    b->x = x;
    b->y = y;

    if (etiq) {
        b->label = (gchar*)malloc(30 * sizeof(gchar)); // Allocation fixe
        strcpy(b->label, etiq);
    }

    if (nom) {
        b->name = (gchar*)malloc(30 * sizeof(gchar)); // Allocation fixe
        strcpy(b->name, nom);
    }

    if (lien) {
        b->lien = (gchar*)malloc(30 * sizeof(gchar)); // Allocation fixe
        strcpy(b->lien, lien);
    }

    if (font) {
        b->font = (gchar*)malloc(30 * sizeof(gchar)); // Allocation fixe
        strcpy(b->font, font);
    }

    if (textColor) {
        b->textColor = (gchar*)malloc(30 * sizeof(gchar)); // Allocation fixe
        strcpy(b->textColor, textColor);
    }

    if (bgColor) {
        b->bgColor = (gchar*)malloc(30 * sizeof(gchar)); // Allocation fixe
        strcpy(b->bgColor, bgColor);
    }

    b->width = w;
    b->height = h;
    b->bold = bold;
    b->italic = italic;

    return b;
}
//***************************************************************

// Fonction pour ajouter une couleur de fond � un widget
void add_bgcolor(GtkWidget* widget, const gchar* color, gdouble opacity) {
    GdkRGBA rgba;

    if (gdk_rgba_parse(&rgba, color)) {
        rgba.alpha = opacity;
        gtk_widget_override_background_color(widget, GTK_STATE_NORMAL, &rgba);
    } else {
        g_print("Erreur : Impossible de parser la couleur %s\n", color);
    }
}


//***********************************************************
/*
void on_button_clicked(GtkWidget *widget, gpointer data) {
    // Cr�ation de la bo�te de dialogue
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_OK,
                                    "voulez vous afficher une image?");

    // Affichage de la bo�te de dialogue et attente de la r�ponse
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Destruction de la bo�te de dialogue apr�s qu'elle a �t� ferm�e
    gtk_widget_destroy(dialog);
}*/
// CREER LE BOUTON SIMPLE
void creer_button_Simple(ButtonSimple* b) {
    // Cr�er un GtkGrid pour contenir l'image et le label
    GtkWidget* grid = gtk_grid_new();

    // Changer la couleur de fond de la grille
    if (b->bgColor)
        add_bgcolor(grid, b->bgColor, 1.0); // Utiliser la couleur de fond sp�cifi�e

    // Cr�er le bouton avec le GtkGrid comme contenu
    b->button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(b->button), grid);

    // Cr�er un label pour le texte
    GtkWidget* label = NULL;
    if (b->label)
        label = gtk_label_new(b->label);

    // Cr�er une image si un chemin d'ic�ne est sp�cifi�
    GtkWidget* image = NULL;
    if (b->lien)
        image = gtk_image_new_from_file(b->lien);

    // Ajouter l'image et/ou le label au GtkGrid
    if (image) {
        // D�finir la taille souhait�e de l'image
        int largeur_image = b->width; // Remplacez par la largeur souhait�e en pixels
        int hauteur_image =  b->height; // Remplacez par la hauteur souhait�e en pixels
        gtk_widget_set_size_request(image, largeur_image, hauteur_image);

        // Attacher l'image au GtkGrid
        gtk_grid_attach_next_to(GTK_GRID(grid), image, NULL, GTK_POS_LEFT, 1, 1);
    }

    if (label)
        gtk_grid_attach_next_to(GTK_GRID(grid), label, NULL, GTK_POS_RIGHT, 1, 1);

    // Appliquer le style du texte (si n�cessaire)
    if (label && (b->font || b->textColor || b->bold || b->italic)) {
        PangoAttrList* attrList = pango_attr_list_new();
        PangoAttribute* attr;

        if (b->font) {
            attr = pango_attr_family_new(b->font);
            pango_attr_list_insert(attrList, attr);
        }

        if (b->textColor) {
            GdkRGBA color;
            gdk_rgba_parse(&color, b->textColor);
            attr = pango_attr_foreground_new(color.red * 65535, color.green * 65535, color.blue * 65535);
            pango_attr_list_insert(attrList, attr);
        }

        if (b->bold) {
            attr = pango_attr_weight_new(PANGO_WEIGHT_BOLD);
            pango_attr_list_insert(attrList, attr);
        }

        if (b->italic) {
            attr = pango_attr_style_new(PANGO_STYLE_ITALIC);
            pango_attr_list_insert(attrList, attr);
        }

        gtk_label_set_attributes(GTK_LABEL(label), attrList);
        pango_attr_list_unref(attrList);
    }

    // D�finir le nom du bouton (ID) pour le styliser en CSS
    if (b->name)
        gtk_widget_set_name(b->button, b->name);

    // D�finir la taille du bouton
    if ((b->width > 0) && (b->height > 0))
        gtk_widget_set_size_request(b->button, b->width, b->height);

    // Connexion de l'�v�nement "clicked" � une fonction de rappel
    // g_signal_connect(b->button, "clicked", G_CALLBACK(on_button_clicked), NULL);

}

#endif //TEST_GTK_BUTTONSIMPLEHEADER_H
