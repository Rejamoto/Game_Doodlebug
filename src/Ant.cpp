#include "../include/Ant.h"
#include "../include/MoveRandom.h"

Ant::Ant() {}
Ant::Ant(Point* point) : Organism(ANT_TYPE, ANT_BREED_TIME, point) {}
Ant::Ant(Point* point, MoveBehavior* mb) : Organism(ANT_TYPE, ANT_BREED_TIME, point, mb) {  }

int Ant::getAntType() const { return getType(); }

Point Ant::getAntLocation() const { return getPoint(); }
Point* Ant::getLocation() const { return getPointer(); }

bool Ant::isBreeding()
{
    if(breeding()){
        resetBreed();
        return true;
    }
    return false;
}

void Ant::movingAnt(TheGrid::GameBoard& gb, Point* p) {
    breedTurn();
    movingOrganism(gb, p);
}
void Ant::moving(const Point& newLocation) { setPoint(newLocation); }

void Ant::setAntMovement(MoveRandom* mb) { setMovement(mb); }
