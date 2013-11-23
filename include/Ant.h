#ifndef ANT_H
#define ANT_H

#include "Organism.h"
#include "OrganismInfo.h"
#include "MoveRandom.h"

class Ant : private Organism
{
    public:
        Ant();
        Ant(Point* point);
        Ant(Point* point, MoveBehavior* mb);

        int getAntType() const;

        Point getAntLocation() const;
        Point* getLocation() const;

        bool isBreeding();

        void movingAnt(TheGrid::GameBoard& gb, Point* p);
        void moving(const Point& newLocation);

        void setAntMovement(MoveRandom* mb);
};

#endif // ANT_H
