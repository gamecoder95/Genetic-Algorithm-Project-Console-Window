#include "SetOfPoints.h"
#include "Controller.h"
#include <iostream>

const int SetOfPoints::MIN_NUM_POINTS = 20;
const int SetOfPoints::MAX_NUM_POINTS = 100;
const float SetOfPoints::BOUND = 200;

SetOfPoints::SetOfPoints()
{
    PlotType plotType = static_cast<PlotType>(Controller::getRandNumInRange(SetOfPoints::EVERYWHERE, SetOfPoints::CUBIC));
    std::cout<<plotType<<std::endl;
    generateRandPoints(plotType);
}

Point& SetOfPoints::operator[](int index)
{
    //Put exception handling here
    return pointList[index];
}

int SetOfPoints::size() const
{
    return pointList.size();
}

void SetOfPoints::generateRandPoints(SetOfPoints::PlotType plotType)
{
    pointList.clear();
    m_numPoints = Controller::getRandNumInRange(MIN_NUM_POINTS, MAX_NUM_POINTS);
    switch(plotType)
    {
    case SetOfPoints::EVERYWHERE:
        generateEverywhere();
        break;

    case SetOfPoints::TOP_HALF:
        generateTopHalf();
        break;

    case SetOfPoints::BOTTOM_HALF:
        generateBottomHalf();
        break;

    case SetOfPoints::RIGHT_HALF:
        generateRightHalf();
        break;

    case SetOfPoints::LEFT_HALF:
        generateLeftHalf();
        break;

    case SetOfPoints::SINGLE_QUADRANT:
        generateOnQuadrant(0);
        break;

    case SetOfPoints::DIAGONAL_QUADRANTS:
    case SetOfPoints::THREE_QUADRANTS:
        {

            int firstQuad = Controller::getRandNumInRange(0, 1);
            generateOnQuadrant(firstQuad);
            generateOnQuadrant(firstQuad + 2); // The one diagonally accross from it.
            if(plotType == SetOfPoints::THREE_QUADRANTS)
            {
                if(Controller::getRandNumInRange(0, 1) == 0)
                {
                    generateOnQuadrant(firstQuad + 1);
                }
                else
                {
                    if(firstQuad + 3 == 4)
                    {
                        generateOnQuadrant(0);
                    }
                    else
                    {
                        generateOnQuadrant(firstQuad + 3);
                    }

                }
            }
        }
        break;

    case SetOfPoints::LINEAR:
        generateLinear();
        break;

    case SetOfPoints::PARABOLA:
        generateParabola();
        break;

    case SetOfPoints::CUBIC:
        generateCubic();
        break;

    default:
        break;
    }

}

void SetOfPoints::generateEverywhere()
{
    int pointType = 0;
    for(int i = 0; i < m_numPoints; ++i)
    {
        pointType = Controller::getRandNumInRange(0, 10); // A randm number to increase variety in the points.
        switch(pointType)
        {
        case 0:
            // A point anywhere.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), Controller::getRandFloatInRange(-BOUND, BOUND)));
            break;

        case 1:
            //A point on the y-axis.
            pointList.push_back(Point(0.0f, Controller::getRandFloatInRange(-BOUND, BOUND)));
            break;

        case 2:
            //A point on the x-axis.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), 0.0f));
            break;

        case 3:
            //A point on the right half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(0.0f, BOUND), Controller::getRandFloatInRange(-BOUND, BOUND)));
            break;

        case 4:
            // A point on the left half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, 0.0f), Controller::getRandFloatInRange(-BOUND, BOUND)));
            break;

        case 5:
            // A point on the top half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), Controller::getRandFloatInRange(0.0f, BOUND)));
            break;

        case 6:
            // A point on the bottom half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), Controller::getRandFloatInRange(-BOUND, 0.0f)));
            break;

        case 7:
            // A point on the top-right quadrant.
            pointList.push_back(Point(Controller::getRandFloatInRange(0.0f, BOUND), Controller::getRandFloatInRange(0.0f, BOUND)));
            break;

        case 8:
            // A point on the top-left quadrant.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, 0.0f), Controller::getRandFloatInRange(0.0f, BOUND)));
            break;

        case 9:
            // A point on the bottom-left quadrant.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, 0.0f), Controller::getRandFloatInRange(-BOUND, 0.0f)));
            break;

        case 10:
            // A point on the bottom-right quadrant.
            pointList.push_back(Point(Controller::getRandFloatInRange(0.0f, BOUND), Controller::getRandFloatInRange(-BOUND, 0.0f)));
            break;

        default:
            break;
        }
    }
}

void SetOfPoints::generateTopHalf()
{
    for(int i = 0; i < m_numPoints; ++i)
    {
        // 10% chance that the point will be on the x axis.
        if(Controller::getRandNumInRange(1, 10) > 1)
        {
            // Generate on bottom half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), Controller::getRandFloatInRange(0.0f, BOUND)));
        }
        else
        {
            //Generate on the x-axis.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), 0.0f));
        }
    }
}

