#ifndef GENEPOOL_H_INCLUDED
#define GENEPOOL_H_INCLUDED

#include "Individual.h"
#include <vector>
#include <iostream>


class GenePool
{
private:
    std::vector<IndivPtr> individualList;

    class GreaterThanSort
    {
    public:
        static bool greaterThan(const IndivPtr& elementA, const IndivPtr& elementB)
        {
            return *elementA > *elementB;
        }
    };

public:
    GenePool();
    void sortPool(); //Sorts the pool from greatest to least (in terms of fitness)
    void scramblePool();
    Individual& operator[](int index);
    Individual& getRandIndiv();
    void pushBackIndiv(const IndivPtr& indiv);
    void pushBackIndiv(Individual& indiv);
    int size() const;
    void clear();
    void addContentsOf(GenePool& other);

    friend std::ostream& operator<<(std::ostream& out, GenePool& pool);
};

std::ostream& operator<<(std::ostream& out, GenePool& pool);

#endif // GENEPOOL_H_INCLUDED
