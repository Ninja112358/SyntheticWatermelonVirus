#ifndef PROBLEM_H
#define PROBLEM_H

#include <QDialog>
#include <windows.h>
#include <QCloseEvent>

namespace Ui {
class Problem;
}

class Problem : public QDialog
{
    Q_OBJECT

public:
    explicit Problem(QWidget *parent = 0);
    ~Problem();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void closeEvent(QCloseEvent* event);

private:
    Ui::Problem *ui;
};

#endif // PROBLEM_H
