
#ifndef TEST_GTK_ENTRYHEADER_H
#define TEST_GTK_ENTRYHEADER_H

#include <gtk/gtk.h>
typedef struct{
    GtkWidget* entry;
    gchar* placeholder;
    gchar* icon;
    gint visible;
    gint icon_pos;
    gint height;
    gint width;
    gint x;
    gint y;
}EntryObj;
//************************************************PROTOTYPES********************************************************
// Radiobox
EntryObj* init_entry (gchar* placeholder,gchar* icon, gint visible,gint icon_pos,gint height,
                      gint width,gint x,gint y);
void create_entry(EntryObj *e);
//****************************************************************************************
EntryObj* init_entry (gchar* placeholder,gchar* icon, gint visible,gint icon_pos,gint height,
                      gint width,gint x,gint y)
{
    EntryObj* N=NULL;
    N=(EntryObj*)malloc(sizeof(EntryObj));
    if(!N)
    {
        printf("\nERREur");
        exit(0);
    }
    if(placeholder){
        N->placeholder=(gchar*)malloc(30*sizeof(gchar));
        if(N->placeholder) strcpy(N->placeholder,placeholder);
        else printf("Erreur d allocation");
    }
    else N->placeholder=NULL;
    //l icone
    if(icon)
    {
        N->icon=(gchar*)malloc(30*sizeof(gchar));
        if(N->icon) strcpy(N->icon,icon);
        else printf("Erreur d allocation");
    }
    else N->icon=NULL;

    N->visible=visible;
    N->icon_pos=icon_pos;
    N->height=height;
    N->width=width;
    N->x=x;
    N->y=y;
    return((EntryObj*)N);
}
//*********************************************************
void create_entry(EntryObj *e)
{
    e->entry=gtk_entry_new();
    gtk_entry_set_placeholder_text(e->entry,e->placeholder);
    if(e->visible==0)
        gtk_entry_set_visibility(e->entry,FALSE);
    if(e->icon)
    {
        switch(e->icon_pos){
            case 1:
                gtk_entry_set_icon_from_stock(e->entry,GTK_ENTRY_ICON_PRIMARY,e->icon);
                break;
            case 2:
                gtk_entry_set_icon_from_stock(e->entry,GTK_ENTRY_ICON_SECONDARY,e->icon);
                break;
            default:break;
        }
    }
    //hauteur et largeur
    if((e->width>=0)&&(e->height>=0))
        gtk_widget_set_size_request(e->entry,e->width,e->height);
}

const gchar *get_entry_text(EntryObj *entry_obj) {
    // Récupérer le texte saisi dans l'entrée
    const gchar *text = gtk_entry_get_text(entry_obj);

    return text;
}
#endif //TEST_GTK_ENTRYHEADER_H
