#ifndef SETOFPOINTS_H_INCLUDED
#define SETOFPOINTS_H_INCLUDED

#include <vector>
#include "Point.h"

class SetOfPoints
{
public:

    static const float BOUND;
    enum PlotType
    {
        EVERYWHERE,
        TOP_HALF,
        BOTTOM_HALF,
        RIGHT_HALF,
        LEFT_HALF,
        SINGLE_QUADRANT,
        DIAGONAL_QUADRANTS,
        THREE_QUADRANTS,
        LINEAR,
        PARABOLA,
        CUBIC
    };

    SetOfPoints();
    void generateRandPoints(PlotType plotType);
    Point& operator[](int index);
    int size() const;

    friend std::ostream& operator<<(std::ostream& out, SetOfPoints& set);

private:
    std::vector<Point> pointList;
    int m_numPoints;
    static const int MIN_NUM_POINTS;
    static const int MAX_NUM_POINTS;

    void generateEverywhere();
    void generateTopHalf();
    void generateBottomHalf();
    void generateRightHalf();
    void generateLeftHalf();
    void generateOnQuadrant(int quadrant = 0);
    void generateLinear();
    void generateParabola();
    void generateCubic();
};

std::ostream& operator<<(std::ostream& out, SetOfPoints& set);

#endif // SETOFPOINTS_H_INCLUDED
