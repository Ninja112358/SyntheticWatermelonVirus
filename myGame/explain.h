#ifndef EXPLAIN_H
#define EXPLAIN_H

#include <QDialog>
#include <cmath>
#include <windows.h>
#include <QCloseEvent>
#include "problem.h"

namespace Ui {
class Explain;
}

class Explain : public QDialog
{
    Q_OBJECT

public:
    char nb[200];
    explicit Explain(QWidget *parent = 0);
    ~Explain();

    void closeEvent(QCloseEvent*);
    double pow_count(int x,double count);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Problem *problem;
    Ui::Explain *ui;
    double result;

};

#endif // EXPLAIN_H
