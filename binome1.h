#ifndef BINOME1_H
#define BINOME1_H

#include <QDialog>

#include "arduino.h"
#include <QDialog>
#include <QDebug>

namespace Ui {
class binome1;
}

class binome1 : public QDialog
{
    Q_OBJECT

public:
    explicit binome1(QString ,QWidget *parent = nullptr);
       ~binome1();
   void  chercher(QString idp);
private:
    Ui::binome1 *ui;
     Arduino A; // objet temporaire
};

#endif // BINOME1_H
