
#ifndef TEST_GTK_HELP_H
#define TEST_GTK_HELP_H
#include <gtk/gtk.h>
#include "gtk_utils.h"
#include "Bdialog.h"
#include "windowboxscroll.h"
#include "textHeader.h"
#include "ButtonSimpleheader.h"
void click_OKdialogue(GtkButton *button, gpointer data) {
    DialogObject *dialog = (DialogObject *)data;
    gtk_widget_destroy(dialog->dialog);
}
void display_help_dialog() {
    FILE *file;
    char help_text[1000000];

    // Ouvrir le fichier help.txt pour lecture
    file = fopen("help.txt", "r");
    if (file == NULL) {
        g_print("Error opening file!\n");
        return;
    }

    char buffer[100];
    help_text[0] = '\0';

    // Lire le contenu du fichier dans help_text
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        strcat(help_text, buffer);
    }

    fclose(file);

    // Initialiser l'objet DialogObject
    DialogObject *help_dialog = init_dialog(NULL, "Help", NULL, 1, 400, 600, GTK_WIN_POS_CENTER, "#FFFFFF", 1.0);

    // Créer la boîte de dialogue
    create_dialog(help_dialog);

    // Ajouter un bouton OK à la boîte de dialogue
    ButtonSimple *ok_button = init_button_simple(NULL, "OK", NULL, 10, 200, NULL, NULL, NULL, 1, 0, 0, 0);
    creer_button_Simple(ok_button);

    // Initialiser le GtkTextView pour afficher le texte d'aide
    TextViewObj *text_view_obj = init_text_view(help_text);
    create_text_view(text_view_obj);

    // Initialiser l'objet Scroll avec text_view comme childContainer
    Scroll *scroll = initializeScroll(help_dialog->content_area, text_view_obj->text_view, 10, 400, 600);

    // Créer le scroll et l'ajouter à la boîte de dialogue
    createScroll(scroll);

    // Ajouter le bouton OK à la boîte de dialogue
    ajouter_widget_dialog(help_dialog->dialog, ok_button->button);

    // Connecter le signal de fermeture
     // Connecter le signal "clicked" du bouton OK à la fonction click_OKdialogue
    g_signal_connect(ok_button->button, "clicked", G_CALLBACK(click_OKdialogue), help_dialog);

    // Afficher la boîte de dialogue
    gtk_widget_show_all(help_dialog->dialog);
}

#endif //TEST_GTK_HELP_H
