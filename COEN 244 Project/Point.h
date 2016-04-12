#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <iostream>

class Point
{
private:
    float m_x;
    float m_y;

public:
    Point();
    Point(float x, float y);

    void setX(float x);
    float getX() const;

    void setY(float y);
    float getY() const;

    void setPoint(float x, float y);

    friend std::ostream& operator<<(std::ostream& out, const Point& point);

};

std::ostream& operator<<(std::ostream& out, const Point& point);

#endif // POINT_H_INCLUDED
