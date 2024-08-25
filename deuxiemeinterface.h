#ifndef DEUXIEMEINTERFACE_H
#define DEUXIEMEINTERFACE_H

#include <QDialog>
#include "historique.h"
#include<QMap>
#include "borne.h"
#include "arduino.h"
#include<QTimer>
namespace Ui {
class deuxiemeinterface;
}

class deuxiemeinterface : public QDialog
{
    Q_OBJECT

public:
    explicit deuxiemeinterface(QWidget *parent = nullptr);
    explicit deuxiemeinterface(QString,QWidget *parent = nullptr);

    ~deuxiemeinterface();

private slots:
    void on_readfile_clicked();

    void on_activer_clicked();
    void onTimerTimeout();

private:
    QString etatBorne;
    int intensiteBorne;
    Ui::deuxiemeinterface *ui;
    QMap<int,QTimer *>tab;
    Historique historique;
    bool verifierint(int ,int );
    bool cherchermain(int);
    bool chercherhistorique(int );
    void ecrirefichier(int);
    void statistique();
    int calculerplace();
    Arduino A;

};

#endif // DEUXIEMEINTERFACE_H
