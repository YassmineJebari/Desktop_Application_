#ifndef BORNE_H
#define BORNE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>


class Borne
{   int numBorne,intensiteBorne;
    QString imageBorne,etatBorne,idEmployee;
   // QDate dateCreation;

public:
    //const
    Borne(QString,int,int,QString,QString);
    Borne();

    //geters
    QString getidEmployeee(){return idEmployee;}
    int getNumBorne(){return numBorne;}
    int getIntensiteBorne(){return intensiteBorne;}
    QString getImageBorne(){return imageBorne;}
    QString getEtatBorne(){return etatBorne;}
    //seters
    void setidEmployee(int n){idEmployee=n;}
    void setNumBorne(int n){numBorne=n;}
    void setIntensiteBorne(int n){intensiteBorne=n;}
    void setImageBorne(QString i){imageBorne=i;}
    void setEtatBorne(QString i){etatBorne=i;}
    //controledesaisie
    bool chercher(int);
    //crud
    bool ajouter();
    bool supprimer(int);
    QSqlQueryModel * afficher();
    bool update(int,int,QString,QString);

};

#endif // BORNE_H
