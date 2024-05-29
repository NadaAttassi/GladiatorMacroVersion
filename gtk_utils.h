
#ifndef TEST_GTK_GTK_UTILS_H
#define TEST_GTK_GTK_UTILS_H

#ifndef GTK_UTILS_H
#define GTK_UTILS_H

#include <gtk/gtk.h>
#include <windows.h>
#include <mmsystem.h>
//prototypes*************************
void set_css_styles();
void update_background_image(GtkWidget *fixed, GtkWidget **background_image, const char *file_path);
void play_sound(const char *sound_file);
//******************************************+

// Function to set CSS styles
void set_css_styles() {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    const char *css_data =
            " button {"
            "   border: 1px solid #7c7c7c;"
            "   border-radius: 15px;"
            "   font: Ranger Eastwood 36;"
            "   color: #543c1c;"
            "   background-color: #e4d3aa;"
            "   background-image: none;"
            "   min-width: 280px;"
            "   text-align: center;"
            "   padding: 10px 20px;"
            "}"
            " button:hover {"
            "   background-color: #fff7d7;"
            "   color: #ffffff;"
            "}"
            " button:hover:active {"
            "   background-color: #e8a154;"
            "   color: #ffffff;"
            "}"
            " button#custom-button-player1, button#custom-button-player2, button#custom-button-player3 {"
            "   border-color: #00ff00;"
            "   min-width: 150px;"
            "   padding: 5px 10px;"

            "}"
            " button#pause_button {"
            "   border: 5px solid #7c7c7c;"
            "   border-radius: 10%;"
            "   font: Ranger Eastwood 24;"
            "   color: #543c1c;"
            "   background-color: #e4d3aa;"
            "   background-image: none;"
            "   min-width: 55px;"
            "   min-height: 55px;"
            "   text-align: center;"
            "   padding: 2px 2px;"
            "}"
            ;

    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

// Function to update the background image
void update_background_image(GtkWidget *fixed, GtkWidget **background_image, const char *file_path) {
    if (*background_image) {
        gtk_widget_destroy(*background_image);
    }
    *background_image = gtk_image_new_from_file(file_path);
    gtk_fixed_put(GTK_FIXED(fixed), *background_image, 0, 0);
    gtk_widget_show(*background_image);
}


// Function to play sound
void play_sound(const char *sound_file) {
    PlaySound(sound_file, NULL, SND_FILENAME | SND_ASYNC);
}

#endif // GTK_UTILS_H

#endif //TEST_GTK_GTK_UTILS_H
