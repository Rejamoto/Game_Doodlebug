#ifndef ANT_H
#define ANT_H

#include "Organism.h"
#include "OrganismInfo.h"
#include "MoveRandom.h"

class Ant : private Organism
{
    public:
        Ant();
        Ant(const Point& point);
        int getAntType() const;
        Point getAntLocation() const;
        bool isBreeding();
        void moving();
        void moving(const Point& newLocation);

        void setAntMovement(MoveRandom* mb);
};

#endif // ANT_H
