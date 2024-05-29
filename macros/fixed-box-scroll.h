#ifndef WINDOWBOXSCROLL_H_INCLUDED
#define WINDOWBOXSCROLL_H_INCLUDED
#include <gtk/gtk.h>

typedef struct {
    GtkWidget* scrollWidget;
    GtkWidget* container;
    GtkWidget* childContainer;
    gint borderWidth;
    gint width;
    gint height;
} Scroll;

typedef struct {
    GtkWidget* box;
    gchar type;
    gchar* name;
    gint homogeneous;
    gint spacing;
    struct {
        gchar* color;
        gfloat degree;
    } bgcolor;
} Box;

typedef struct {
    GtkWidget* fixed;
    gchar* name;
    gint width;
    gint height;
} FixWidget;



Scroll* initializeScroll(GtkWidget* container, GtkWidget* childContainer, gint borderWidth, gint width, gint height) {
    Scroll* scroll = (Scroll*)malloc(sizeof(Scroll));
    if (!scroll) {
        g_print("\nMemory allocation error");
        exit(-1);
    }

    scroll->container = NULL;
    scroll->childContainer = NULL;
    scroll->borderWidth = 0;
    scroll->height = scroll->width = 100;

    if (container)
        scroll->container = container;
    if (childContainer)
        scroll->childContainer = childContainer;
    if (borderWidth > 0)
        scroll->borderWidth = borderWidth;
    if (width > 100)
        scroll->width = width;
    if (height > 100)
        scroll->height = height;

    return scroll;
}

Scroll* createScroll(Scroll* scroll) {
    scroll->scrollWidget = gtk_scrolled_window_new(NULL, NULL);

    if (scroll->container) {
        if (GTK_IS_BOX(scroll->container))
            gtk_box_pack_start(GTK_BOX(scroll->container), scroll->scrollWidget, TRUE, TRUE, 0);
        else
            gtk_container_add(GTK_CONTAINER(scroll->container), scroll->scrollWidget);
    }

    gtk_container_set_border_width(GTK_CONTAINER(scroll->scrollWidget), scroll->borderWidth);

    if (scroll->childContainer) {
        gtk_container_add(GTK_CONTAINER(scroll->scrollWidget), scroll->childContainer);
    }

    gtk_widget_set_size_request(scroll->scrollWidget, scroll->width, scroll->height);

    return scroll;
}

Box* initializeBox(gchar* name, gchar type, gint homogeneous, gint spacing, gchar* bgcolor, gfloat opacity) {
    Box* B = (Box*)malloc(sizeof(Box));
    if (!B) {
        g_print("\nMemory allocation error for Box");
        exit(1);
    }

    B->name = NULL;
    B->bgcolor.color = NULL;

    if (name) {
        B->name = (gchar*)malloc(30 * sizeof(gchar));
        if (B->name)
            strcpy(B->name, name);
        else
            g_print("Memory allocation error for box name");
    }

    if (bgcolor) {
        B->bgcolor.color = (gchar*)malloc(15 * sizeof(gchar));
        if (B->bgcolor.color)
            strcpy(B->bgcolor.color, bgcolor);
        else
            g_print("Memory allocation error for box background color");
    }

    B->type = type;
    B->homogeneous = homogeneous;
    B->spacing = spacing;
    B->bgcolor.degree = opacity;

    return B;
}

Box* createBox(Box* B) {
    if (B->homogeneous)
        B->box = gtk_box_new(TRUE, B->spacing);
    else
        B->box = gtk_box_new(FALSE, B->spacing);

    if ((B->type == 'v') || (B->type == 'V'))
        gtk_orientable_set_orientation(GTK_ORIENTABLE(B->box), GTK_ORIENTATION_VERTICAL);
    else if ((B->type == 'h') || (B->type == 'H'))
        gtk_orientable_set_orientation(GTK_ORIENTABLE(B->box), GTK_ORIENTATION_HORIZONTAL);
    else {
        g_print("The type does not exist");
        exit(0);
    }

    if (B->bgcolor.color)
        addBackgroundColooor(B->box, B->bgcolor.color, B->bgcolor.degree);
    if (B->name)
        gtk_widget_set_name(B->box, B->name);

    return B;
}

void addToBox(GtkWidget* box, GtkWidget* child, gint type, gboolean expand, gboolean fill, gint padding) {
    if (type)
        gtk_box_pack_start(GTK_BOX(box), child, expand, fill, padding);
    else
        gtk_box_pack_end(GTK_BOX(box), child, expand, fill, padding);
}

FixWidget* initializeFixWidget(gchar* name, gint width, gint height) {
    FixWidget* FW = (FixWidget*)malloc(sizeof(FixWidget));
    if (!FW) {
        g_print("\nMemory allocation error for FixWidget");
        exit(1);
    }

    FW->name = NULL;
    if (name) {
        FW->name = (gchar*)malloc(30 * sizeof(gchar));
        if (FW->name)
            strcpy(FW->name, name);
        else
            g_print("Memory allocation error for FixWidget name");
    }

    FW->width = width;
    FW->height = height;
    return FW;
}

void createFixWidget(FixWidget* F) {
    F->fixed = gtk_fixed_new();
    gtk_widget_set_size_request(F->fixed, F->width, F->height);
    if (F->name)
        gtk_widget_set_name(F->fixed, F->name);
}

void addMoveToFixWidget(FixWidget* F, GtkWidget* child, gint posx, gint posy, gint type) {
    if (type)
        gtk_fixed_put(F->fixed, child, posx, posy);
    else
        gtk_fixed_move(GTK_FIXED(F->fixed), child, posx, posy);
}




#endif // WINDOWBOXSCROLL_H_INCLUDED
