
#ifndef TEST_GTK_ARENA_H
#define TEST_GTK_ARENA_H
#include <gtk/gtk.h>
#include "player.h"
#include "arena.h"
#include "gtk_utils.h" // Include the utility functions header
#include "imageHeader.h"
#include "label.h"
#include "ButtonSimpleheader.h"
#include "window.h"
#include "windowboxscroll.h"


typedef struct {
    int standing;
    int walking;
    int running;
    int attacking1;
    int attacking2;
    int attacking3;
    int defending;
    int jumping;
    int hurting;
    int dying;
} ActionFrames;

typedef struct Actor{
    const char *name;
    GdkPixbuf *sprite_sheet;
    GtkImage *image;
    int current_frame;
    int current_action;
    ActionFrames action_frames;
    int x_position;
    int y_position;
    double sprite_width;
    double sprite_height;
    int health;
    int Attack_P;
    int Defense_d;
    int speed ;
    void (*attack)(struct Actor *self, struct Machine *opponent);
} Actor;


typedef struct Machine{
    GdkPixbuf *sprite_sheet;
    GtkImage *image;
    int current_frame;
    int current_action;
    struct {
        int standing;
        int walking;
        int attacking1;
        int hurting;
        int dying;
    } action_frames;
    int x_position;
    int y_position;
    double sprite_width;
    double sprite_height;
    int health;
    int attack_p ;
    int add_health ;
    int speed ;
    void (*attack)(struct Machine *self, struct Actor *opponent);
} Machine;


extern Actor gladiator;
extern Machine machine;

extern gchar* selected_arena;

gboolean update_animation(gpointer data);
gboolean update_machine_animation(gpointer data);
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data);
void run_game(Actor *player , Machine *machine , int isSmart );
void gladiator_attack(Actor *self, Machine *opponent);
void machine_attack(Machine *self, Actor *opponent);
void update_health_label(int health ,LabelObj *health_label);



// Global variables
gint last_machine_attack_time = 0;
gint last_gladiator_attack_time = 0; // Track the last attack time of the gladiator
gboolean is_game_over = FALSE;
LabelObj *machine_health_label;
LabelObj *gladiator_health_label;
Actor gladiator ;
Machine machine;
int smart_flag = 0;  // flag to indicate smart machine
gchar* selected_arena = "resourses/Arena.png";


gboolean is_paused = FALSE;


extern imageObj *life_image_player1;
extern imageObj *life_image_player2;
extern imageObj *time_image;
extern imageObj *table_time_image;



// Global variable for the label widget
LabelObj *time_label;
int is_backward = 0 ;
// Function to update the health label
void update_health_label(int health, LabelObj *label) {
    if(health < 0 ) health = 0 ;
    gchar *health_str = g_strdup_printf("%d", health);
    gtk_label_set_text(label->label, health_str);
    g_free(health_str);
}

gboolean update_animation(gpointer data) {

    if (is_paused || is_game_over) {
        return TRUE;
    }

    // Ensure the gladiator stops all actions if dead
    if (gladiator.health <= 0) {
        gladiator.current_action = 9; // Dying action
        if (gladiator.current_frame < gladiator.action_frames.dying - 1) {
            gladiator.current_frame++;
        }
        // Stay on the last frame of the dying animation
        if (gladiator.current_frame >= gladiator.action_frames.dying - 1) {
            gladiator.current_frame = gladiator.action_frames.dying - 1;
        }
        // Update the action of the machine to standing when gladiator dies
        machine.current_action = 0; // Standing action
    } else {
        update_health_label(gladiator.health, gladiator_health_label);

        int frame_count = 0;
        switch (gladiator.current_action) {
            case 0: frame_count = gladiator.action_frames.standing; break;
            case 1: frame_count = gladiator.action_frames.walking; break;
            case 2: frame_count = gladiator.action_frames.running; break;
            case 3: frame_count = gladiator.action_frames.attacking1; break;
            case 4: frame_count = gladiator.action_frames.attacking2; break;
            case 5: frame_count = gladiator.action_frames.attacking3; break;
            case 6: frame_count = gladiator.action_frames.defending; break;
            case 7: frame_count = gladiator.action_frames.jumping; break;
            case 8: frame_count = gladiator.action_frames.hurting; break;
            case 9: frame_count = gladiator.action_frames.dying; break;
        }

        int x = (gladiator.current_frame % frame_count) * gladiator.sprite_width;
        int y = gladiator.current_action * gladiator.sprite_height;
        GdkPixbuf *frame = gdk_pixbuf_new_subpixbuf(gladiator.sprite_sheet, x, y, gladiator.sprite_width, gladiator.sprite_height);
        gtk_image_set_from_pixbuf(gladiator.image, frame);
        gtk_fixed_move(GTK_FIXED(data), GTK_WIDGET(gladiator.image), gladiator.x_position, gladiator.y_position);

        // Check if gladiator is dying
        if (gladiator.current_action == 9 && gladiator.current_frame == gladiator.action_frames.dying - 1) {
            is_game_over = TRUE; // Set game over state
        }
        gladiator.current_frame++;

        if (gladiator.current_frame >= frame_count) {
            gladiator.current_frame = 0;
        }

        // Handle gladiator movement
        if (!(gladiator.x_position >= 1900 - gladiator.sprite_width)) {
            if (gladiator.current_action == 1) {
                gladiator.x_position += gladiator.speed / 2;
            } else if (gladiator.current_action == 2) {
                gladiator.x_position += gladiator.speed;
            } else if (gladiator.current_action == 7) {
                if (is_backward && gladiator.x_position >= 17) {
                    gladiator.x_position -= gladiator.speed;
                } else {
                    gladiator.x_position += gladiator.speed - 2;
                }
            }
        }
    }

    return TRUE;
}


