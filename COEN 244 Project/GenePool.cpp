#include "GenePool.h"
#include "Controller.h"
#include <algorithm>
#include <exception>

GenePool::GenePool()
{
    //Reserve space for up to 10000 individuals.
    individualList.reserve(10000);
}

void GenePool::sortPool()
{
    // Sort the vector from greatest to least using GreatertThanSort
    // The third parameter is the address of the GreaterThanSort's greater than function for a GreaterThanSort for Individuals
    std::sort(individualList.begin(), individualList.end(), &GreaterThanSort::greaterThan);
}

Individual& GenePool::operator[](int index)
{
    try
    {
        return *individualList.at(index);
    }
    catch(std::exception& exception)
    {
        std::cout<<"Error in GenePool: index out of bounds. Will exit the program."<<std::endl;
        exit(0);
    }
}

// Get an individual from the list between index 0 and index size - 1
Individual& GenePool::getRandIndiv()
{
    return this->operator[](Controller::getRandNumInRange(0, this->size() - 1));
}

void GenePool::pushBackIndiv(const IndivPtr& indiv)
{
    individualList.push_back(indiv);
}

void GenePool::pushBackIndiv(Individual& indiv)
{
    Individual * p2Indiv = &indiv;

    if(LangermannPoint * pIndivL = dynamic_cast<LangermannPoint*>(p2Indiv))
    {
        IndivPtr pL(new LangermannPoint(*pIndivL));
        individualList.push_back(pL);
    }
    else if(CurveParams * pIndivC = dynamic_cast<CurveParams*>(p2Indiv))
    {
        IndivPtr pC(new CurveParams(*pIndivC));
        individualList.push_back(pC);
    }
}

int GenePool::size() const
{
    return individualList.size();
}

void GenePool::clear()
{
    if(!individualList.empty())
    {
        individualList.clear();
    }
}

void GenePool::addContentsOf(GenePool& other)
{
    for(int i = 0; i < other.size(); ++i)
    {
        pushBackIndiv(other[i]);
    }
}

void GenePool::scramblePool()
{
    std::random_shuffle(individualList.begin(), individualList.end());
}

std::ostream& operator<<(std::ostream& out, GenePool& pool)
{
    out<<std::endl;
    if(pool.individualList.empty())
    {
        out<<"This gene pool is empty!"<<std::endl;
    }
    else
    {
        pool.sortPool();
        out<<"This gene pool contains "<<pool.size()<<" individuals: "<<std::endl;
        out<<"The fittest individual is: "<<pool[0]<<std::endl;
        out<<"The least fit individual is: "<<pool[pool.size() - 1]<<std::endl;
        out<<std::endl;

        pool.scramblePool();
    }

    return out;
}

