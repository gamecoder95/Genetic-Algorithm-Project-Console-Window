#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <ctime> // For time()
#include <random> //For mt19937 (mersenne rng)
#include <iostream>
#include "GenePool.h"
#include "SetOfPoints.h"
#include "LangermannPoint.h"
#include "CurveParams.h"

class Controller
{
public:
    static const double FRACTION;
    static const int DEFAULT_POP_SIZE;
    static const int MAX_GEN_COUNT;
    static const int MAX_RIVAL_GEN_COUNT_L;
    static const int MAX_RIVAL_GEN_COUNT_C;
    static int getRandNumInRange(int low, int high);
    static float getRandFloatInRange(float low, float high);
    static float getRandBtwZeroOne();

    enum ProblemType
    {
        OPTIMIZATION,
        CURVE_FITTING
    };

    static void run();
    static void setProblemType(ProblemType type);
    static ProblemType getProblemType();
    static void setInitPopSize(int initSize);
    static int getInitPopSize();
    static SetOfPoints& getSetOfPoints();
    static void setSolution(Individual& sol);
    static Individual& getSolution();
    static void setRival(Individual& rival);
    static Individual& getRival();

private:
    static ProblemType problemType;

    static int initPopSize;
    static int generationCount;
    static int rivalGenCount;

    static GenePool population;
    static GenePool parentPool;
    static GenePool offspringPool;
    static GenePool jointPool;

    static SetOfPoints setOfPoints;
    static IndivPtr solution;
    static IndivPtr fittestRivalOfSol;

    static std::mt19937 mersenne;

    static void initializePop();
    static IndivPtr generateIndiv();
    static bool terminationCondition();
    static void performParentSelection();
    static void generateAndAddTwoOffspring();
};

#endif // CONTROLLER_H_INCLUDED
