#ifndef FRUIT_H
#define FRUIT_H
#include <QPointF>

class Fruit
{
public:
    Fruit(const double &Boundary, const double &w,
          const double &h, const int &mse, const double r);
    QPointF& upDatePos();
    const double& getV_Right() const;
    const double& getV_Down() const;
    QPointF getV() const;
    const double& getR() const;
    const QPointF& getPos() const;
    const bool& IsActive() const;
    Fruit& setV_Right(const double &vR);
    Fruit& setV_Down(const double &vD);
    Fruit& setV(const QPointF &v);
    Fruit& setPos(const QPointF &pos);
    Fruit& setR(const double &r);
    void setActive(const bool & b);
private:
    const double _a_Down = 1000.0;
    const double _a_Friction = 40.0;
    const double _upper_Boundary;
    const double _width;
    const double _height;
    //
    QPointF _pos;
    double _sed;
    double _v_Right = 0.0;
    double _v_Down = 0.0;
    bool isActive = false;
    //
    double _r;
};

#endif // FRUIT_H