void gladiator_attack(Actor *self, Machine *opponent) {
    gint current_time = g_get_real_time();
    if (current_time - last_gladiator_attack_time >= 1 * G_TIME_SPAN_SECOND) { // 1 second cooldown
        if (self->current_action == 3 || self->current_action == 4 || self->current_action == 5) {
            if (abs(self->x_position - opponent->x_position) + 200 < self->sprite_width) {
                opponent->health -= self->Attack_P;
                opponent->current_action = 3; // Hurting action
                opponent->current_frame = 0;
                opponent->x_position -= opponent->speed; // Move the machine backward when hit
                if (opponent->health <= 0) {
                    opponent->current_action = 4; // Dying action
                }
            }
        }
        last_gladiator_attack_time = current_time; // Update the last attack time
    }
}


//------------------------------------------------------- reguler machine --------------------

// Function to handle machine attack
void machine_attack(Machine *self, Actor *opponent) {
    if (self->health <= 0) {
        self->current_action = 4; // Ensure the machine is in the dying action if health is zero
        return;
    }
    if (self->current_action == 2) {
        if (abs(self->x_position - opponent->x_position) < self->sprite_width + 100) {
            int damage = self->attack_p;
            if (opponent->current_action == 6) { // Check if gladiator is defending
                damage -= opponent->Defense_d / 2 ; // Reduce damage if defending
            }
            opponent->health -= damage;
            if (opponent->health <= 0) {
                opponent->current_action = 9; // Dying action
            } else {
                opponent->current_action = 8; // Hurting action
            }
            opponent->current_frame = 0;
        }
    }

    // Smart machine feature: health regeneration if gladiator is not attacking
    if (smart_flag && opponent->current_action < gladiator.action_frames.attacking1) {
        self->health += self->add_health; // Regenerate health
        if (self->health > 200) {
            self->health = 200; // Cap health at 200
        }
    }
}

gboolean update_machine_animation(gpointer data) {
    if (is_paused || is_game_over) return TRUE;

    Machine *actor = &machine;
    Actor *opponent = &gladiator;

    update_health_label(actor->health, machine_health_label);

    // Ensure the machine stops all actions if dead
    if (actor->health <= 0) {
        actor->current_action = 4; // Dying action
        if (actor->current_frame < actor->action_frames.dying - 1) {
            actor->current_frame++;
        }
        // Stay on the last frame of the dying animation
        if (actor->current_frame >= actor->action_frames.dying - 1) {
            actor->current_frame = actor->action_frames.dying - 1;
        }
    } else {
        // Calculate distance between machine and gladiator
        int distance = abs(actor->x_position - opponent->x_position);

        // Limit machine attacks to once every 3 seconds
        gint current_time = g_get_real_time();
        if (current_time - last_machine_attack_time >= 3 * G_TIME_SPAN_SECOND) {
            // If gladiator is within attack range, perform attack
            if (distance + 500 < actor->sprite_width) {
                actor->current_action = 2; // Attack action
                actor->current_frame = (actor->current_frame + 1) % actor->action_frames.attacking1;
                actor->attack(actor, opponent);
                last_machine_attack_time = current_time; // Update last attack time
            } else {
                // If not in range, move towards the gladiator
                if (actor->x_position < opponent->x_position) {
                    actor->x_position += 10; // Move right
                    actor->current_action = 1; // Walking right
                } else if (actor->x_position + 150 > opponent->x_position) {
                    actor->x_position -= 5; // Move left
                    actor->current_action = 1; // Walking left
                }
            }
        }
    }

    // Update the animation frame
    int frame_count = 0;
    switch (actor->current_action) {
        case 0: frame_count = actor->action_frames.standing; break;
        case 1: frame_count = actor->action_frames.walking; break;
        case 2: frame_count = actor->action_frames.attacking1; break;
        case 3: frame_count = actor->action_frames.hurting; break;
        case 4: frame_count = actor->action_frames.dying; break;
        default: return TRUE; // No action needed for other actions
    }

    int x = (actor->current_frame % frame_count) * actor->sprite_width;
    int y = actor->current_action * actor->sprite_height;
    GdkPixbuf *frame = gdk_pixbuf_new_subpixbuf(actor->sprite_sheet, x, y, actor->sprite_width, actor->sprite_height);
    gtk_image_set_from_pixbuf(actor->image, frame);
    gtk_fixed_move(GTK_FIXED(data), GTK_WIDGET(actor->image), actor->x_position, actor->y_position);

    if (actor->health > 0) { // Only increment the frame if the machine is not dead
        actor->current_frame++;
        if (actor->current_frame >= frame_count) {
            actor->current_frame = 0;
            if (actor->current_action == 3 && actor->health <= 0) {
                actor->current_action = 4; // Change to dying action if health is zero
            }
        }
    }

    return TRUE;
}


gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    switch (event->keyval) {
        case GDK_KEY_s:
            gladiator.current_action = 0; // Standing
            break;
        case GDK_KEY_w:
            gladiator.current_action = 1; // Walking
            break;
        case GDK_KEY_Right:
            gladiator.current_action = 2; // Running forward
            break;
        case GDK_KEY_a:
            gladiator.current_action = 3; // Attacking 1
            gladiator.attack(&gladiator, &machine);
            break;
        case GDK_KEY_f:
            gladiator.current_action = 4; // Attacking 2
            gladiator.attack(&gladiator, &machine);
            break;
        case GDK_KEY_g:
            gladiator.current_action = 5; // Attacking 3
            gladiator.attack(&gladiator, &machine);
            break;
        case GDK_KEY_d:
            gladiator.current_action = 6; // Defending
            break;
        case GDK_KEY_Up:
            gladiator.current_action = 7; // Jumping
            is_backward = 0;
            break;
        case GDK_KEY_Left:
            gladiator.current_action = 7; // Jumping backward
            is_backward = 1;
            break;
        default:
            return FALSE; // No action matched, do not reset the frame counter
    }

    // Reset the frame counter
    gladiator.current_frame = 0;

    // Return TRUE to keep the timeout running
    return TRUE;
}



// Function to update the time label
gboolean update_time_label(gpointer user_data) {
    if (is_paused || is_game_over ) {
        return TRUE;
    }
    static int seconds = 0;

    // Calculate minutes and seconds
    int minutes = seconds / 60;
    int display_seconds = seconds % 60;

    // Create the time string
    char time_str[6];
    snprintf(time_str, sizeof(time_str), "%02d:%02d", minutes, display_seconds);

    // Update the label text
    gtk_label_set_text(time_label->label, time_str);

    // Increment the seconds
    seconds++;

    // Return TRUE to keep the timeout running
    return TRUE;
}


gboolean reset_action(gpointer data);
void toggle_pause(GtkWidget *widget, gpointer data) {
    is_paused = !is_paused;

    GtkWidget *button = GTK_WIDGET(data);

    if (is_paused) {
        gtk_button_set_label(GTK_BUTTON(button), "Play");
    } else {
        gtk_button_set_label(GTK_BUTTON(button), "Pause");

    }
}



