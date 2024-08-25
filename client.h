#ifndef CLIENT_H
#define CLIENT_H
#include <QSqlQuery>
#include <QSqlQueryModel>

class client
{

public:
   //constructeurs
    client();
    client(int,QString,QString,QString,int,QString);
    //getters
    int getCin()
    {
        return cin;
    }
    QString getNom()
    {
        return nom;
    }
    QString getPreom()
    {
        return prenom;
    }
    QString getEmail()
    {
        return email;
    }
    int getPhone()
    {
        return phone;
    }
    QString getRegion()
    {
        return region;
    }
    // Setters
    void setCin(int cin)
    {
        this->cin=cin;
    }
    void setNom(QString nom)
    {
        this->nom=nom;
    }
    void setPrenom(QString prenom)
    {
        this->prenom=prenom;
    }
    void setEmail(QString email)
    {
        this->email=email;
    }
    void setPhone(int phone)
    {
        this->phone=phone;
    }
    void setRegion(QString region)
    {
        this->region=region;
    }

   // les fonctions
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int cin);
    bool modifier(int cin);
    bool chercherexistid(int cin);
QSqlQueryModel* afficherArd();
private:
    int cin;
   QString nom;
   QString prenom;
   QString email;
    int phone;
    QString region;

};

#endif // CLIENT_H
