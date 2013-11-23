#ifndef DOODLEBUG_H
#define DOODLEBUG_H

#include "Organism.h"
#include "OrganismInfo.h"
#include "MoveRandom.h"

class Doodlebug : private Organism
{
    public:
        Doodlebug();
        Doodlebug(Point* point);
        Doodlebug(Point* point, MoveBehavior* mb);

        int getDoodleType() const;

        Point getDoodleLocation() const;
        Point* getLocation() const;

        bool isBreeding();
        bool isAlive() const;

        void movingDoodlebug(TheGrid::GameBoard& gb, Point* p);
        void moving(const Point& newLocation);

        void noEat();
        void eatAnt();

        void setDoodleMovement(MoveRandom* mb);
    private:
        int hunger;
};

#endif // DOODLEBUG_H
