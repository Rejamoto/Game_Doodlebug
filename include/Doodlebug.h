#ifndef DOODLEBUG_H
#define DOODLEBUG_H

#include "Organism.h"
#include "OrganismInfo.h"
#include "MoveRandom.h"

class Doodlebug : private Organism
{
    public:
        Doodlebug();
        Doodlebug(const Point& point);
        int getDoodleType() const;
        Point getDoodleLocation() const;
        bool isBreeding();
        bool isAlive() const;
        void moving();
        void moving(const Point& newLocation);
        void noEat();
        void eatAnt();

        void setDoodleMovement(MoveRandom* mb);
    private:
        int hunger;
};

#endif // DOODLEBUG_H
