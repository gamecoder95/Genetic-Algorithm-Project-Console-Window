#ifndef LANGERMANNPOINT_H_INCLUDED
#define LANGERMANNPOINT_H_INCLUDED

#include "Individual.h"

class LangermannPoint : public Individual
{
private:
    float m_x;
    float m_y;

    // A, B, and C are the Langermann function constants
    // M is the limit of summation for the Langermann function
    static const int M;
    static const int A[5];
    static const int B[5];
    static const int C[5];
    static const float PI;

    void setFitness(float x, float y);

protected:

    void mutate();

public:

    Individual* crossOverAndMutate(Individual& other);
    static const float POINT_BOUND;

    LangermannPoint(float x, float y);
    LangermannPoint(LangermannPoint& other);

    float getX() const;
    float getY() const;

    void operator=(LangermannPoint& other);
};

#endif // LANGERMANNPOINT_H_INCLUDED
