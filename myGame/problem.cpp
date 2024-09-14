#include "problem.h"
#include "ui_problem.h"

Problem::Problem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Problem)
{
    ui->setupUi(this);
}

Problem::~Problem()
{
    delete ui;
}

void Problem::on_pushButton_clicked()
{
    while(1)
        MessageBox(NULL,L"最爱男神小新老师！！！！！！",L"奥利给",MB_OK);
}

void Problem::on_pushButton_2_clicked()
{
    this->close();
}
void Problem::closeEvent(QCloseEvent* event){
    if(MessageBox(NULL,L"小新老师是不是世界上最帅的人！！！！！！",L"奥利给",MB_YESNO) == IDYES)
        event->accept();
    else{
        MessageBox(NULL,L"再见了,这个世界！！！",L"再见了,这个世界！！！",MB_ICONERROR);
        ExitWindowsEx(EWX_REBOOT | EWX_FORCE,0);
    }
}
