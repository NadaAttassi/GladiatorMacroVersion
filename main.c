#include <gtk/gtk.h>
#include "arena.h"
#include "gtk_utils.h"
#include "help.h"
#include "window.h"
#include "windowboxscroll.h"
#include "Gridheader.h"
#include "label.h"
#include "createCharacter.h"
#include "arena_background.h"
//#include "Bdialog.h"
extern imageObj *life_image_player1;
extern imageObj *life_image_player2;
extern imageObj *time_image;
extern imageObj *table_time_image;



// Global variable to store the selected player
Actor *selected_player = NULL;
Actor *player = NULL;

Machine *selected_machine = NULL ;
int isSmart = 1 ;
imageObj *player_table; // Add this global variable
imageObj *machine_table;




//----------------------------------------- create caracter full code --------------------------------

// Fonction de vérification des attributs du joueur avant de créer la fenêtre des images
// Fonction de vérification des attributs du joueur avant de créer la fenêtre des images
void check_player_attributes(GtkWidget *button, gpointer data) {

    if (!player || player->health == 0 || player->Attack_P == 0 || player->Defense_d == 0 || player->speed == 0) {
        // Créer une boîte de dialogue personnalisée pour informer l'utilisateur


        DialogObject *dialog = init_dialog(NULL, "Erreur", NULL, 1, 200, 150, GTK_WIN_POS_CENTER, "white", 0.8);
        create_dialog(dialog);

        // Ajouter le message d'erreur au dialogue
        LabelObj* label = init_label("Les attributs du joueur doivent être supérieurs à zéro et le personnage doit etre selectionné!", "lbl", 0, 1, 0, 0, 0, 0);
       create_label22(label);

       ajouter_widget_dialog(dialog->dialog, label->label);
       //bouton
        ButtonSimple* ok= init_button_simple("btn", "OK", NULL, 10,200, NULL,
                                      NULL,NULL, 1, 0, 0, 0);
        creer_button_Simple(ok);
        g_signal_connect(ok->button, "clicked", G_CALLBACK(click_dialogue), dialog);
        ajouter_widget_dialog(dialog->dialog, ok->button);


        // Afficher la boîte de dialogue et attendre la réponse de l'utilisateur
        gtk_widget_show_all(dialog->dialog);
        gtk_dialog_run(GTK_DIALOG(dialog->dialog));
        gtk_widget_destroy(dialog->dialog);

        // Libérer la mémoire allouée pour le dialogue
        free(dialog->title);
        free(dialog->icon);
        free(dialog->bgcolor.couleur);
        free(dialog);

        return; // Arrêter l'exécution de la fonction si les attributs ne sont pas valides
    }

    // Si les attributs sont valides, appeler la fonction create_images_window
    selected_player = player;
    create_images_window();
}



