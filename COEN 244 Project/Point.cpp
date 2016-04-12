#include "Point.h"

Point::Point() : Point(0.0f, 0.0f)
{
    //Creates a point at origin (0, 0)
}

Point::Point(float x, float y)
{
    setPoint(x, y);
}

void Point::setPoint(float x, float y)
{
    setX(x);
    setY(y);
}

void Point::setX(float x)
{
    m_x = x;
}

void Point::setY(float y)
{
    m_y = y;
}

float Point::getX() const
{
    return m_x;
}

float Point::getY() const
{
    return m_y;
}

std::ostream& operator<<(std::ostream& out, const Point& point)
{
    out<<"("<<point.m_x<<", "<<point.m_y<<")";
    return out;
}
