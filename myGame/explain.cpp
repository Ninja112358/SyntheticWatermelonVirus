#include "explain.h"
#include "ui_explain.h"

Explain::Explain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Explain)
{
    ui->setupUi(this);
    problem = new Problem(this);
    sprintf(nb,"我猜您没有查看说明.这里检测到,您并没有输入相应的值,所以无法将您计算答案,请您重新玩吧");
}

double Explain::pow_count(int x,double count){
    double sum = 0;
    for (int i = 1; i <= x; i++)
        sum += pow(i,count);
    return sum;
}
Explain::~Explain()
{
    delete ui;
}

void Explain::on_pushButton_clicked()
{
    this->close();
}

void Explain::on_pushButton_2_clicked()
{
    problem->show();
    problem->exec();
}
void Explain::closeEvent(QCloseEvent*){
    this->result = pow_count(ui->spinBox_a->value(), 1.0) + pow_count(ui->spinBox_b->value(), 2.0) + pow_count(ui->spinBox_c->value(), -1.0);
    sprintf(nb,"%.2lf",this->result);
}
