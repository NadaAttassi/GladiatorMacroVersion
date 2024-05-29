
#ifndef TEST_GTK_CREATECHARACTER_H
#define TEST_GTK_CREATECHARACTER_H



#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "arena.h"
#include "windowboxscroll.h"
#include "Gridheader.h"
#include "label.h"
#include "ButtonSimpleheader.h"
#include "EntryHeader.h"
#include "imageHeader.h"
#include "Bdialog.h"
#include "window.h"
#define TOTAL_POINTS 200
#define NUM_PlayerISTICS 4
// Create player data

Box *photo_area ;// D�claration de la variable globale
imageObj *current_photo_widget;
Actor *selected_player ;
Actor *player ;

typedef struct {
    GtkWidget *sliders[NUM_PlayerISTICS];
    LabelObj *remaining_points_label;
    int remaining_points;
} PlayerOptions;

// Structure to hold player data


//Machine *machine = NULL;
typedef struct {
    GtkWidget *entry;
    Fenetre *window;
} CallbackData;

typedef struct {
    const gchar *name;
    int value;
} Playeristic;

typedef struct {
    gchar* grp;
    gchar* name;
} infos;

GList *button_groups = NULL;

typedef struct {
    const gchar *ancienNom;
    gchar *nouveauNom;
} Noms;

Noms *noms = NULL;
PlayerOptions options;
void apply_label_style(GtkWidget *label) {
    PangoAttrList* attrList = pango_attr_list_new();
    PangoAttribute* attr;

    attr = pango_attr_family_new("arial");
    pango_attr_list_insert(attrList, attr);

    attr = pango_attr_size_new(16 * PANGO_SCALE);
    pango_attr_list_insert(attrList, attr);

    GdkRGBA color;
    gdk_rgba_parse(&color, "#ffd55c");
    attr = pango_attr_foreground_new(color.red * 65535, color.green * 65535, color.blue * 65535);
    pango_attr_list_insert(attrList, attr);

    gtk_label_set_attributes(GTK_LABEL(label), attrList);
    pango_attr_list_unref(attrList);
}

void click_dialogue(GtkButton *button, gpointer data) {
     DialogObject *dialog = (DialogObject *)data;
    gtk_dialog_response(dialog->dialog, GTK_RESPONSE_OK);
}

void on_ok_edit_name(GtkButton *button, gpointer data) {
    // Émettre une réponse de dialogue "OK"
    gtk_dialog_response(GTK_DIALOG(gtk_widget_get_parent(gtk_widget_get_parent(button))), GTK_RESPONSE_OK);
}

void modifier(GtkButton *button, gpointer data) {
    // Récupérer le label et son ancien nom
    LabelObj* label = (LabelObj *)data;
    const gchar *ancienNom = label->label;

    // Créer une copie de l'ancien nom pour éviter les problèmes de mémoire
    gchar *copieAncienNom = g_strdup(ancienNom);

    // Créer une boîte de dialogue pour modifier le nom
    DialogObject *dialog = init_dialog(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                       "Modifier le nom", NULL, 1, 200, 150, GTK_WIN_POS_CENTER, "white", 0.8);
    create_dialog(dialog);

    // Créer une entrée pré-remplie avec l'ancien nom
    EntryObj* entry = init_entry(copieAncienNom, NULL, 1, 0, 10, 40, 0, 14);
    create_entry(entry);
    ajouter_widget_dialog(dialog->dialog, entry->entry);

   // Création des boutons OK et Annuler
    ButtonSimple* ok_button = init_button_simple(NULL, "OK", NULL, 10, 200, NULL, NULL, NULL, 1, 0, 0, 0);
    creer_button_Simple(ok_button);

    ButtonSimple* cancel_button = init_button_simple(NULL, "Annuler", NULL, 10, 200, NULL, NULL, NULL, 1, 0, 0, 0);
    creer_button_Simple(cancel_button);

    // Ajout des boutons à la boîte de dialogue
    ajouter_widget_dialog(dialog->dialog, ok_button->button);
    ajouter_widget_dialog(dialog->dialog, cancel_button->button);

    // Connectez les signaux "clicked" des boutons à des fonctions appropriées

        g_signal_connect(ok_button->button, "clicked", G_CALLBACK(on_ok_edit_name), NULL);
        g_signal_connect(cancel_button->button, "clicked", G_CALLBACK(click_dialogue), dialog);

    // Afficher la boîte de dialogue
    gtk_widget_show_all(dialog->dialog);

    // Attendre la réponse de l'utilisateur
    gint result = gtk_dialog_run(GTK_DIALOG(dialog->dialog));

    // Si l'utilisateur appuie sur OK, mettre à jour le label avec le nouveau nom
    if (result == GTK_RESPONSE_OK) {


        const gchar *nouveauNom =  get_entry_text(entry->entry);


           gtk_label_set_text(label->label, nouveauNom);

        // Assurez-vous de libérer la mémoire de l'ancien nom s'il a été alloué dynamiquement
        g_free(copieAncienNom);

        // Mettre à jour le nom du joueur si nécessaire
        if (player->name) {
            g_free(player->name);
        }
        player->name = g_strdup(nouveauNom);
    }

    // Détruire la boîte de dialogue et libérer la mémoire
    gtk_widget_destroy(dialog->dialog);
    free(dialog->title);
    free(dialog->icon);
    free(dialog->bgcolor.couleur);
    free(dialog);
}


