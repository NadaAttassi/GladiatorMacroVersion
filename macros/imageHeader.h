
#ifndef TEST_GTK_IMAGEHEADER_H
#define TEST_GTK_IMAGEHEADER_H
#include <gtk/gtk.h>
typedef struct{
    GtkWidget* image;
    gint w;
    gint h;
    gchar* path;
}imageObj;
imageObj* init_img(gchar* path, gint width, gint height) {
    imageObj* im = (imageObj*)malloc(sizeof(imageObj));
    if (im != NULL) {
        im->path = g_strdup(path); // Allouer de la m�moire et copier le chemin
        im->w = width;
        im->h = height;
    }
    return im;
}

void create_image(imageObj* img)
{
    if (img->path && img->path[0] != '\0') {
        GdkPixbuf *px = gdk_pixbuf_new_from_file_at_size(img->path, img->w, img->h, NULL);
        if (px != NULL) {
            img->image = gtk_image_new_from_pixbuf(px);
        } else {
            g_print("Failed to load image from file: %s\n", img->path);
            img->image = NULL;
        }
    } else {
        //g_print("Invalid image file path\n");
       // img->image = NULL;
        img->image=gtk_image_new_from_file(NULL);
    }
}



#endif //TEST_GTK_IMAGEHEADER_H