void run_game(Actor *player, Machine *selected_machine, int is_smart) {
    // Initialize GTK
    gtk_init(NULL, NULL);

    gladiator = *player;
    machine = *selected_machine;
    // Load the sprite sheet image
    gladiator.sprite_sheet = gdk_pixbuf_new_from_file(player->sprite_sheet, NULL);
    machine.sprite_sheet = gdk_pixbuf_new_from_file(machine.sprite_sheet, NULL);

    // Create the main window
    GdkPixbuf *arena_pixbuf = gdk_pixbuf_new_from_file(selected_arena, NULL);
    gint arena_width = gdk_pixbuf_get_width(arena_pixbuf);
    gint arena_height = gdk_pixbuf_get_height(arena_pixbuf);

    Fenetre *window = initialiser_win("Sprite Animation", NULL, "f1", NULL, arena_width, arena_height,
                                      0, 0, 0, 0, 1);
    create_window_n(window);

    // Connect key press event to the window
    g_signal_connect(window->window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    FixWidget *fixed = initializeFixWidget("fixed1", arena_width, arena_height);
    createFixWidget(fixed);
    gtk_container_add(GTK_CONTAINER(window->window), fixed->fixed);

    // Set the initial background image
    imageObj *arena_image = init_img(selected_arena, arena_width, arena_height);
    create_image(arena_image);
    addMoveToFixWidget(fixed, arena_image->image, 0, 0, 1);

    // Create an image widget to display the sprite animation
    gladiator.image = GTK_IMAGE(gtk_image_new());
    gtk_widget_set_size_request(GTK_WIDGET(gladiator.image), gladiator.sprite_width, gladiator.sprite_height);
    addMoveToFixWidget(fixed, GTK_WIDGET(gladiator.image), gladiator.x_position, gladiator.y_position, 1);

    // Create an image widget to display the sprite animation
    machine.image = GTK_IMAGE(gtk_image_new());
    gtk_widget_set_size_request(GTK_WIDGET(machine.image), machine.sprite_width, machine.sprite_height);
    addMoveToFixWidget(fixed, GTK_WIDGET(machine.image), machine.x_position, machine.y_position, 1);

    // Create image widgets for player 1 and player 2 life
    imageObj *life_image_player1 = init_img("resourses/LifeBar.png", 200, 200);
    create_image(life_image_player1);
    imageObj *life_image_player2 = init_img("resourses/LifeBar.png", 200, 200);
    create_image(life_image_player2);

    imageObj *time_image = init_img("resourses/clock.png", 200, 200);
    create_image(time_image);

    imageObj *table_time_image = init_img("resourses/table.png", 200, 200);
    create_image(table_time_image);

    // Add the life images to the fixed container
    addMoveToFixWidget(fixed, life_image_player1->image, 0, 0, 1);
    addMoveToFixWidget(fixed, life_image_player2->image, arena_width - 375, 0, 1);
    addMoveToFixWidget(fixed, table_time_image->image, arena_width / 2 - 160, 20, 1);
    addMoveToFixWidget(fixed, time_image->image, arena_width / 2 - 200, 0, 1);

    // Create labels to display the health of both players
    gladiator_health_label = init_label("100", "Ranger Eastwood 24", 0, 0, 500, 70, 250, 30);
    create_label22(gladiator_health_label);
    addMoveToFixWidget(fixed, gladiator_health_label->label, arena_width / 2 - 200, 0, 1);

    machine_health_label = init_label("100", "Ranger Eastwood 24", 0, 0, 500, 70, 20, 20);
    create_label22(machine_health_label);
    addMoveToFixWidget(fixed, machine_health_label->label, arena_width - 120, 30, 1);

    // Create a label for the time
    time_label = init_label("00:00", "Ranger Eastwood 24", 0, 0, 500, 70, 20, 20);
    create_label22(time_label);
    addMoveToFixWidget(fixed, time_label->label, arena_width / 2 - 90, 35, 1);

    // Add the pause button
    ButtonSimple *pause_button = init_button_simple("pause_button", "Pause", NULL, 10, 100, NULL,
                                                    NULL, NULL, 0, 0, arena_width - 350, arena_height - 400);
    creer_button_Simple(pause_button);
    addMoveToFixWidget(fixed, pause_button->button, arena_width / 3 + 20, 20, 1);

    // Attach the button to itself to pass to the callback
    g_object_set_data(G_OBJECT(pause_button->button), "pause_button", pause_button->button);

    // Connect the toggle_pause function to the button
    g_signal_connect(pause_button->button, "clicked", G_CALLBACK(toggle_pause), pause_button->button);

    // Add the fixed container to the window
    gtk_container_add(GTK_CONTAINER(window->window), fixed->fixed);

    // Show the window
    gtk_widget_show_all(window->window);

    // Start the animation
    g_timeout_add(100, update_animation, fixed->fixed);
    g_timeout_add(100, update_machine_animation, fixed->fixed);

    // Start the time label update every second
    g_timeout_add_seconds(1, update_time_label, NULL);

    // Play background music
    play_sound("resourses/GameMusic.wav");

    // Start the GTK main loop
    gtk_main();
}


//gboolean check_game_over(gpointer data) {
//    if (is_game_over) {
//        if (gladiator.health <= 0) {
//            gtk_label_set_text(GTK_LABEL(data), "Machine Wins!");
//        } else if (machine.health <= 0) {
//            gtk_label_set_text(GTK_LABEL(data), "Gladiator Wins!");
//        }
//    }
//
//    return TRUE;
//}
#endif //TEST_GTK_ARENA_H



