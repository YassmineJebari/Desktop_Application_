#ifndef CALCULATRICE_H
#define CALCULATRICE_H
#include <QAbstractButton>
#include <QDialog>

namespace Ui {
class Calculatrice;
}

class Calculatrice : public QDialog
{
    Q_OBJECT

public:
    explicit Calculatrice(QWidget *parent = nullptr);
    ~Calculatrice();

private slots:
  //void on_pushButton_r_clicked();

private:
     Ui::Calculatrice *ui;

private slots :
    void NumPressed();
    void MathButtonPressed();
    void EqualButtonPressed();
    void ChangeNumberSign();
    void on_pushButton_Q_clicked();


    void on_Clear_clicked();
};

#endif // CALCULATRICE_H