void update_remaining_points_label(PlayerOptions *options) {
    gchar label_text[64];
    g_snprintf(label_text, sizeof(label_text), "Remaining Points: %d", options->remaining_points);
    gtk_label_set_text(options->remaining_points_label->label, label_text);
}

void on_slider_value_changed(GtkRange *range, gpointer user_data) {
    PlayerOptions *options = (PlayerOptions *)user_data;
    int total_allocated = 0;
    printf("\nEntered on_slider_value_changed");

    // Calculate the total allocated points
    for (int i = 0; i < NUM_PlayerISTICS; ++i) {
        total_allocated += (int)gtk_range_get_value(GTK_RANGE(options->sliders[i]));
    }
    printf("\nTotal allocated points: %d", total_allocated);

    // Update remaining points
    options->remaining_points = TOTAL_POINTS - total_allocated;
    printf("\nRemaining points: %d", options->remaining_points);
    update_remaining_points_label(options);

    // Adjust sliders based on remaining points
    int adjustment = options->remaining_points < 0 ? -options->remaining_points : 0;
    if (adjustment != 0) {
        printf("\nAdjusting sliders by: %d", adjustment);
    }

    for (int i = 0; i < NUM_PlayerISTICS; ++i) {
        double value = gtk_range_get_value(GTK_RANGE(options->sliders[i])) - adjustment;
        gtk_range_set_value(GTK_RANGE(options->sliders[i]), value);
    }
    printf("\nSliders adjusted");

    // Ensure player is allocated before updating
    if (player == NULL) {
        fprintf(stderr, "\nError: player is NULL. Allocating memory now.");
        player = (Actor *)malloc(sizeof(Actor));
        if (player == NULL) {
            fprintf(stderr, "\nFailed to allocate memory for player");
            exit(1);
        }
        // Optionally initialize player fields to default values
        player->health = 0;
        player->Attack_P = 0;
        player->Defense_d = 0;
//        player->speed = 0;
    }

    // Mettre � jour les caract�ristiques du joueur � partir des curseurs
    player->health = (int)gtk_range_get_value(GTK_RANGE(options->sliders[2]));
    player->Attack_P = (int)gtk_range_get_value(GTK_RANGE(options->sliders[1]));
    player->Defense_d = (int)gtk_range_get_value(GTK_RANGE(options->sliders[0]));
    player->speed = (int)gtk_range_get_value(GTK_RANGE(options->sliders[3]));
    printf("\nPlayer stats updated: name:%s PV=%d, Attack=%d, Defense=%d,speed=%d",
           player->name,player->health, player->Attack_P, player->Defense_d,player->speed);


}

GtkWidget* create_slider(PlayerOptions *options) {
    GtkWidget *slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, TOTAL_POINTS, 1);
    g_signal_connect(slider, "value-changed", G_CALLBACK(on_slider_value_changed), options);
    return slider;
}

void load_css2() {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    GError *error = NULL;

    if (!gtk_css_provider_load_from_path(provider, "Gladiator/style.css", &error)) {
        g_warning("Failed to load CSS file: %s", error->message);
        g_error_free(error);
        return;
    }
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

// Fonction pour charger la grande photo du personnage s�lectionn�
void load_Player_photo(const gchar *photo_path) {
    // Charger la nouvelle photo dans le widget existant
    gtk_image_set_from_file(current_photo_widget->image, photo_path);
}
void Player_thumbnail_clicked(GtkWidget *widget, Actor *data) {
    selected_player = (Actor *)data;
    player = data;
    player->image = data->image;
    player->sprite_sheet = data->sprite_sheet;

    // Afficher les informations du joueur sélectionné
    printf("Nom : %s\n", selected_player->name);
    printf("Chemin d'image : %s\n", selected_player->sprite_sheet);
    printf("PV du joueur : %d\n", selected_player->health);
    printf("Attaque du joueur : %d\n", selected_player->Attack_P);
    printf("Défense du joueur : %d\n", selected_player->Defense_d);
    printf("Vitesse du joueur : %d\n", selected_player->speed);

    // Charger la photo du joueur sélectionné
    load_Player_photo(player->image);

    // Mettre à jour les sliders en fonction des attributs du joueur
    for (int i = 0; i < 4; i++) {
        gtk_range_set_value(GTK_RANGE(options.sliders[i]), 0);

    }
}


GtkWidget* create_Player_thumbnails(Actor players[4], int num_Players) {
    // Cr�er un conteneur pour les miniatures

  Box *thumbnails_container = initializeBox("thumbnails_container-box", 'v', 1, 0, NULL, 1);
    createBox(thumbnails_container);
    // Ajouter des boutons de miniature pour chaque personnage
    for (int i = 0; i < num_Players; i++) {
        // Cr�er un bouton avec une �tiquette pour le nom du personnage
        // Création d'un bouton simple
        ButtonSimple* button = init_button_simple(NULL, players[i].name,NULL, 10,10, "arial", NULL,"", FALSE, TRUE, 1600, 400);
        creer_button_Simple(button);

        // Connecter le signal "clicked" du bouton � la fonction de rappel Player_thumbnail_clicked
        g_signal_connect(button->button, "clicked", G_CALLBACK(Player_thumbnail_clicked), &players[i]);

        // Ajouter une marge en bas de chaque miniature
        gtk_widget_set_margin_bottom(button, 5);

        // Ajouter le bouton � la liste des miniatures
         addToBox(thumbnails_container->box, button->button, 0, FALSE, FALSE, FALSE);
    }

    return thumbnails_container;
}






#endif //TEST_GTK_CREATECHARACTER_H
