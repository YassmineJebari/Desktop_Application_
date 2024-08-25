#ifndef HISTORIQUE_H
#define HISTORIQUE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Historique
{
    int NUMBORNE,CAPACITE,POURCENTAGE;

public:
    Historique(int,int,int);
    Historique();

    //geters
    int getNumborne(){return NUMBORNE;}
    int getCapacite(){return CAPACITE;}
    int getPourcentage(){return POURCENTAGE;}
    //seters
    void setNumborne(int n){NUMBORNE=n;}
    void setCapacite(int n){CAPACITE=n;}
    void setPourcentage(int n){POURCENTAGE=n;}
    //controledesaisie
   // bool chercher(int);
    //crud
    bool ajouter();
    bool supprimer(int);
    float calculertemps(int);

};

#endif // HISTORIQUE_H
