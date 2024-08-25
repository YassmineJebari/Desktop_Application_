#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "login.h"
#include "application.h"
#include "ui_application.h"
#include <QMainWindow>
#include "arduino.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
   // QString getcurrent_user(){return current_user;}
  //  void setuser(QString username){ this->username=username;}
   // void setemail (QString email){ this->email=email;}





private slots:
    void update_label();   // slot permettant la mise à jour du label état de la lampe 1,

    void on_exist_clicked();

    void on_enter_clicked();

    void on_pushButton_2_clicked();

    void on_enterp_clicked();

    void on_signp_clicked();
    void onDialogFinished(int x);

private:
    Ui::MainWindow *ui;

    login *log;
    bool test=false;
    QString username;


    QRegExp mail_regex ;


    QByteArray data; // variable contenant les données reçues
    QByteArray datafinal; // variable contenant les données reçues
    QString data_text;
    Arduino A; // objet temporaire
    application * a;

};
#endif // MAINWINDOW_H
