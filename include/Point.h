#ifndef POINT_H
#define POINT_H


class Point
{
    public:
        Point();
        Point(const Point& copyPoint);
        Point(int x, int y);
        Point getPoint();
        int getX() const;
        int getY() const;
        void setPoint(int x, int y);
        void setPoint(const Point& point);
        bool operator ==(const Point& point);
    private:
        int x, y;
};

#endif // POINT_H
