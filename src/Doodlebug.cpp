#include "../include/Doodlebug.h"
#include "../include/MoveRandom.h"

Doodlebug::Doodlebug() : Organism() {}
Doodlebug::Doodlebug(Point* point) : Organism(DOODLEBUG_TYPE, DOODLEBUG_BREED_TIME, point), hunger(DOODLEBUG_HUNGER_LIMIT) {}
Doodlebug::Doodlebug(Point* point, MoveBehavior* mb) : Organism(DOODLEBUG_TYPE, DOODLEBUG_BREED_TIME, point, mb), hunger(DOODLEBUG_HUNGER_LIMIT) {}

int Doodlebug::getDoodleType() const { return getType(); }

Point Doodlebug::getDoodleLocation() const { return getPoint(); }
Point* Doodlebug::getLocation() const { return getPointer(); }

bool Doodlebug::isBreeding()
{
    if(breeding()){
        resetBreed();
        return true;
    }
    return false;
}
bool Doodlebug::isAlive() const { return (hunger > 0); }

void Doodlebug::movingDoodlebug(TheGrid::GameBoard& gb, Point* p) {
    breedTurn();
    movingOrganism(gb, p);
}
void Doodlebug::moving(const Point& newLocation) { setPoint(newLocation); }

void Doodlebug::noEat() { hunger--; }
void Doodlebug::eatAnt() { hunger = DOODLEBUG_HUNGER_LIMIT; }

void Doodlebug::setDoodleMovement(MoveRandom* mb) { setMovement(mb); }