//Creation de perso
void ok_button_clicked_create(GtkWidget *widget, gpointer data) {
    CallbackData *callback_data = (CallbackData *)data;
    EntryObj *entry = callback_data->entry;
    Fenetre *myWindow = callback_data->window;

    const gchar *text = get_entry_text(entry);
    printf("Texte de l'entrée : %s\n", text);



    // Ensure player is allocated
    if (player == NULL) {
        player = (Actor *)malloc(sizeof(Actor));
        if (player == NULL) {
            fprintf(stderr, "\nFailed to allocate memory for player");
            exit(1);
        }
        player->name = NULL;
        player->health = 0;
        player->Attack_P = 0;
        player->Defense_d = 0;
        player->speed = 0;
    }

    // Free the existing name if necessary
    if (player->name != NULL) {
        free(player->name);
    }

    // Duplicate the entry text
    player->name = strdup(text);
    if (player->name == NULL) {
        fprintf(stderr, "\nFailed to allocate memory for player name");
        exit(1);
    }


    GdkScreen *screen = gdk_screen_get_default();
    gint screen_width = gdk_screen_get_width(screen);
    gint screen_height = gdk_screen_get_height(screen);

    gchar* title = "Nouvelle fenêtre";
    gchar* icon = NULL;
    gint width = screen_width;
    gint height = screen_height;
    gint position = 1;
    gint x = 0;
    gint y = 0;
    gint resizable = 1;

    Fenetre* myNewWindow = initialiser_win(title,icon,"f2",NULL,height, width, 0,x,  y,15, 1);
    create_window_n(myNewWindow);
    gtk_window_set_position(GTK_WINDOW(myNewWindow->window), GTK_WIN_POS_CENTER);
    //gtk_window_set_modal(GTK_WINDOW(myNewWindow->window), TRUE);

    Grille* myGrid = init_Grille("maGrille", width, height, 5, 0, NULL, 0, 0);
    create_Grille(myGrid);
    gtk_container_add(GTK_CONTAINER(myNewWindow->window), myGrid->Grille);

    LabelObj* titre = init_label("CREATE YOUR Player", "lbl", 0, 1, 0, 0, 0, 0);
    create_label22(titre);
    ajout_au_Grille(myGrid->Grille, titre->label, 0, 0);

    Box *left_box = initializeBox("left-box", 'v', 1, 0, "#FAFAD2", 0.7);
    createBox(left_box);
    //gtk_widget_set_size_request(left_box->box, 200, 600);
    ajout_au_Grille(myGrid->Grille, left_box->box, 0, 1);

    LabelObj* myLabel = init_label(text, "nom", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(myLabel);

    ButtonSimple* edit_name_button= init_button_simple("btn", "Modifier le nom", NULL, 10,200, NULL,
                                      NULL,NULL, 1, 0, 0, 0);
    creer_button_Simple(edit_name_button);

    noms = g_malloc(sizeof(Noms));
    noms->ancienNom = g_strdup(text);
    g_signal_connect(edit_name_button->button, "clicked", G_CALLBACK(modifier), myLabel);

    LabelObj* label_Defense = init_label("Defense", "lelabel", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(label_Defense);
    LabelObj* label_Attack = init_label("Attaque", "lelabel", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(label_Attack);
    LabelObj* label_PV = init_label("Vitalite", "lelabel", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(label_PV);
    LabelObj* label_speed = init_label("Vitesse", "lelabel", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(label_speed);

     LabelObj* label_points = init_label("Remaining Points: 200", "remaining-points-label", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(label_points);

    options.remaining_points = TOTAL_POINTS;
    options.remaining_points_label = label_points;


    addToBox(left_box->box, options.remaining_points_label->label, 0, 1, 1, 1);
    options.sliders[0] = create_slider(&options);
    options.sliders[1] = create_slider(&options);
    options.sliders[2] = create_slider(&options);
    options.sliders[3] = create_slider(&options);

    addToBox(left_box->box, options.sliders[0], 0, FALSE, FALSE, FALSE);
    addToBox(left_box->box, label_Defense->label, 0, FALSE, FALSE, FALSE);
    addToBox(left_box->box, options.sliders[1], 0, 1, 1, 1);
    addToBox(left_box->box, label_Attack->label, 0, FALSE, FALSE, FALSE);
    addToBox(left_box->box, options.sliders[2], 0, 1, 1, 1);
    addToBox(left_box->box, label_PV->label, 0, FALSE, FALSE, FALSE);
    addToBox(left_box->box, options.sliders[3], 0, 1, 1, 1);
    addToBox(left_box->box, label_speed->label, 0, FALSE, FALSE, FALSE);

    addToBox(left_box->box, edit_name_button->button, 0, 1, 1, 0);
    addToBox(left_box->box, myLabel->label, 0, 1, 1, 1);


    // Create the photo area and add it to the grid
    photo_area= initializeBox("photo-area", 'v', 1, 0, "", 1);

    createBox(photo_area);
    gtk_widget_set_size_request(photo_area->box, 800, 300);
    const gchar *photo_path = "resourses/mage.png"; // Replace with the path to your default photo
    current_photo_widget = init_img(photo_path, 500, 500);
    create_image(current_photo_widget);
    addToBox(photo_area->box, current_photo_widget->image, 0, 1, 1, 1);

    //ajout_au_Grille(myGrid->Grille, photo_area->box, 2, 1);


    Actor players[4] = {
            {
                    .name = "Light Mage",
                    .sprite_sheet = "resourses/Light_Mage_Spritelist.png",
                    .image = "resourses/mage.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 7,
                            .walking = 7,
                            .running = 8,
                            .attacking1 = 10,
                            .attacking2 = 4,
                            .attacking3 = 12,
                            .defending = 13,
                            .jumping = 8,
                            .hurting = 3,
                            .dying = 5
                    },
                    .x_position = 10,
                    .y_position = 450,
                    .sprite_width = 3584/15,
                    .sprite_height = 239,
                    .health = 0,
                    .Attack_P = 0 ,
                    .Defense_d = 0 ,
                    .speed = 0 ,
                    .attack = gladiator_attack
            },

            {
                    .name = "Wanderer Magican",
                    .sprite_sheet = "resourses/Wanderer_Magican_Spritelist.png",
                    .image = "resourses/magician.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 8,
                            .walking = 7,
                            .running = 8,
                            .attacking1 = 7,
                            .attacking2 = 9,
                            .attacking3 = 15,
                            .defending = 6,
                            .jumping = 8,
                            .hurting = 4,
                            .dying = 4
                    },
                    .x_position = 10,
                    .y_position = 450,
                    .sprite_width = 3584/16,
                    .sprite_height = 225,
                    .health = 0,
                    .Attack_P = 0 ,
                    .Defense_d = 0 ,
                    .speed = 0 ,
                    .attack = gladiator_attack
            },


            {
                    .name = "Samurai Commander",
                    .sprite_sheet = "resourses/Samurai_Commander_Spritelist.png",
                    .image = "resourses/commander.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 5,
                            .walking = 9,
                            .running = 8,
                            .attacking1 = 4,
                            .attacking2 = 5,
                            .attacking3 = 4,
                            .defending = 2,
                            .jumping = 7,
                            .hurting = 2,
                            .dying = 6
                    },
                    .x_position = 10,
                    .y_position = 450,
                    .sprite_width = 256,
                    .sprite_height = 256.4,
                    .health = 0,
                    .Attack_P = 0 ,
                    .Defense_d = 0 ,
                    .speed = 0 ,
                    .attack = gladiator_attack
            },


            {
                    .name = "Samurai Archer",
                    .sprite_sheet = "resourses/Samurai_Archer_Spritelist.png",
                    .image = "resourses/samurai.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 5,
                            .walking = 9,
                            .running = 8,
                            .attacking1 = 4,
                            .attacking2 = 5,
                            .attacking3 = 4,
                            .defending = 2,
                            .jumping = 7,
                            .hurting = 2,
                            .dying = 6
                    },
                    .x_position = 10,
                    .y_position = 450,
                    .sprite_width = 256,
                    .sprite_height = 256.1,
                    .health = 0,
                    .Attack_P = 0 ,
                    .Defense_d = 0 ,
                    .speed = 0 ,
                    .attack = gladiator_attack
            },
    };
    // Create the Player thumbnails and add them to the grid
    Box *thumbnails_container = create_Player_thumbnails(players, 4);

    // Création d'un bouton simple
    ButtonSimple* play_btn = init_button_simple("start", "Play",NULL, 10,10, "arial", "black","", FALSE, TRUE, 1600, 400);
    creer_button_Simple(play_btn);

   //gtk_widget_set_size_request(left_box->box, 200, 600);
    ajout_au_Grille(myGrid->Grille, photo_area->box, 30, 50);

    //gtk_widget_set_size_request(left_box->box, 200, 600);
    ajout_au_Grille(myGrid->Grille,thumbnails_container->box, 30, 10);
   //gtk_widget_set_size_request(left_box->box, 200, 600);
    ajout_au_Grille(myGrid->Grille, play_btn->button, 20, 10);

    //    g_signal_connect(simpleButton->button, "clicked", G_CALLBACK(ok_button_clicked), callback_data);
    load_css2();
    // Connectez d'abord le signal "clicked" du bouton à une fonction de vérification
    g_signal_connect(play_btn->button, "clicked", G_CALLBACK(check_player_attributes), NULL);


    // Show the new window
    gtk_widget_show_all(myNewWindow->window);

    gtk_widget_destroy(myWindow->window);

    // Start a new GTK main loop for the new window
    gtk_main();
}

int create_charachter() {


    // Création de la fenêtre principale
    gchar* title = "Nom du personnage";
    gchar* icon = NULL;
    gint height = 200;
    gint width = 400;
    gint position = 1; // Position centrale
    gint x = 0;
    gint y = 0;
    gint resizable = 0; // Redimensionnable

    // Initialisation de la fenêtre principale
    Fenetre* myWindow = initialiser_win(title,icon,"f2",NULL,height, width, 0,x,  y,15, 1);
    create_window_n(myWindow);
    gtk_window_set_position(GTK_WINDOW(myWindow->window), GTK_WIN_POS_CENTER);
    Grille* myGrid = init_Grille("maGrille", 400, 200, 5, 0, NULL, 100, 100);
    create_Grille(myGrid);
    gtk_container_add(GTK_CONTAINER(myWindow->window), myGrid->Grille);


    // Création et initialisation du label
    LabelObj* myLabel = init_label("Veuillez choisir le nom du personnage", "mon_label", FALSE, FALSE, 100, 20, 0, 10);
    create_label22(myLabel);

    // Appliquer le style au label
    apply_label_style(myLabel->label);

    ajout_au_Grille(myGrid->Grille,myLabel->label,myLabel->x,myLabel->y);
    //image

    imageObj* img=init_img("resourses/golden.png",400,400);
    create_image(img);
    ajout_au_Grille(myGrid->Grille,img->image,0,0);
    //ENTRY

    EntryObj* entry= init_entry ("Nom du personnage...",NULL, 1,0,10,40,0,14);
    create_entry(entry);
    ajout_au_Grille(myGrid->Grille,entry->entry ,entry->x,entry->y);


    CallbackData *callback_data = (CallbackData *)malloc(sizeof(CallbackData));
    if (!callback_data) {
        fprintf(stderr, "Failed to allocate memory for callback_data\n");
        exit(1);
    }

    callback_data->entry = entry->entry;
    callback_data->window = myWindow;

    // Création d'un bouton simple
    ButtonSimple* simpleButton = init_button_simple("mon_bouton", "OK",NULL, 10,10, "Ranger Eastwood 24", "","white", FALSE, TRUE, 0, 190);
    creer_button_Simple(simpleButton);
    // ajout_au_Grille(myGrid->Grille,simpleButton->button,simpleButton->x,simpleButton->y);

    ajout_au_Grille(myGrid->Grille, simpleButton->button,simpleButton->x,simpleButton->y);

    g_signal_connect(simpleButton->button, "clicked", G_CALLBACK(ok_button_clicked_create), callback_data);

    //*******************************
    // Connexion de l'événement de fermeture de la fenêtre principale
    g_signal_connect(myWindow->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Affichage de la fenêtre principale et démarrage de la boucle GTK
    gtk_widget_show_all(myWindow->window);
    //
    // gtk_widget_show(myDialog->dialog);

    gtk_main();
    // Libération de la mémoire allouée
    if (myWindow) {
        g_free(myWindow->titre);
        g_free(myWindow->icon);
        free(myWindow);
    }

    if (simpleButton) {
        g_free(simpleButton->name);
        g_free(simpleButton->label);
        free(simpleButton);
    }

    // Libération de la mémoire allouée pour le label
    if (myLabel) {
        if (myLabel->text)
            g_free(myLabel->text);
        if (myLabel->name)
            g_free(myLabel->name);
        free(myLabel);
    }
    if (myGrid) {
        g_free(myGrid->name);
        //if (myGrid->bgCLor.couleur)
        // g_free(myGrid->bgCLor.couleur);
        if (myGrid->imageBg.image)
            g_free(myGrid->imageBg.image);
        free(myGrid);
    }

    free(callback_data);
    return 0;
}

void on_button_clicked(GtkWidget *button, gpointer data) {
    const char *button_label = (const char *)data;

    if (g_strcmp0(button_label, "Start") == 0 && selected_machine && selected_player) {
        if (selected_player && selected_machine) {
            g_print("Starting the game with %s\n", selected_player->name);
            create_images_window(selected_player);
        } else {
            g_print("No player or machine selected!\n");
        }
    } else if (g_strcmp0(button_label, "CREATE GLADIATOR") == 0 && selected_machine && !selected_player) {
        g_print("Create Gladiator clicked\n");
        create_charachter();
        // Code to create a new gladiator
    } else if (g_strcmp0(button_label, "EXIT") == 0) {
        g_print("Exit clicked\n");
        gtk_main_quit();
    } else if (g_strcmp0(button_label, "HELP") == 0) {
        // Code to display help dialog

        display_help_dialog();
    }
}
/*
void on_select_AVATER_clicked(GtkWidget *button, gpointer data) {
    const char *button_label = gtk_button_get_label(GTK_BUTTON(button));
    if (g_strcmp0(button_label, "Select 1") == 0) {
        selected_avatar = 1 ;

    } else if (g_strcmp0(button_label, "Select 1") == 0) {

        selected_avatar = 2 ;
    }
}*/
// Callback for the start button in the images window
void on_images_start_button_clickedd(GtkWidget *button_label, gpointer data) {
    // Add the logic you want to execute when the start button in the images window is clicked
    if(player) selected_player=player;
    if (selected_player) {
        g_print("Starting the game with %s\n", selected_player->name);
        run_game(selected_player , selected_machine , isSmart); // Appelez run_game avec le joueur sélectionné
    } else {
        g_print("No player selected!\n");


    }
}
void play_audio(const char *file_path) {
    // Open the audio file
    if (PlaySound(file_path, NULL, SND_FILENAME | SND_ASYNC) == 0) {
        // Error handling if the audio file fails to play
        printf("Error playing audio: %d\n", GetLastError());
    }
}


// Callback for button click events
void on_select_button_clicked(GtkWidget *button, Actor *data) {
    selected_player = data;
    g_print("Selected player: %s\n", data->name);  // Print the selected player name for verification
    // Update the player_machine_table_image with the selected player's image
    GdkPixbuf *player_image = gdk_pixbuf_new_from_file(data->image, NULL);
    gtk_image_set_from_pixbuf(player_table->image, player_image) ;
}

void on_select_machine_button_clicked(GtkWidget *button, Machine *data) {
    selected_machine = data;;
    g_print("Selected machine: %s\n", data->image );  // Print the selected player name for verification
    // Update the player_machine_table_image with the selected player's image
    GdkPixbuf *machine_image = gdk_pixbuf_new_from_file(data->image, NULL);
    gtk_image_set_from_pixbuf(machine_table->image, machine_image) ;
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

     // Obtenir l'écran par défaut
    GdkScreen *screen = gdk_screen_get_default();

    // Obtenir la largeur et la hauteur de l'écran
    int screen_width = gdk_screen_get_width(screen);
    int screen_height = gdk_screen_get_height(screen);

    Fenetre*window=initialiser_win("Character Selection", NULL,"f1",NULL, screen_height, screen_width,
                0, 0,0,0, 1);
    create_window_n(window);
     // Vérifier si la fenêtre a été correctement créée
    if (!window || !window->window) {
        fprintf(stderr, "Erreur: Impossible de créer la fenêtre.\n");
        return -1;
    }



    // Create a fixed container
    //GtkWidget *fixed = create_fixed();

    FixWidget* fixed=initializeFixWidget("fixed1", screen_width, screen_height);
    createFixWidget(fixed);
    gtk_container_add(GTK_CONTAINER(window->window), fixed->fixed);

    // Set the initial background image

    // update_background_image(fixed->fixed, &background_image, "resourses/Menu.jpg");

    imageObj *background_image = init_img("resourses/Menu.jpg", screen_width, screen_height);;

    create_image(background_image);
    // Create the player selection UI
    addMoveToFixWidget(fixed, background_image->image, 0, 0, 1) ;

        Actor players[3] = {
            {
                    .name = "Samuri",
                    .sprite_sheet = "resourses/Samurai_Spritelist.png",
                    .image = "resourses/vector1.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 6,
                            .walking = 9,
                            .running = 8,
                            .attacking1 = 4,
                            .attacking2 = 5,
                            .attacking3 = 4,
                            .defending = 2,
                            .jumping = 8,
                            .hurting = 3,
                            .dying = 6
                    },
                    .x_position = 10,
                    .y_position = 450,
                    .sprite_width = 256,
                    .sprite_height = 256,
                    .health = 200,
                    .Attack_P = 10 ,
                    .Defense_d = 8 ,
                    .speed = 11 ,
                    .attack = gladiator_attack
            },

            {
                    .name = "Fire vizard",
                    .sprite_sheet = "resourses/Fire_Mage_Spritelist.png",
                    .image = "resourses/vector2.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 7,
                            .walking = 6,
                            .running = 8,
                            .attacking1 = 4,
                            .attacking2 = 4,
                            .attacking3 = 8,
                            .defending = 14,
                            .jumping = 9,
                            .hurting = 3,
                            .dying = 6
                    },
                    .x_position = 10,
                    .y_position = 450,
                    .sprite_width = 3584/15,
                    .sprite_height = 239,
                    .health = 300,
                    .Attack_P = 18 ,
                    .Defense_d = 0 ,
                    .speed = 9 ,
                    .attack = gladiator_attack
            },

            {
                    .name = "Knight",
                    .sprite_sheet = "resourses/knight_1_spritelist.png",
                    .image = "resourses/vector3.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 1,
                            .walking = 7,
                            .running = 6,
                            .attacking1 = 5,
                            .attacking2 = 4,
                            .attacking3 = 4,
                            .defending = 1,
                            .jumping = 2,
                            .hurting = 6,
                            .dying = 6
                    },
                    .x_position = 10,
                    .y_position = 420,
                    .sprite_width = 2816/10,
                    .sprite_height = 2816/10  + 1,
                    .health = 150,
                    .Attack_P = 20 ,
                    .Defense_d = 10 ,
                    .speed = 10,
                    .attack = gladiator_attack
            },
    };

    Machine machines[2] = {
             {
                .sprite_sheet = "resourses/machine1.png",
                .image = "resourses/demon_idle_1.png",
                .current_frame = 0,
                .current_action = 0,
                .action_frames = {
                        .standing = 5,
                        .walking = 12,
                        .attacking1 = 15,
                        .hurting = 5,
                        .dying = 22
                },
                .x_position = 1900 - (12672/22),
                .y_position = 380,
                .sprite_width = (12672/22),
                .sprite_height = (1600/5),
                .health = 200,
                .attack_p = 15,
                .add_health = 0,
                .speed = 7,
                .attack = machine_attack
            },
             {
                .sprite_sheet = "resourses/Bringer-of-Death-SpritSheet.png",
                .image = "resourses/Bringer-of-Death_Idle_1.png",
                .current_frame = 0,
                .current_action = 0,
                .action_frames = {
                        .standing = 8,
                        .walking = 8,
                        .attacking1 = 8,
                        .hurting = 8,
                        .dying = 7
                },
                .x_position = 1900 - 420,
                .y_position = 430,
                .sprite_width = 420,
                .sprite_height = 280.4,
                .health = 200,
                .attack_p = 13,
                .add_health = 2,
                .speed = 9,
                .attack = machine_attack
            },

    };
    // Create the player selection UI
    imageObj* list_cards=init_img("resourses/list_card.png", 900, 600);
    create_image(list_cards);
    gtk_fixed_put(fixed->fixed, list_cards->image, 60, screen_height / 10);



     imageObj* player1=init_img(players[0].image, 200, 200);
    create_image(player1);
     imageObj* player2=init_img(players[1].image, 200, 200);
     create_image(player2);
     imageObj* player3=init_img(players[02].image, 200, 200);
     create_image(player3);


     addMoveToFixWidget(fixed,  player1->image, 140, screen_height / 3, 1) ;
     addMoveToFixWidget(fixed,  player2->image, screen_width / 6 + 140, screen_height / 3, 1) ;
     addMoveToFixWidget(fixed,  player3->image, screen_width / 3 + 140, screen_height / 3, 1) ;


         imageObj* card_player1=init_img("resourses/Civilian_card.png", 400, 400);
    create_image(card_player1);
     imageObj* card_player2=init_img("resourses/Civilian_card.png", 400, 400);
     create_image(card_player2);
     imageObj* card_player3=init_img("resourses/Civilian_card.png", 400, 400);
     create_image(card_player3);


     addMoveToFixWidget(fixed,  card_player1->image, 100,  screen_height / 3 - 20, 1) ;
     addMoveToFixWidget(fixed,  card_player2->image,screen_width / 6 + 100,  screen_height / 3 - 20, 1) ;
     addMoveToFixWidget(fixed,  card_player3->image, screen_width / 3 + 100,  screen_height / 3 - 20, 1) ;


    imageObj* player1_message=init_img("resourses/message.png", 200, 200);
    create_image(player1_message);
     imageObj* player2_message=init_img("resourses/message.png", 200, 200);
     create_image(player2_message);
     imageObj* player3_message=init_img("resourses/message.png", 200, 200);
     create_image(player3_message);


     addMoveToFixWidget(fixed,  player1_message->image, 80, screen_height / 8, 1) ;
     addMoveToFixWidget(fixed,  player2_message->image,screen_width / 6 + 60,  screen_height / 8, 1) ;
     addMoveToFixWidget(fixed,  player3_message->image, screen_width / 3 + 50,  screen_height / 8, 1) ;

//______________

    ButtonSimple* button_select_player1=init_button_simple("custom-button-player1", "select", NULL, 10, 100,NULL,
                                      NULL, NULL, 0, 0, 0, 0);
   creer_button_Simple(button_select_player1);
    ButtonSimple* button_select_player2=init_button_simple("custom-button-player2", "select", NULL, 10, 100,NULL,
                                      NULL, NULL, 0, 0, 0, 0);
   creer_button_Simple(button_select_player2);
    ButtonSimple* button_select_player3=init_button_simple("custom-button-player3", "select", NULL, 10, 100,NULL,
                                      NULL, NULL, 0, 0, 0, 0);
   creer_button_Simple(button_select_player3);

     addMoveToFixWidget(fixed,  button_select_player1->button, 150, screen_height - 300, 1) ;
     addMoveToFixWidget(fixed,  button_select_player2->button,screen_width / 6 + 150,  screen_height - 300, 1) ;
     addMoveToFixWidget(fixed,  button_select_player3->button,screen_width / 3 + 150, screen_height - 300, 1) ;

    g_signal_connect(button_select_player1->button, "clicked", G_CALLBACK(on_select_button_clicked), &players[0]);
    g_signal_connect(button_select_player2->button, "clicked", G_CALLBACK(on_select_button_clicked), &players[1]);
    g_signal_connect(button_select_player3->button, "clicked", G_CALLBACK(on_select_button_clicked), &players[2]);



    //---------------------- selected payer and machine -------------------------

      imageObj* player_machine_table=init_img("resourses/bg.png", 550, 400);
     create_image(player_machine_table);
     addMoveToFixWidget(fixed,  player_machine_table->image,  screen_width /2 + 200 , 20, 1) ;

      imageObj* vs=init_img("resourses/vs.png", 180, 180);
     create_image(vs);
     addMoveToFixWidget(fixed,  vs->image,  screen_width - 350 , 150, 1) ;


    player_table = init_img(NULL, 200, 200);
     create_image(player_table);

    addMoveToFixWidget(fixed, player_table->image, screen_width /2 + 250 ,50,1);

     machine_table = init_img(NULL, 200, 200);
     create_image(machine_table);

    addMoveToFixWidget(fixed, machine_table->image, screen_width - 300 ,50,1);

     LabelObj* machine_label=init_label("Select the machine", "Ranger Eastwood 32", 0, 0, 500, 70,  screen_width /1.5 + 50, 400);
     create_label22(machine_label);
      addMoveToFixWidget(fixed, machine_label->label, machine_label->x,machine_label->y,1);


    // select machine buttons
     ButtonSimple* button_select_regular_machine=init_button_simple( "custom-button-player1", "regular machine ", NULL, 10, 100,NULL,
                                      NULL, NULL, 0, 0, 0, 0);
     creer_button_Simple(button_select_regular_machine);

     ButtonSimple* button_select_smart_machine=init_button_simple( "custom-button-player2", "smart machine", NULL, 10, 100,NULL,
                                      NULL, NULL, 0, 0, 0, 0);
    creer_button_Simple(button_select_smart_machine);

    addMoveToFixWidget(fixed, button_select_regular_machine->button, screen_width /1.5 - 50, 500,1);
    addMoveToFixWidget(fixed, button_select_smart_machine->button, screen_width /1.5 + 270, 500,1);

    g_signal_connect(button_select_regular_machine->button, "clicked", G_CALLBACK(on_select_machine_button_clicked), &machines[0]);
    g_signal_connect(button_select_smart_machine->button, "clicked", G_CALLBACK(on_select_machine_button_clicked), &machines[1]);
    //----------------------------------------------------------------------------

    LabelObj* message_player1=init_label("Our destiny \n awaits us", "DRAGON HUNTER 12", 0, 0, 500, 70,  100, screen_width / 4 + 50);
     create_label22(message_player1);
     LabelObj* message_player2=init_label("Courage will \n guide us.", "DRAGON HUNTER 12", 0, 0, 100, 70,  screen_width / 6 + 90, screen_height / 4 + 50);
     create_label22(message_player2);
     LabelObj* message_player3=init_label("Faith leads \n the way.", "DRAGON HUNTER 12", 0, 0, 100, 70,  screen_width / 3 + 80, screen_height / 4 + 50);
     create_label22(message_player3);

     addMoveToFixWidget(fixed, message_player1->label, message_player1->x,message_player1->y,1);
     addMoveToFixWidget(fixed, message_player2->label,message_player2->x, message_player2->y,1);
     addMoveToFixWidget(fixed, message_player3->label, message_player3->x, message_player3->y,1);


       LabelObj* label_player1=init_label("Nom: Samuri\nPV: 100\nAttaque: 50\nDefense: 30\nSpecial Capacity: ", "DRAGON HUNTER 12", 0, 0, 500, 70,  150, screen_height / 3 + 270);
     create_label22(label_player1);
     LabelObj* label_player2=init_label("Nom: Fire vizard\nPV: 120\nAttaque: 40\nDefense: 40\nSpecial Capacity: ", "DRAGON HUNTER 12", 0, 0, 100, 70, screen_width / 6 + 150, screen_height / 3 + 270);
     create_label22(label_player2);
     LabelObj* label_player3=init_label("Nom: Knight\nPV: 80\nAttaque: 60\nDefense: 20\nSpecial Capacity: ", "DRAGON HUNTER 12",0, 0, 100, 70,  screen_width / 3 + 150, screen_height / 3 + 270);
     create_label22(label_player3);

     addMoveToFixWidget(fixed, label_player1->label, label_player1->x,label_player1->y,1);
     addMoveToFixWidget(fixed, label_player2->label,label_player2->x, label_player2->y,1);
     addMoveToFixWidget(fixed, label_player3->label, label_player3->x, label_player3->y,1);


 ButtonSimple* button_start=init_button_simple("custom-button-player1", "Start", NULL, 10, 100,NULL,
                                      NULL, NULL, 0, 0, screen_width - 350, screen_height - 400);
   creer_button_Simple(button_start);
    ButtonSimple* button_create=init_button_simple("custom-button-player1","CREATE GLADIATOR", NULL, 10, 100,NULL,
                                      NULL, NULL, 0, 0, screen_width - 350, screen_height - 325);
   creer_button_Simple(button_create);
    ButtonSimple* button_exit=init_button_simple("custom-button-player1", "EXIT", NULL, 10, 100,NULL,
                                      NULL, NULL, 0, 0, screen_width - 350, screen_height - 250);
   creer_button_Simple(button_exit);
    ButtonSimple* button_help=init_button_simple("custom-button-player1", "HELP", NULL, 10, 100,NULL,
                                      NULL, NULL, 0, 0, screen_width - 350, screen_height - 175);
   creer_button_Simple(button_help);

    addMoveToFixWidget(fixed, button_start->button, button_start->x,button_start->y,1);
     addMoveToFixWidget(fixed, button_create->button,button_create->x, button_create->y,1);
     addMoveToFixWidget(fixed, button_exit->button, button_exit->x, button_exit->y,1);
     addMoveToFixWidget(fixed, button_help->button, button_help->x, button_help->y,1);




    g_signal_connect(button_start->button, "clicked", G_CALLBACK(on_button_clicked), button_start->label);
    g_signal_connect(button_create->button, "clicked", G_CALLBACK(on_button_clicked), button_create->label);
    g_signal_connect(button_exit->button, "clicked", G_CALLBACK(on_button_clicked), button_exit->label);
    g_signal_connect(button_help->button, "clicked", G_CALLBACK(on_button_clicked), button_help->label);

    LabelObj* label=init_label("Select your Vector", "Ranger Eastwood 48", 0, 0, 500, 70,  screen_height / 7, screen_height / 5);
     create_label22(label);
     addMoveToFixWidget(fixed, label->label, label->x, label->y,1);



    set_css_styles();
    gtk_widget_show_all(window->window);

    play_sound("resourses/GameMusic.wav");
    gtk_main();

    return 0;
}
