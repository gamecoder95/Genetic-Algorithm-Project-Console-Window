#include "Individual.h"
#include "Controller.h"
#include "LangermannPoint.h"
#include "CurveParams.h"

const float Individual::MUTATION_VAL_L = 0.25f;
const float Individual::MUTATION_VAL_C = 0.25*CurveParams::PARAM_BOUND;
const float Individual::BOOSTED_MUTATION_VAL_L = 1.50f;
const float Individual::BOOSTED_MUTATION_VAL_C = 0.5*CurveParams::PARAM_BOUND;

float Individual::mutationVal = Individual::MUTATION_VAL_L;

void Individual::resetMutationVal()
{
    mutationVal = (Controller::getProblemType() == Controller::OPTIMIZATION)? MUTATION_VAL_L : MUTATION_VAL_C;
}

void Individual::boostMutationVal()
{
    mutationVal = (Controller::getProblemType() == Controller::OPTIMIZATION)? BOOSTED_MUTATION_VAL_L
                                                                            : BOOSTED_MUTATION_VAL_C;
}

float Individual::getMutationVal()
{
    return mutationVal;
}

Individual::Individual() : fitness(0.0f)
{
    // Default constructor, initialize fields to zero.
}

Individual::Individual(Individual& other) : fitness(other.getFitness())
{
    // Copy constructor, initialize fields to the other's values.
}

float Individual::getFitness() const
{
    return fitness;
}

// In both applications of the problem, the smaller fitness value is better
bool Individual::operator>(Individual& other)
{
    return getFitness() < other.getFitness();
}

// In both applications of the problem, the smaller fitness value is better
bool Individual::operator<(Individual& other)
{
    return getFitness() > other.getFitness();
}

bool Individual::operator==(Individual& other)
{
    return getFitness() == other.getFitness();
}

bool Individual::operator!=(Individual& other)
{
    return !(*this == other);
}

void Individual::operator=(Individual& other)
{
    fitness = other.getFitness();
}

std::ostream& operator<<(std::ostream& out, Individual& indiv)
{
    out<<"Fitness = "<<indiv.fitness;
    Individual * p2Indiv = &indiv;
    if(LangermannPoint* pIndivL = dynamic_cast<LangermannPoint*>(p2Indiv))
    {
        out<<" x = "<<pIndivL->getX()<<" y = "<<pIndivL->getY();
    }
    else if(CurveParams* pIndivC = dynamic_cast<CurveParams*>(p2Indiv))
    {
        out<<" f(x) = "<<pIndivC->getA()<<"x^3 + "<<pIndivC->getB()<<"x^2 + "<<pIndivC->getC()<<"x + "<<pIndivC->getD();
    }

    return out;
}
