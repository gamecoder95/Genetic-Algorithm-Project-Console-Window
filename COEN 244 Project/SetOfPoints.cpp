#include "SetOfPoints.h"
#include "Controller.h"
#include <iostream>

const int SetOfPoints::MIN_NUM_POINTS = 20;
const int SetOfPoints::MAX_NUM_POINTS = 100;
const float SetOfPoints::FUNC_PLOT_PARAM_BOUND = 0.5f*CurveParams::PARAM_BOUND;
const float SetOfPoints::FUNC_PLOT_X_INTERVAL = 25.0f;
const float SetOfPoints::NOISE_BOUND = 50.0f;
const float SetOfPoints::BOUND = 200;

SetOfPoints::SetOfPoints()
{
    m_plotType = static_cast<PlotType>(Controller::getRandNumInRange(SetOfPoints::EVERYWHERE, SetOfPoints::CUBIC));
    generateRandPoints(m_plotType);
}

Point& SetOfPoints::operator[](int index)
{
    try
    {
        return pointList.at(index);
    }
    catch(std::exception& exception)
    {
        std::cout<<"Error in SetOfPoints: index out of bounds. Will exit the program."<<std::endl;
        exit(0);
    }
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
                    generateOnQuadrant(firstQuad + 3);

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
    enum PointType
    {
        EVERYWHERE,
        Y_AXIS,
        X_AXIS,
        RIGHT_HALF,
        LEFT_HALF,
        TOP_HALF,
        BOTTOM_HALF,
        QUADRANT_0,
        QUADRANT_1,
        QUADRANT_2,
        QUADRANT_3
    };

    PointType pointType = EVERYWHERE;
    for(int i = 0; i < m_numPoints; ++i)
    {
        pointType = static_cast<PointType>(Controller::getRandNumInRange(EVERYWHERE, QUADRANT_3));
        switch(pointType)
        {
        case X_AXIS:
            //A point on the y-axis.
            pointList.push_back(Point(0.0f, Controller::getRandFloatInRange(-BOUND, BOUND)));
            break;

        case Y_AXIS:
            //A point on the x-axis.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), 0.0f));
            break;

        case RIGHT_HALF:
            //A point on the right half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(0.0f, BOUND), Controller::getRandFloatInRange(-BOUND, BOUND)));
            break;

        case LEFT_HALF:
            // A point on the left half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, 0.0f), Controller::getRandFloatInRange(-BOUND, BOUND)));
            break;

        case TOP_HALF:
            // A point on the top half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), Controller::getRandFloatInRange(0.0f, BOUND)));
            break;

        case BOTTOM_HALF:
            // A point on the bottom half of the plane.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), Controller::getRandFloatInRange(-BOUND, 0.0f)));
            break;

        case QUADRANT_0:
            // A point on the top-right quadrant.
            pointList.push_back(Point(Controller::getRandFloatInRange(0.0f, BOUND), Controller::getRandFloatInRange(0.0f, BOUND)));
            break;

        case QUADRANT_1:
            // A point on the top-left quadrant.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, 0.0f), Controller::getRandFloatInRange(0.0f, BOUND)));
            break;

        case QUADRANT_2:
            // A point on the bottom-left quadrant.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, 0.0f), Controller::getRandFloatInRange(-BOUND, 0.0f)));
            break;

        case QUADRANT_3:
            // A point on the bottom-right quadrant.
            pointList.push_back(Point(Controller::getRandFloatInRange(0.0f, BOUND), Controller::getRandFloatInRange(-BOUND, 0.0f)));
            break;

        default:
            // A point anywhere.
            pointList.push_back(Point(Controller::getRandFloatInRange(-BOUND, BOUND), Controller::getRandFloatInRange(-BOUND, BOUND)));
            break;
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
    float slope = Controller::getRandFloatInRange(-FUNC_PLOT_PARAM_BOUND , FUNC_PLOT_PARAM_BOUND );
    for(int i = 0; i < m_numPoints/2; ++i)
    {
        float x = static_cast<float>(FUNC_PLOT_X_INTERVAL * i);
        float y = slope * x;
        pointList.push_back(Point(x, y + Controller::getRandFloatInRange(-NOISE_BOUND, NOISE_BOUND)));

        if(i != 0)
        {
            pointList.push_back(Point(-x, -y + Controller::getRandFloatInRange(-NOISE_BOUND, NOISE_BOUND)));
        }
    }
}

void SetOfPoints::generateParabola()
{
    float a = Controller::getRandFloatInRange(-FUNC_PLOT_PARAM_BOUND , FUNC_PLOT_PARAM_BOUND );
    float h = Controller::getRandFloatInRange(-FUNC_PLOT_PARAM_BOUND , FUNC_PLOT_PARAM_BOUND );
    float k = Controller::getRandFloatInRange(-FUNC_PLOT_PARAM_BOUND , FUNC_PLOT_PARAM_BOUND );

    for(int i = 0; i < m_numPoints/2; ++i)
    {
        float x = static_cast<float>(FUNC_PLOT_X_INTERVAL * i);
        float y = a * (x - h)*(x - h) + k;
        pointList.push_back(Point(x, y + Controller::getRandFloatInRange(-NOISE_BOUND, NOISE_BOUND)));

        if(i != 0)
        {
            pointList.push_back(Point(-x, y + Controller::getRandFloatInRange(-NOISE_BOUND, NOISE_BOUND)));
        }
    }
}

void SetOfPoints::generateCubic()
{
    float a = Controller::getRandFloatInRange(-FUNC_PLOT_PARAM_BOUND , FUNC_PLOT_PARAM_BOUND );
    float b = Controller::getRandFloatInRange(-FUNC_PLOT_PARAM_BOUND , FUNC_PLOT_PARAM_BOUND );
    float c = Controller::getRandFloatInRange(-FUNC_PLOT_PARAM_BOUND , FUNC_PLOT_PARAM_BOUND );
    float d = Controller::getRandFloatInRange(-FUNC_PLOT_PARAM_BOUND , FUNC_PLOT_PARAM_BOUND );

    for(int i = 0; i < m_numPoints/2; ++i)
    {
        float x = static_cast<float>(FUNC_PLOT_X_INTERVAL * i);
        float y1 = a*x*x*x + b*x*x + c*x + d;
        float y2 = -a*x*x*x + b*x*x - c*x + d;
        pointList.push_back(Point(x, y1 + Controller::getRandFloatInRange(-NOISE_BOUND, NOISE_BOUND)));

        if(i != 0)
        {
            pointList.push_back(Point(-x, y2 + Controller::getRandFloatInRange(-NOISE_BOUND, NOISE_BOUND)));
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
        out<<"The plot type is: ";
        switch(set.m_plotType)
        {
        case SetOfPoints::TOP_HALF:
            out<<"on the top half of the cartesian plane."<<std::endl;
            break;

        case SetOfPoints::BOTTOM_HALF:
            out<<"on the bottom half of the cartesian plane."<<std::endl;
            break;

        case SetOfPoints::RIGHT_HALF:
            out<<"on the right side of the cartesian plane."<<std::endl;
            break;

        case SetOfPoints::LEFT_HALF:
            out<<"on the left side of the cartesian plane."<<std::endl;
            break;

        case SetOfPoints::SINGLE_QUADRANT:
            out<<"on one quadrant."<<std::endl;
            break;

        case SetOfPoints::DIAGONAL_QUADRANTS:
            out<<"on two diagonally-opposite quadrants."<<std::endl;
            break;

        case SetOfPoints::THREE_QUADRANTS:
            out<<"on three out of four quadrants."<<std::endl;
            break;

        case SetOfPoints::LINEAR:
            out<<"a linear function plot with noise."<<std::endl;
            break;

        case SetOfPoints::PARABOLA:
            out<<"a parabola plot with noise."<<std::endl;
            break;

        case SetOfPoints::CUBIC:
            out<<"a cubic function plot with noise."<<std::endl;
            break;

        default:
            out<<"everywhere on the cartesian plane."<<std::endl;
            break;
        }
        for(int i = 0; i < set.size(); ++i)
        {
            out<<set[i]<<std::endl;
        }
    }

    out<<std::endl;
    return out;
}
