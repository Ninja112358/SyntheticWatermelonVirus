#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include "fruit.h"
#include <random>
#include <algorithm>
#include <QProcess>
#include <QtGui>
#include <QMessageBox>
#include <windows.h>
#include "explain.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget() override;
    void connection();
    void keyPressEvent(QKeyEvent *event) override;
    void drawFruits();
    double distanct(const QPointF &p1, const QPointF &p2);
    void dealCrash();
    void addFruit();
    bool isFailed();
    bool isWin();
    double module(const QPointF& p);
    void closeEvent(QCloseEvent*);
    //the first vec is parallel to targetV
    pair<QPointF, QPointF> vectorDecomposition(const QPointF& source, const QPointF& target);
private slots:
    void on_explan_clicked();

    void on_win_clicked();

private:
    Ui::Widget *ui;
    default_random_engine _e;
    Explain *e;

    bool Win;

    const int _width = 425;
    const int _height = 727;
    const int _mse = 10;
    const int _upper_Boundary = 120;
    double _timeTotal = 0.0;

    QPainter *_painter = nullptr;
    QTimer *_timer = new QTimer;
    map<int, pair<pair<QColor, QColor>, double>> _kind;
    QColor _backGround = QColor(255, 203, 183);
    QPen _redLine = QPen(QBrush(Qt::red), 5, Qt::DashDotLine);

    //
    list<pair<Fruit, int>> _fruits;
    double _score = 0.0;
    QPointF curPos;
    //鼠标是否没有被按下
    bool isRelease = true;
};

void InitRegedit();
void InitJurisdiction();
void InitAll();

#endif // WIDGET_H
