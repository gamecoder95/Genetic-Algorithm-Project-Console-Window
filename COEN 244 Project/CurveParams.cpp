#include "CurveParams.h"
#include "Controller.h"

const float CurveParams::PARAM_BOUND = 30.0f; //Arbitrary parameter value for initializing curves

CurveParams::CurveParams(CurveParams& other) : Individual(other), m_a(other.getA()), m_b(other.getB()),
                                                                  m_c(other.getC()), m_d(other.getD())
{
    // Copy constructor.
}

CurveParams::CurveParams(float a, float b, float c, float d) : m_a(a), m_b(b), m_c(c), m_d(d)
{
    setFitness(m_a, m_b, m_c, m_d, Controller::getSetOfPoints());
}

// The fitness function is the Mean Square error function
void CurveParams::setFitness(float a, float b, float c, float d, SetOfPoints& setOfPoints)
{
    float meanSquareError = 0.0f;
    float vertDist = 0.0f;

    for(int i = 0; i < setOfPoints.size(); ++i)
    {
        vertDist = setOfPoints[i].getY() - getYAtXOf(setOfPoints[i]);

        meanSquareError += vertDist * vertDist;
    }

    meanSquareError /= setOfPoints.size();

    fitness = sqrt(meanSquareError);
}

float CurveParams::getYAtXOf(Point& point) const
{
    return    m_a*powf(point.getX(), 3)
            + m_b*powf(point.getX(), 2)
            + m_c*point.getX()
            + m_d;
}

Individual* CurveParams::crossOverAndMutate(Individual& other)
{
    Individual * p2Other = &other;

    // Equivalent to instanceOf operator in Java
    if(CurveParams * pOther = dynamic_cast<CurveParams*>(p2Other))
    {
        float a_new = 0.0f;
        float b_new = a_new;
        float c_new = a_new;
        float d_new = a_new;

        // Generate 0 < |alpha| < 1;
        float alpha = pow(-1, Controller::getRandNumInRange(0, 1))*Controller::getRandBtwZeroOne();

        //If both parents are the same, mutate the other to have a bit of a difference.
        if(*this == *pOther)
        {
            other.mutate();
        }

        /*
        a_new = getA()*alpha + (1 - alpha)*(pOther->getA());
        b_new = getB()*alpha + (1 - alpha)*(pOther->getB());
        c_new = getC()*alpha + (1 - alpha)*(pOther->getC());
        d_new = getD()*alpha + (1 - alpha)*(pOther->getD());
        */

        if( (getA() == 0.0f && *this > *pOther) || (pOther->getA() == 0.0f && *pOther > *this))
        {
            a_new = 0;
        }
        else
        {
            a_new = getA()*alpha + (1 - alpha)*(pOther->getA());
        }

        if((getB() == 0.0f && *this > *pOther) || (pOther->getB() == 0.0f && *pOther > *this))
        {
            b_new = 0;
        }
        else
        {
            b_new = getB()*alpha + (1 - alpha)*(pOther->getB());
        }

        if((getC() == 0.0f && *this > *pOther) || (pOther->getC() == 0.0f && *pOther > *this))
        {
            c_new = 0.0f;
        }
        else
        {
            c_new = getC()*alpha + (1 - alpha)*(pOther->getC());
        }

        if((getD() == 0.0f && *this > *pOther) || (pOther->getD() == 0.0f && *pOther > *this))
        {
            d_new = 0.0f;
        }
        else
        {
            d_new = getD()*alpha + (1 - alpha)*(pOther->getD());
        }

        Individual* child = new CurveParams(a_new, b_new, c_new, d_new); //child
        child->mutate();

        return child;
    }
    else
    {
        return nullptr;
    }
}

void CurveParams::mutate()
{
    m_a += (m_a != 0.0f)? Controller::getRandFloatInRange(-Individual::getMutationVal(), Individual::getMutationVal()) : 0.0f;
    m_b += (m_b != 0.0f)? Controller::getRandFloatInRange(-Individual::getMutationVal(), Individual::getMutationVal()) : 0.0f;
    m_c += (m_c != 0.0f)? Controller::getRandFloatInRange(-Individual::getMutationVal(), Individual::getMutationVal()) : 0.0f;
    m_d += (m_d != 0.0f)? Controller::getRandFloatInRange(-Individual::getMutationVal(), Individual::getMutationVal()) : 0.0f;
    setFitness(m_a, m_b, m_c, m_d, Controller::getSetOfPoints());
}

float CurveParams::getA() const
{
    return m_a;
}

float CurveParams::getB() const
{
    return m_b;
}

float CurveParams::getC() const
{
    return m_c;
}

float CurveParams::getD() const
{
    return m_d;
}

void CurveParams::operator=(CurveParams& other)
{
    Individual::operator=(other);
    m_a = other.getA();
    m_b = other.getB();
    m_c = other.getC();
    m_d = other.getD();
}
