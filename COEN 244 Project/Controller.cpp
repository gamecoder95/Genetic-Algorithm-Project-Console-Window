#include "Controller.h"
#include "LangermannPoint.h"
#include "CurveParams.h"

std::mt19937 Controller::mersenne(static_cast<unsigned int>(time(0)));
// Constant random fraction used for calculating numbers within range
const double Controller::FRACTION = 1.0 / (static_cast<double>(mersenne.max()/*RAND_MAX*/) + 1.0);
const int Controller::DEFAULT_POP_SIZE = 100;
const int Controller::MAX_GEN_COUNT = 200;

Controller::ProblemType Controller::problemType = Controller::OPTIMIZATION;

int Controller::initPopSize = Controller::DEFAULT_POP_SIZE;
int Controller::generationCount = 1;
int Controller::rivalGenCount = 0;

GenePool Controller::population;
GenePool Controller::parentPool;
GenePool Controller::offspringPool;
GenePool Controller::jointPool;

SetOfPoints Controller::setOfPoints(SetOfPoints::EVERYWHERE);

IndivPtr Controller::solution;//Null
IndivPtr Controller::fittestRivalOfSol;//Null


void Controller::run()
{
    initializePop();
    //Display the initial population.
    std::cout<<std::endl<<"The initial population is:"<<population;
    char beginChar;

    if(getProblemType() == Controller::CURVE_FITTING)
    {
        std::cout<<setOfPoints;
    }

    std::cout<<std::endl<<"Enter a character to begin the genetic algorithm: ";
    std::cin>>beginChar;
    std::cout<<std::endl;

    while(!terminationCondition())
    {
        performParentSelection();
        while(offspringPool.size() < 10 * getInitPopSize())
        {
            parentPool.scramblePool();
            // Choose two random parents.
            Individual* parent1 = &parentPool.getRandIndiv();
            Individual* parent2 = &parentPool.getRandIndiv();

            // Create two children from those parents.
            Individual* child1 = parent1->crossOverAndMutate(*parent2);
            Individual* child2 = parent1->crossOverAndMutate(*parent2);

            // Add those children to the offspring pool.
            offspringPool.pushBackIndiv(*child1);
            offspringPool.pushBackIndiv(*child2);

            delete child1;
            delete child2;
        }

        // Form the joint pool from the parent and offspring pools
        jointPool.clear();
        jointPool.addContentsOf(parentPool);
        jointPool.addContentsOf(offspringPool);
        jointPool.sortPool();

        population.clear();
        std::cout<<"Generation number "<<generationCount<<":"<<std::endl;
        for(int i = 0; i < getInitPopSize(); ++i)
        {
            population.pushBackIndiv(jointPool[i]);
        }
        std::cout<<population;

        //For formatting purposes.
        std::cout<<std::endl<<std::endl;

        ++generationCount;// Increment the number of generations.

    }

    // Display either the most fit individual or the solution to the problem.
    if(generationCount < MAX_GEN_COUNT)
    {
        std::cout<<std::endl<<"The solution is: "<<getSolution()<<std::endl;
    }
    else
    {
        std::cout<<std::endl<<"The most fit individual is: "<<getSolution()<<std::endl;
    }

    std::cout<<std::endl<<"Number of generations passed: "<<generationCount<<std::endl;
}

bool Controller::terminationCondition()
{
    population.sortPool();
    Individual::resetMutationVal();

    // After sorting, the first is the fittest
    if(generationCount <= 1)
    {
        setSolution(population[0]);
        population.scramblePool();
        return false;
    }
    else if(generationCount < MAX_GEN_COUNT)
    {
        bool needMoreGuys = false;
        if(getSolution() < population[0])
        {
            setSolution(population[0]);
            return false;
        }
        else if(getSolution() == population[0])
        {
            // If the solution saved from last generation is the same,
            // add half the population size of new individuals.
            needMoreGuys = true;
            Individual::boostMutationVal();

        }
        else if(getSolution() > population[0])
        {
            if(&getRival() == nullptr || rivalGenCount == 0)
            {
                std::cout<<"HERE 0!"<<std::endl;
                setRival(population[0]);;
                rivalGenCount = 1;
            }
            else if(getRival() == population[0])
            {
                std::cout<<"HERE 1"<<std::endl;

                if(rivalGenCount <= 3)
                {
                    std::cout<<"HERE 2"<<std::endl;
                    ++rivalGenCount;
                }
                else
                {
                    std::cout<<"HERE 3"<<std::endl;
                    rivalGenCount = 0;
                    needMoreGuys = true;
                    Individual::boostMutationVal();
                }
            }
            else
            {
                return true;
            }
        }

        if(needMoreGuys)
        {
            for(int i = population.size()/2; i < population.size(); ++i)
            {
                population[i] = *generateIndiv();
            }
        }
        population.scramblePool();
        return false;
    }
    else
    {
        return true;
    }
}

