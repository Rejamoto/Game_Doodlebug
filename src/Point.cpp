#include "../include/Point.h"

Point::Point() : Point(0, 0) {}
Point::Point(const Point& copyPoint) : Point(copyPoint.getX(), copyPoint.getY()) {}
Point::Point(int x, int y) { this->x = x; this->y = y; }

Point Point::getPoint() { return *this; }
int Point::getX() const { return x; }
int Point::getY() const { return y; }
void Point::setPoint(int x, int y) { this->x = x; this->y = y; }
void Point::setPoint(const Point& point) { this->x = point.getX(); this->y = point.getY(); }
bool Point::operator==(const Point& point) { return ((point.getX() == this->x) && (point.getY() == this->y)); }