void SetOfPoints::generateRightHalf()
{
    for(int i = 0; i < m_numPoints; ++i)
    {
        // 10% chance that the point will be on the y axis.
        if(Controller::getRandNumInRange(1, 10) > 1)
        {
            // Generate on right half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(0.0f, BOUND), Controller::getRandFloatInRange(-BOUND, BOUND)));
        }
        else
        {
            //Generate on the y-axis.
            pointList.push_back(Point(0.0f, Controller::getRandFloatInRange(-BOUND, BOUND)));
        }
    }
}

void SetOfPoints::generateLeftHalf()
{
    for(int i = 0; i < m_numPoints; ++i)
    {
        // 10% chance that the point will be on the y axis.
        if(Controller::getRandNumInRange(1, 10) > 1)
        {
            // Generate on left half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, 0.0f), Controller::getRandFloatInRange(-BOUND, BOUND)));
        }
        else
        {
            //Generate on the y-axis.
            pointList.push_back(Point(0.0f, Controller::getRandFloatInRange(-BOUND, BOUND)));
        }
    }
}

void SetOfPoints::generateBottomHalf()
{
    for(int i = 0; i < m_numPoints; ++i)
    {
        // 10% chance that the point will be on the x axis.
        if(Controller::getRandNumInRange(1, 10) > 1)
        {
            // Generate on bottom half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), Controller::getRandFloatInRange(-BOUND, 0.0f)));
        }
        else
        {
            //Generate on the x-axis.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), 0.0f));
        }
    }
}

void SetOfPoints::generateOnQuadrant(int quadrant)
{
    while(quadrant < 0)
    {
        quadrant += 4;
    }

    while(quadrant > 3)
    {
        quadrant -= 4;
    }

    for(int i = 0; i < m_numPoints; ++i)
    {
        switch(quadrant)
        {
            case 0:
                // A point on the top-right quadrant.
                pointList.push_back(Point(Controller::getRandFloatInRange(0.0f, BOUND), Controller::getRandFloatInRange(0.0f, BOUND)));
                break;

            case 1:
                // A point on the top-left quadrant.
                pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, 0.0f), Controller::getRandFloatInRange(0.0f, BOUND)));
                break;

            case 2:
                // A point on the bottom-left quadrant.
                pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, 0.0f), Controller::getRandFloatInRange(-BOUND, 0.0f)));
                break;

            case 3:
                // A point on the bottom-right quadrant.
                pointList.push_back(Point(Controller::getRandFloatInRange(0.0f, BOUND), Controller::getRandFloatInRange(-BOUND, 0.0f)));
                break;

            default:
                break;
        }
    }
}

void SetOfPoints::generateLinear()
{
    float slope = Controller::getRandFloatInRange(-5, 5);
    for(int i = 0; i < m_numPoints/2; ++i)
    {
        float x = static_cast<float>(5 * i);
        float y = slope * x;
        pointList.push_back(Point(x, y + Controller::getRandFloatInRange(-25, 25)));

        if(i != 0)
        {
            pointList.push_back(Point(-x, -y + Controller::getRandFloatInRange(-25, 25)));
        }
    }
}

void SetOfPoints::generateParabola()
{
    float a = Controller::getRandFloatInRange(-5, 5);
    float h = Controller::getRandFloatInRange(-5, 5);
    float k = Controller::getRandFloatInRange(-5, 5);

    for(int i = 0; i < m_numPoints/2; ++i)
    {
        float x = static_cast<float>(25 * i);
        float y = a * (x - h)*(x - h) + k;
        pointList.push_back(Point(x, y + Controller::getRandFloatInRange(-25, 25)));

        if(i != 0)
        {
            pointList.push_back(Point(-x, y + Controller::getRandFloatInRange(-25, 25)));
        }
    }
}

void SetOfPoints::generateCubic()
{
    float a = Controller::getRandFloatInRange(-5, 5);
    float b = Controller::getRandFloatInRange(-5, 5);
    float c = Controller::getRandFloatInRange(-5, 5);
    float d = Controller::getRandFloatInRange(-5, 5);

    for(int i = 0; i < m_numPoints/2; ++i)
    {
        float x = static_cast<float>(25 * i);
        float y1 = a*x*x*x + b*x*x + c*x + d;
        float y2 = -a*x*x*x + b*x*x - c*x + d;
        pointList.push_back(Point(x, y1 + Controller::getRandFloatInRange(-25, 25)));

        if(i != 0)
        {
            pointList.push_back(Point(-x, y2 + Controller::getRandFloatInRange(-25, 25)));
        }
    }
}

std::ostream& operator<<(std::ostream& out, SetOfPoints& set)
{
    out<<std::endl;
    if(set.pointList.empty())
    {
        out<<"This set of points is empty!"<<std::endl;
    }
    else
    {
        out<<"This set of points contains "<<set.m_numPoints<<" points:"<<std::endl;
        for(int i = 0; i < set.size(); ++i)
        {
            out<<set[i]<<std::endl;
        }
    }

    out<<std::endl;
    return out;
}
