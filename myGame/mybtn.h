#ifndef MYBTN_H
#define MYBTN_H

#include <QObject>
#include <QPushButton>

class myBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit myBtn(QWidget *parent = nullptr);
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *ev) override;
signals:

};

#endif // MYBTN_H