void Controller::performParentSelection()
{
     parentPool.clear();
     population.scramblePool();
     while(parentPool.size() < population.size()/2)
     {
         Individual * candidateA = &population.getRandIndiv();
         Individual * candidateB = &population.getRandIndiv();

         if(*candidateA != *candidateB)
         {
             parentPool.pushBackIndiv((*candidateA > *candidateB)? *candidateA : *candidateB);
         }
     }

}

void Controller::setProblemType(Controller::ProblemType type)
{
    problemType = type;
}

inline Controller::ProblemType Controller::getProblemType()
{
    return problemType;
}

void Controller::setInitPopSize(int initSize)
{
    if(initSize < DEFAULT_POP_SIZE)
    {
        initSize = DEFAULT_POP_SIZE;
    }
    initPopSize = initSize;
}

int Controller::getInitPopSize()
{
    return initPopSize;
}

SetOfPoints& Controller::getSetOfPoints()
{
    return setOfPoints;
}

void Controller::setSolution(Individual& sol)
{
    if(problemType == Controller::OPTIMIZATION)
    {
        solution = IndivPtr(new LangermannPoint(dynamic_cast<LangermannPoint&>(sol)));
    }
    else
    {
        solution = IndivPtr(new CurveParams(dynamic_cast<CurveParams&>(sol)));
    }
}

Individual& Controller::getSolution()
{
    return *solution;
}

void Controller::setRival(Individual& sol)
{
    if(problemType == Controller::OPTIMIZATION)
    {
        fittestRivalOfSol = IndivPtr(new LangermannPoint(dynamic_cast<LangermannPoint&>(sol)));
    }
    else
    {
        fittestRivalOfSol = IndivPtr(new CurveParams(dynamic_cast<CurveParams&>(sol)));
    }
}

Individual& Controller::getRival()
{
    return *fittestRivalOfSol;
}

int Controller::getRandNumInRange(int low, int high)
{
    return static_cast<int>(mersenne() * FRACTION * (high - low + 1) + low);
}

float Controller::getRandFloatInRange(float low, float high)
{
    return low + static_cast<float>(mersenne()) / (static_cast<float>(/*RAND_MAX*/ mersenne.max()/(high - low)));
}

float Controller::getRandBtwZeroOne()
{
    return static_cast<float>(mersenne()) / static_cast<float>(/*RAND_MAX*/ mersenne.max());
}

void Controller::initializePop()
{
    for(int i = 0; i < getInitPopSize(); ++i)
    {
        population.pushBackIndiv(generateIndiv());
    }
}

IndivPtr Controller::generateIndiv()
{
    if(getProblemType() == Controller::OPTIMIZATION)
    {
        return IndivPtr(new LangermannPoint(getRandFloatInRange(0.0f, LangermannPoint::POINT_BOUND),
                                            getRandFloatInRange(0.0f, LangermannPoint::POINT_BOUND)));
    }
    else
    {
        int paramToTakeOut = getRandNumInRange(0, 10);

        //Add a curve with parameters missing depending on paramToTakeOut.
        switch(paramToTakeOut)
        {
        case 1:
            return IndivPtr(new CurveParams(0,
                                          getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                          getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                          getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND)));
            break;

        case 2:
            return IndivPtr(new CurveParams(getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              0,
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND)));
            break;

        case 3:
            return IndivPtr(new CurveParams(getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                          getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                          0,
                                          getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND)));
            break;

        case 4:
            return IndivPtr(new CurveParams(getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              0));
            break;

        case 5:
            return IndivPtr(new CurveParams(0,
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              0));
            break;

        case 6:
            return IndivPtr(new CurveParams(getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              0,
                                              0,
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND)));
            break;

        case 7:
            return IndivPtr(new CurveParams(0,
                                              0,
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND)));
            break;

        case 8:
            return IndivPtr(new CurveParams(getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              0,
                                              0));
            break;

        case 9:
            return IndivPtr(new CurveParams(0,
                                              0,
                                              0,
                                              getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND)));
            break;

        case 10:
            return IndivPtr(new CurveParams(getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                              0,
                                              0,
                                              0));
            break;

        default:
            return IndivPtr(new CurveParams(getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                          getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                          getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND),
                                          getRandFloatInRange(-CurveParams::PARAM_BOUND, CurveParams::PARAM_BOUND)));
            break;
        }

    }
}

