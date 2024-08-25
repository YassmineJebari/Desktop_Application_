#ifndef INTEFACE_PRODUIT_H
#define INTEFACE_PRODUIT_H
#include "arduino.h"
#include <QWidget>
#include "produits.h"
#include <QMainWindow>
#include <QAbstractButton>
#include <QDialog>
#include <QString>
#include <QSqlQuery>
#include<QtDebug>
#include<QObject>
#include<QDateTime>
#include<QPrinter>
#include<QPainter>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include "calculatrice.h"
#include <QSortFilterProxyModel>
#include "binome1.h"
namespace Ui {
class inteface_produit;
}

class inteface_produit : public QWidget
{
    Q_OBJECT

public:
    explicit inteface_produit(QWidget *parent = nullptr);
    ~inteface_produit();


    explicit inteface_produit(QString,QString,QWidget *parent = nullptr);

    void show_tables();



private slots:
    void on_add_clicked();
    //void on_delete_clicked();
   //void stat();
    void on_delete_2_clicked();

    void on_modifier_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_activated(const QModelIndex &index);

    //void on_pushButton_5_clicked();

    void on_calcul_clicked();

   // void on_pushButton_Recherche_clicked();

    //void on_pushButton_trie_clicked();

    void on_pdf_clicked();

    //void on_pdf1_clicked();

    void on_pb_qr_clicked();



       void on_le_rech_textChanged(const QString &arg1);

       void on_stat_clicked();
       void update_label();
       void notifer();
       void onDialogFinished(int x);



       void on_clients_clicked();

       void on_employees_clicked();

       void on_stations_clicked();

       void on_lougout_clicked();

       void on_rdv_clicked();

private:
    Ui::inteface_produit *ui;
    Produits Etmp;
    int selected=0;
    QRegExp cin_regex,chaine_regex,tel_regex,mail_regex,salaire_regex,horaire_regex ;
    Produits p;
    QSortFilterProxyModel *proxy;
    Calculatrice *calc;



    QString current_user;
    QString current_id;

    QByteArray data; // variable contenant les données reçues
    QByteArray datafinal; // variable contenant les données reçues
    QString data_text;
    Arduino A; // objet temporaire
   binome1 *b;



};

#endif // INTEFACE_PRODUIT_H
