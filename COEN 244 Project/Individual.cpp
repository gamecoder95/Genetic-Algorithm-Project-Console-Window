#include "Individual.h"
#include "Controller.h"
#include "LangermannPoint.h"
#include "CurveParams.h"

Individual::Individual() : fitness(0.0f), fitnessPercent(0.0f)
{
    // Default constructor, initialize fields to zero.
}

Individual::Individual(Individual& other) : fitness(other.getFitness()), fitnessPercent(other.getFitnessPercent())
{
    // Copy constructor, initialize fields to the other's values.
}

Individual::~Individual()
{

}

float Individual::getFitness() const
{
    return fitness;
}

void Individual::setFitnessPercent(float sumAllFitness)
{
    // Since the fitter individual has a smaller fitness value,
    // the fitness percent will be 100 - (fitness/sumAllFitness)*100%
    fitnessPercent = 100 - (fitness/sumAllFitness)*100;
}

float Individual::getFitnessPercent() const
{
    return fitnessPercent;
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

// In both applications of the problem, the smaller fitness value is better
bool Individual::operator>=(Individual& other)
{
    return getFitness() <= other.getFitness();
}

// In both applications of the problem, the smaller fitness value is better
bool Individual::operator<=(Individual& other)
{
    return getFitness() >= other.getFitness();
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
    fitnessPercent = other.getFitnessPercent();
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
