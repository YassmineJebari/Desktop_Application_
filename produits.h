#ifndef PRODUITS_H
#define PRODUITS_H
#include <QString>
#include <QSqlQueryModel>
class Produits
{
public:
    // constructeurs

    Produits();
    Produits(int,QString ,int  ,QString,QString,int,QString,float);

    //getters
    int getidproduit();
    QString getnom();
    QString gettype();
    int getquantite();
    QString getdisponibilite();

    //setters
    float getprix();
    void setidproduit(int);
    void setnom(QString);
    void settype(QString);
    void setquantite(int);
    void setdiponibilite(QString);
    void setprix(float);
    //fonctions relatives a lentite produits
    bool ajouter();
    bool supprimer (int idproduit);
    QSqlQueryModel *afficher();
    bool modifier_prod(int idproduit);
    bool chercherexistid(int);
    bool chercherexistcin(int );
   // bool chercherexistemail(QString);



    QSqlQueryModel * trie_ID();
    QSqlQueryModel * trie_NOM();
    QSqlQueryModel * trie_PRIX();

    QSqlQueryModel* rechercheridproduit(QString);
    QSqlQueryModel* recherchernom(QString);
    QSqlQueryModel* rechercherdisponibilite(QString);
    QSqlQueryModel* recherchertype(QString);
      int statistique(QString);


private:
    int idproduit;
    int ID_CLIENT;
    QString idemploye;
    QString nom;
    QString type;
    int quantite;
    QString disponibilite;
    float prix;




};

#endif // PRODUITS_H
