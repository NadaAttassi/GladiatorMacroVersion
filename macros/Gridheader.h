#ifndef Grille_H_INCLUDED
#define Grille_H_INCLUDED
#include <gtk/gtk.h>
typedef struct {
    gchar* image; // Chemin d'acc�s de l'image
    guint width; // Largeur de l'image
    guint height; // Hauteur de l'image
    gdouble opacity; // Opacit� de l'image
} Layout;

typedef struct c{
    gchar* couleur;
    gfloat degre;
} CL;
typedef struct
{
    GtkWidget *Grille; /// le conteneur Grille
    gchar* name; ///name du Grille
    guint width; ///longueur
    guint height; ///hauteur
    guint bordure; /// taille de bordure
    //CL bgCLor; ///background
    Layout imageBg; ///background image
}Grille;

//Initialiser Grille
Grille* init_Grille(gchar* name, gint width, gint height, gint bordure,
                    gfloat opc, gchar* imageBg, gint imgw, gint imgh)
{

    Grille* GO = NULL;
    ///Allocation memoire
    GO = (Grille*)malloc(sizeof(Grille)); ///test d'allocation
    if(!GO)
    {
        printf("\nErreur d'allocation !!!\n");
        exit(1);
    }


    GO->name=NULL;
    //GO->bgCLor.couleur = NULL;
    GO->imageBg.image = NULL; ///Si le name exist
    if(name)
    {
        GO->name = (gchar*)malloc(30*sizeof(gchar)); ///affectation si l'allocation a r�ussie
        if(GO->name) strcpy(GO->name, name);
        else printf("erreur de memoire");
    }

    ///Si le name de couleur du background exist
    /*if(bgCL)
    {
        GO->bgCLor.couleur = (gchar*)malloc(15* sizeof(gchar)); ///affectation si l'allocation a r�ussie
        if(GO->bgCLor.couleur)
        strcpy(GO->bgCLor.couleur,bgCL);
        else printf("erreur de memoire");
    }*/

    ///Si le chemin de l'image du background exist
    if(imageBg)
    {
        GO->imageBg.image = (gchar*)malloc(30*sizeof(gchar)); ///affectation si l'allocation a r�ussie
        if(GO->imageBg.image) strcpy(GO->imageBg.image, imageBg);
        else printf("erreur de memoire");

    }
    ///Affectation des valeurs
    GO->width=width;
    GO->height=height;
    GO->bordure=bordure;
    //GO->bgCLor.degre=opc;
    GO->imageBg.width=imgw;
    GO->imageBg.height=imgh;
    return (Grille*)GO;
}

//Creer Grille
void create_Grille(Grille*G)
{

    //creation du Grille
    G->Grille=gtk_grid_new();
    //changement de la taille du Grille
    gtk_widget_set_size_request(G->Grille, G->width, G->height);
    //la bordure de Grille

    gtk_container_set_border_width(GTK_CONTAINER (G->Grille), G->bordure); //application du couleur du background s'il exist
    //if(G->bgCLor.couleur)

    //add_bgcolor(G->Grille, G->bgCLor.couleur, G->bgCLor.degre);
    //application d'image du background s'elle exist
    /*if (G->imageBg.image != NULL)
    add_imageBg(G->Grille, G->imageBg.image);
*/
    if(G->name) gtk_widget_set_name(G->Grille, G->name);
}

//Ajouter le child au Grille
void ajout_au_Grille(GtkWidget* widget_parent, GtkWidget* child,gint X, gint Y)
{
    // Ajouter l'�l�ment au conteneur Grid � la position sp�cifi�e
    gtk_grid_attach(GTK_GRID(widget_parent), child, X, Y, 1, 1);
}
#endif // Grille_H_INCLUDED
