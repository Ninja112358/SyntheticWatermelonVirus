#include "mybtn.h"

myBtn::myBtn(QWidget *parent) : QPushButton(parent)
{
}

void myBtn::enterEvent(QEvent *event)
{
    this->setStyleSheet("border: 4px solid rgb(45, 45, 45);"
                        "border-radius: 0px;"
                        "color: rgb(52, 52, 52);"
                        "background-color: rgb(129, 129, 129);");
    return QPushButton::enterEvent(event);
}

void myBtn::leaveEvent(QEvent *ev)
{
    this->setStyleSheet("border: 4px solid rgb(45, 45, 45);"
                        "border-radius: 0px;"
                        "color: rgb(52, 52, 52);"
                        "background-color: rgb(255, 255, 255);");
    return QPushButton::leaveEvent(ev);
}
