#include "fruit.h"

Fruit::Fruit(const double &Boundary, const double &w, const double &h, const int &mse, const double r)
    : _upper_Boundary(Boundary), _width(w), _height(h), _sed(mse * 0.001), _r(r)
{
    this->_pos = QPointF(
                     0.5 * this->_width,
                     0.5 * this->_upper_Boundary
                 );
}

QPointF &Fruit::upDatePos()
{
    if(this->isActive == false) {
        return this->_pos;
    }
    //v-dir : dh = v0*dt + 0.5*a*dt*dt
    this->_v_Down += this->_a_Down * this->_sed;
    // small enough
    if(abs(this->_v_Down) <= 10.0 && this->_pos.y() > (this->_height - 1.1 * this->_r)) {
        this->_v_Down = 0.0;
    }
    //the range
    if(this->_pos.y() > this->_height - this->_r) {
        this->_pos.setY(this->_height - this->_r);
        this->_v_Down *= -0.3;
    } else {
        this->_pos.setY(
            this->_pos.y() + this->_v_Down * this->_sed + 0.5 * this->_a_Down * this->_sed * this->_sed
        );
    }
    //h-dir : dx = v0*dt
    if(this->_v_Right > 10.0) {
        this->_v_Right += -this->_a_Friction * this->_sed;
    } else if(this->_v_Right < -10.0) {
        this->_v_Right += this->_a_Friction * this->_sed;
    }
    //水平速度过小直接置0.0
    if(abs(this->_v_Right) <= 10.0) {
        this->_v_Right = 0.0;
    }
    //the range
    if(this->_pos.x() < this->_r ) {
        this->_pos.setX(this->_r);
        this->_v_Right *= -0.3;
    } else if(this->_pos.x() > this->_width - this->_r) {
        this->_pos.setX(this->_width - this->_r);
        this->_v_Right *= -0.3;
    } else {
        this->_pos.setX(
            this->_pos.x() + this->_v_Right * this->_sed
        );
    }
    return this->_pos;
}

const double& Fruit::getV_Right() const
{
    return this->_v_Right;
}

const double& Fruit::getV_Down() const
{
    return this->_v_Down;
}

QPointF Fruit::getV() const
{
    return QPointF(this->_v_Right, this->_v_Down);
}

const double &Fruit::getR() const
{
    return this->_r;
}

const QPointF &Fruit::getPos() const
{
    return this->_pos;
}

const bool &Fruit::IsActive() const
{
    return this->isActive;
}

Fruit &Fruit::setV_Right(const double &vR)
{
    this->_v_Right = vR;
    return *this;
}

Fruit &Fruit::setV_Down(const double &vD)
{
    this->_v_Down = vD;
    return *this;
}

Fruit &Fruit::setV(const QPointF &v)
{
    this->_v_Right = v.x();
    this->_v_Down = v.y();
    return *this;
}

Fruit &Fruit::setPos(const QPointF &pos)
{
    this->_pos = pos;
    return *this;
}

Fruit &Fruit::setR(const double &r)
{
    this->_r = r;
    return *this;
}

void Fruit::setActive(const bool &b)
{
    this->isActive = b;
}
