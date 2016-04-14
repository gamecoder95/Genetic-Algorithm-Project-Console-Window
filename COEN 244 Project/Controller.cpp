#include "Controller.h"
#include "LangermannPoint.h"
#include "CurveParams.h"

std::mt19937 Controller::mersenne(static_cast<unsigned int>(time(0)));

// Constant random fraction used for calculating numbers within range
const double Controller::FRACTION = 1.0 / (static_cast<double>(mersenne.max()) + 1.0);
const int Controller::DEFAULT_POP_SIZE = 100;
const int Controller::MAX_GEN_COUNT = 500;
const int Controller::MAX_RIVAL_GEN_COUNT_L = 25;
const int Controller::MAX_RIVAL_GEN_COUNT_C = 5;

Controller::ProblemType Controller::problemType = Controller::OPTIMIZATION;

int Controller::initPopSize = Controller::DEFAULT_POP_SIZE;
int Controller::generationCount = 1;
int Controller::rivalGenCount = 0;

GenePool Controller::population;
GenePool Controller::parentPool;
GenePool Controller::offspringPool;
GenePool Controller::jointPool;

SetOfPoints Controller::setOfPoints;

IndivPtr Controller::solution;//Null
IndivPtr Controller::fittestRivalOfSol;//Null


void Controller::run()
{
    initializePop();

    // If the application is the curve fitting one, display the set of points that the curve will try to fit.
    if(getProblemType() == Controller::CURVE_FITTING)
    {
        std::cout<<setOfPoints;
    }
    std::cout<<std::endl<<"The initial population is:"<<std::endl<<population;
    char beginChar;

    // This allows the user to observe the initial population and/or set of points before beginning at his/her will.
    std::cout<<std::endl<<"Enter a character to begin the genetic algorithm: ";
    std::cin>>beginChar;
    std::cout<<std::endl;

    while(!terminationCondition())
    {
        performParentSelection();
        while(offspringPool.size() < 10 * getInitPopSize())
        {
            generateAndAddTwoOffspring();
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
        std::cout<<"The fittest individual of all time is: "<<getSolution();

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
    // After sorting, the first individual is the fittest.

    // Initialize the solution to the fittest member of the population.
    if(&getSolution() == nullptr)
    {
        setSolution(population[0]);
        population.scramblePool();
        return false;
    }
    else if(generationCount < MAX_GEN_COUNT)
    {
        bool needAdjustments = false;

        if(getProblemType() == Controller::OPTIMIZATION)
        {
            /*
                The goal here is to get the point that produces the lowest value for the function.
                If the fittest member of the new population is fitter than the solution,
                set the solution to that member.
                Since there is no longer a rival to the solution, reset the rival generation count.
            */
            if(getSolution() < population[0])
            {
                setSolution(population[0]);
                if(rivalGenCount > 0)
                {
                    rivalGenCount = 0;
                }
                return false;
            }
            else if(getSolution() == population[0])
            {
                /*
                    If the solution saved from last generation is the same,
                    adjust the population, and boost the mutation value for more variety.
                */
                needAdjustments = true;
                Individual::boostMutationVal();

                // Reset the rival count if the fittest member is no longer equal to the rival but to the solution.
                if(rivalGenCount > 0)
                {
                    rivalGenCount = 0;
                }
            }
            else if(getSolution() > population[0])
            {
                /*
                    If the solution is fitter than the fittest member of the present generation,
                    we want to see if we can, after a few generations, produce a fitter solution.

                    If there is no rival currently set the rival to the fittest member of the present generation.

                    Otherwise, check to see if the rival is equal to the fittest member of the present generation.
                    If it is for a few generations, adjust the population. If after double that number, the solution
                    saved is still the fittest, terminate.

                    If the rival is not equal to the fittest member of the present generation, then terminate then,
                    as we are getting not going anywhere if that is the case.
                */
                if(&getRival() == nullptr)
                {
                    setRival(population[0]);
                }
                else if(getRival() == population[0])
                {
                    if(rivalGenCount < MAX_RIVAL_GEN_COUNT_L)
                    {
                        if(++rivalGenCount > MAX_RIVAL_GEN_COUNT_L/2)
                        {
                            needAdjustments = true;
                        }
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    return true;
                }
            }
        }
        else
        {
            /*
                The goal here is to get the curve with a fitness as close as possible to zero.
                If the fittest member of the new population is fitter than the solution,
                set the solution to that member.
                Since there is no longer a rival to the solution, reset the rival generation count.
            */
            if(getSolution() < population[0])
            {
                setSolution(population[0]);
                rivalGenCount = 0;
            }
            else
            {
                /*
                    Otherwise, adjust the population.
                    If the solution is fitter than the fittest of the new population, keep incrementing the rival generation
                    count. This is so that a chance to the later generations to produce a more worthy individual is given.
                    If the solution is equal to the fittest, or if after a few generations, the solution is still the fittest,
                    then boost the mutation constant. If after a lot of generations, no fitter solution is produced, then terminate.
                */
                needAdjustments = true;
                if(getSolution() > population[0])
                {
                    ++rivalGenCount;
                }
                if(getSolution() == population[0]
                   || (getSolution() > population[0] && rivalGenCount >= MAX_RIVAL_GEN_COUNT_C/5))
                {
                    Individual::boostMutationVal();
                    if(rivalGenCount >= MAX_RIVAL_GEN_COUNT_C)
                    {
                        return true;
                    }
                }
            }
        }

        // If needed, adjust the population by replacing the bottom half of the population with fresh, new individuals.
        if(needAdjustments)
        {
            for(int i = population.size()/2; i < population.size(); ++i)
            {
                population[i] = *generateIndiv();
            }
        }

        // Scramble the pool to reduce selection pressure.
        population.scramblePool();
        return false;
    }
    else
    {
        /*
            If the number of generations passed is greater or equal to the limit, terminate.
            Too many generations passing with barely any change won't produce any more significant results.
        */
        return true;
    }
}

void Controller::performParentSelection()
{
     parentPool.clear();

     // Scramble the pool to further randomize selection.
     population.scramblePool();

     /*
        Until the parent pool is half the size of the population,
        randomly choose two individuals, and add the fitter out of the
        two if they aren't equal to each other.
     */
     while(parentPool.size() < population.size()/2)
     {
         Individual& candidateA = population.getRandIndiv();
         Individual& candidateB = population.getRandIndiv();

         if(candidateA != candidateB)
         {
             parentPool.pushBackIndiv((candidateA > candidateB)? candidateA : candidateB);
         }
     }

}

void Controller::generateAndAddTwoOffspring()
{
    if(parentPool.size() > 0)
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
}

void Controller::setProblemType(Controller::ProblemType type)
{
    problemType = type;
}

Controller::ProblemType Controller::getProblemType()
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

void Controller::setRival(Individual& rival)
{
    if(problemType == Controller::OPTIMIZATION)
    {
        fittestRivalOfSol = IndivPtr(new LangermannPoint(dynamic_cast<LangermannPoint&>(rival)));
    }
    else
    {
        fittestRivalOfSol = IndivPtr(new CurveParams(dynamic_cast<CurveParams&>(rival)));
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
    return low + static_cast<float>(mersenne()) / (static_cast<float>(mersenne.max()/(high - low)));
}

float Controller::getRandBtwZeroOne()
{
    return static_cast<float>(mersenne()) / static_cast<float>(mersenne.max());
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
        // Generate a point between the LangermannPoint bounds.
        return IndivPtr(new LangermannPoint(getRandFloatInRange(-LangermannPoint::POINT_BOUND, LangermannPoint::POINT_BOUND),
                                            getRandFloatInRange(-LangermannPoint::POINT_BOUND, LangermannPoint::POINT_BOUND)));
    }
    else
    {
        // Generate a random number between 0 and ten to increase variety in the curves generated.
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

