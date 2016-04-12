#include "LangermannPoint.h"
#include "Controller.h"

const int LangermannPoint::M = 5;
const int LangermannPoint::A[5] = {3, 5, 2, 1, 7};
const int LangermannPoint::B[5] = {5, 2, 1, 4, 9};
const int LangermannPoint::C[5] = {1, 2, 5, 2, 3};
const float LangermannPoint::PI = 3.14159265358979323846f;

const float LangermannPoint::POINT_BOUND = 30.0f;/* Refers to page 17 of functions pdf, the limits of y and x on the graph.
                                                     Used to initialize Langermann points in the algorithm.*/

LangermannPoint::LangermannPoint(LangermannPoint& other) : Individual(other), m_x(other.getX()), m_y(other.getY())
{
    // Copy constructor.
}

LangermannPoint::LangermannPoint(float x, float y) : m_x(x), m_y(y)
{
    setFitness(m_x, m_y);
}

// The fitness function is the Langermann function
void LangermannPoint::setFitness(float x, float y)
{
    // Just in case it is not equal zero, set the fitness to zero
    fitness = 0.0f;
    for(int i = 0; i < M; ++i)
    {
        fitness += C[i]*exp( (-1)*pow(x - A[i], 2)/PI - pow(y - B[i], 2)/PI )
                       *cos( PI*pow(x - A[i], 2) - PI*pow(y - B[i], 2) );
    }
}

Individual* LangermannPoint::crossOverAndMutate(Individual& other)
{
    Individual * p2Other = &other;

    // Equivalent to instanceOf operator in Java
    if(LangermannPoint * pOther = dynamic_cast<LangermannPoint*>(p2Other))
    {
        // other was safely casted to LangermannPoint
        float x_new = 0.0f;
        float y_new = x_new;

        // Generate 0 < |alpha| < 1;
        float alpha = pow(-1, Controller::getRandNumInRange(0, 1))*Controller::getRandBtwZeroOne();

        //If both parents are the same, mutate the other to have a bit of a difference.
        if(*this == *pOther)
        {
            other.mutate();
        }

        x_new = getX()*alpha + (1 - alpha)*(pOther->getX());
        y_new = getY()*alpha + (1 - alpha)*(pOther->getY());

        Individual* child = new LangermannPoint(x_new, y_new); // The child
        child->mutate();

        return child;
    }
    else
    {
        return nullptr;
    }
}

void LangermannPoint::mutate()
{
    m_x += Controller::getRandFloatInRange(-MUTATION_VAL, MUTATION_VAL);
    m_y += Controller::getRandFloatInRange(-MUTATION_VAL, MUTATION_VAL);
    setFitness(m_x, m_y);
}

float LangermannPoint::getX() const
{
    return m_x;
}

float LangermannPoint::getY() const
{
    return m_y;
}

void LangermannPoint::operator=(LangermannPoint& other)
{
    Individual::operator=(other);
    m_x = other.getX();
    m_y = other.getY();
}
