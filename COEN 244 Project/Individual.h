#ifndef INDIVIDUAL_H_INCLUDED
#define INDIVIDUAL_H_INCLUDED

#include <cmath>
#include <iostream>
#include <memory> //For smart pointers

//prototype for the class so that the typedef can stay at the top.
class Individual;

// Typedef for more user-friendly name.
typedef std::shared_ptr<Individual> IndivPtr;

class Individual
{
private:
    static float mutationVal;

protected:
    float fitness;
    float fitnessPercent;

public:

    static const float MUTATION_VAL;// = 0.25
    static const float BOOSTED_MUTATION_VAL;// = 1.5
    static void resetMutationVal();
    static void boostMutationVal();
    static float getMutationVal();

    Individual();
    Individual(Individual& other);
    virtual ~Individual();

    virtual Individual* crossOverAndMutate(Individual& other) = 0;
    virtual void mutate() = 0;

    float getFitness() const;
    void  setFitnessPercent(float sumAllFitness);
    float getFitnessPercent() const;

    //Used to compare individuals based on fitness.
    bool operator>(Individual& other);
    bool operator<(Individual& other);
    bool operator>=(Individual& other);
    bool operator<=(Individual& other);
    bool operator==(Individual& other);
    bool operator!=(Individual& other);
    virtual void operator=(Individual& other);

    friend std::ostream& operator<<(std::ostream& out, Individual& indiv);
};

std::ostream& operator<<(std::ostream& out, Individual& indiv);

#endif // INDIVIDUAL_H_INCLUDED
