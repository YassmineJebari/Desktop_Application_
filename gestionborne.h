#ifndef GESTIONBORNE_H
#define GESTIONBORNE_H
#include "borne.h"
#include <QString>
#include <QTableView>
#include "deuxiemeinterface.h"
#include "capteurultrason.h"
#include "arduino.h"
#include<QDebug>
#include <QWidget>

namespace Ui {
class gestionBorne;
}

class gestionBorne : public QWidget
{
    Q_OBJECT

public:
    explicit gestionBorne(QWidget *parent = nullptr);
    ~gestionBorne();
    explicit gestionBorne(QString,QString,QWidget *parent = nullptr);
    void on_pushButton_ajouter_clicked();
    bool cherchermain(int);
    //void saveTableViewToPdf(QTableView *tableView, QString title, QString fileName);
private slots:
    void on_add_clicked();
    //void on_delete_2_clicked();

    void on_update_clicked();

    void on_delete_2_clicked();

    void on_pushButton_5_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_lineEdit_recherche_textChanged(const QString &arg1);

    //void tri();

    void on_pdf_clicked();

    void on_historique_clicked();

    void on_pushButton_2_clicked();
void notifer();
void onDialogFinished(int);


void on_logout_clicked();

void on_employees_clicked();

void on_clients_clicked();

void on_products_clicked();

void on_rdv_clicked();

private:
    Ui::gestionBorne *ui;
    void show_tables();
    Borne Etmp;
    QString image;
    bool verifierint(int,int);
    QByteArray data;
    Arduino A;
    deuxiemeinterface *dinter;
    Capteurultrason *capt;
    int calculerplace();
    QString current_user;
    QString current_id;
};

#endif // GESTIONBORNE_H
