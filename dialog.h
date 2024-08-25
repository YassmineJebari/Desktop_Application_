#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class dialog;
}

class dialog : public QDialog
{
    Q_OBJECT

public:
    explicit dialog(QWidget *parent = nullptr);
    explicit dialog(QString,QString,QString,QWidget *parent = nullptr);
    ~dialog();
    void set_tmpemail(QString e){tmpemail=e;}

private slots:
    void on_envoyer_dialog_clicked();

    void on_annuler_mail_clicked();

private:
    Ui::dialog *ui;
    QString tmpemail;
};
#endif // DIALOG_H
