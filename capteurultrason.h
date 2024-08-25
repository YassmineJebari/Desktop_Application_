#ifndef CAPTEURULTRASON_H
#define CAPTEURULTRASON_H
#include "arduino.h"
#include <deque>
#include <QDialog>
#include "borne.h"
#include "historique.h"
#include <QPropertyAnimation>
#include "deuxiemeinterface.h"
#include <QRect>
namespace Ui {
class Capteurultrason;
}

class Capteurultrason : public QDialog
{
    Q_OBJECT

public:
    explicit Capteurultrason(QWidget *parent = nullptr);
    ~Capteurultrason();
private slots:
    void onMsg();
    void onDialogFinished(int x);
private:
    int calculerplace();
    bool cherchermain(int numborne);
    Ui::Capteurultrason *ui;
    std::deque<int> lastPlateValues;
        QByteArray data;
        Arduino A; // objet temporaire
            int safeZonePlate = 8;
            int lastDist = -1;
            QString etatborne;
            QPropertyAnimation *animation;
            deuxiemeinterface *deuxieme;
};

#endif // CAPTEURULTRASON_H
