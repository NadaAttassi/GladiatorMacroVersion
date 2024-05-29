#ifndef TEST_GTK_ARENA_BACKGROUND_H
#define TEST_GTK_ARENA_BACKGROUND_H
#include <gtk/gtk.h>


void on_images_start_button_clickedd(GtkWidget *widget, gpointer data);
void add_child(GtkWidget *container, GtkWidget *child) {
    gtk_container_add(GTK_CONTAINER(container), child);
}

void on_select_image_bg(GtkWidget *button, gpointer data) {
    selected_arena = (gchar *)data; // Update the selected_arena variable with the new path
    g_print("Selected arena path: %s\n", selected_arena);
}

// Fonction principale
void create_images_window() {

     // Obtenir l'écran par défaut
    GdkScreen *screen = gdk_screen_get_default();

    // Obtenir la largeur et la hauteur de l'écran
    int screen_width = gdk_screen_get_width(screen);
    int screen_height = gdk_screen_get_height(screen);

    // Utilisez une taille l�g�rement inf�rieure � celle de l'�cran pour la fen�tre
    gint window_width = screen_width ;
    gint window_height = screen_height ;

    // Add background image
    Fenetre *images_window = initialiser_win("Images Window", NULL, "Images_Window", NULL, window_height , window_width, 0, 0, 0, 0, TRUE);
    create_window_n(images_window);

      FixWidget* fixed=initializeFixWidget("fixed2", screen_width, screen_height);
     createFixWidget(fixed);
     gtk_container_add(GTK_CONTAINER(images_window->window), fixed->fixed);

    imageObj *background_image = init_img("resourses/bggg1.png", window_width, window_height);;
    create_image(background_image);
    // Create the player selection UI
    addMoveToFixWidget(fixed, background_image->image, 0, 0, 1) ;


     LabelObj* message_player1= init_label("Choose your Arena","DRAGON HUNTER 48", 0, 0, 100,100, 0,0);
     addMoveToFixWidget(fixed, message_player1->label, 0, 0, 1) ;

    // Ajouter un bouton OK à la boîte de dialogue
    ButtonSimple *image1 = init_button_simple(NULL, "", "resourses/arena7_button.png", 10, 200, NULL, NULL, NULL, 1, 0, 0, 0);
    creer_button_Simple(image1);
      g_signal_connect(image1->button, "clicked", G_CALLBACK(on_select_image_bg),"resourses/arena7.png");
     addMoveToFixWidget(fixed, image1->button, 100, 10, 1) ;

     ButtonSimple *glad2 = init_button_simple(NULL, "", "resourses/arena5_button.png", 10, 200, NULL, NULL, NULL, 1, 0, 0, 0);
    creer_button_Simple(glad2);
     g_signal_connect(glad2->button, "clicked", G_CALLBACK(on_select_image_bg),"resourses/arena5.png");
     addMoveToFixWidget(fixed, glad2->button, 400, 10, 1) ;

     ButtonSimple *glad3 = init_button_simple(NULL, "", "resourses/arena6_button.png", 10, 200, NULL, NULL, NULL, 1, 0, 0, 0);
    creer_button_Simple(glad3);
       g_signal_connect(glad3->button, "clicked", G_CALLBACK(on_select_image_bg),"resourses/arena6.png");
     addMoveToFixWidget(fixed, glad3->button, 100, 100, 1) ;

     ButtonSimple *glad4 = init_button_simple(NULL, "", "resourses/arena8_button.png", 10, 200, NULL, NULL, NULL, 1, 0, 0, 0);
    creer_button_Simple(glad4);
    g_signal_connect(glad4->button, "clicked", G_CALLBACK(on_select_image_bg),"resourses/arena8.png");
     addMoveToFixWidget(fixed, glad4->button, 400, 100, 1) ;





    // Ajoutez le bouton "Start"
       ButtonSimple *button_start = init_button_simple(NULL, "Start", NULL, 10, 200, NULL, NULL, NULL, 1, 0, 0, 0);
    creer_button_Simple(button_start);
    g_signal_connect(button_start->button, "clicked", G_CALLBACK(on_images_start_button_clickedd),NULL);
     addMoveToFixWidget(fixed, button_start->button, 950, 570, 1) ;


    // Ajoutez le bouton "Exit"

           ButtonSimple *button_exit = init_button_simple(NULL, "Exit", NULL, 10, 200, NULL, NULL, NULL, 1, 0, 0, 0);
    creer_button_Simple(button_exit);
    g_signal_connect(button_exit->button, "clicked", G_CALLBACK(on_images_start_button_clickedd),NULL);
     addMoveToFixWidget(fixed, button_exit->button, 950, 650, 1) ;

     gtk_widget_show_all(images_window->window);

}


#endif //TEST_GTK_ARENA_BACKGROUND_H
