#ifndef CURVEPARAMS_H_INCLUDED
#define CURVEPARAMS_H_INCLUDED

#include "Individual.h"
#include "SetOfPoints.h"

class CurveParams : public Individual
{
private:
    float m_a;
    float m_b;
    float m_c;
    float m_d;

    void setFitness(float a, float b, float c, float d, SetOfPoints& setOfPoints);
    float getYAtXOf(Point& point) const;

protected:
    void mutate();

public:
    static const float PARAM_BOUND;

    CurveParams(float a, float b, float c, float d);
    CurveParams(CurveParams& other);
    Individual* crossOverAndMutate(Individual& other);


    float getA() const;
    float getB() const;
    float getC() const;
    float getD() const;

    void operator=(CurveParams& other);
};

#endif // CURVEPARAMS_H_INCLUDED
