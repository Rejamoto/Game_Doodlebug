#include "../include/Organism.h"
#include "../include/MoveRandom.h"

Organism::Organism() : type(0), breedPoints(0), breedTime(0), point(Point()) {}
Organism::Organism(int type, int timeTillBreed, Point point) :
    type(type), breedPoints(timeTillBreed), breedTime(timeTillBreed) { this->point = point; }
Organism::~Organism() { delete movement; }

bool Organism::breeding() const {  return (breedPoints <= 0); }
int Organism::getType() const { return type; }
Point Organism::getPoint() const { return point; }

void Organism::setType(int type) { this->type = type; }
void Organism::setPoint(const Point& newPoint) { this->point.setPoint(newPoint.getX(), newPoint.getY()); }

void Organism::resetBreed() { breedPoints = breedTime; }
void Organism::breedTurn() { breedPoints--; }

void Organism::setMovement(MoveBehavior* mb) { movement = mb; }
void Organism::movingOrganism() { movement->moving(); }
