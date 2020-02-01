/*  Task:
    Даны два отрезка в пространстве 
    (x1, y1, z1) - (x2, y2, z2) и 
    (x3, y3, z3) - (x4, y4, z4). 
    Найдите расстояние между отрезками. 
    developed by Crashdown
*/
#include <iostream>
#include <math.h>

struct Point
{
    double x;
    double y;
    double z;

    Point(double _x, double _y, double _z);

    double distanceToPoint(Point other);
    double distance2ToPoint(Point other);
    double max_diff(Point other);


    friend Point operator+(const Point& a, const Point& b);
    friend Point operator-(const Point& a, const Point& b);
    friend Point operator/(const Point& a, int den);
      
};

class Vector3
{
public:
    Vector3(Point x);
    Vector3(Point start, Point end);

    double distanceToPoint(Point point);
    double distance2ToPoint(Point point);

    double distanceToVector(Vector3 other);
    double distance2ToVector(Vector3 other);

    Point ternarySearchLeft(const Point& a, const Point& b) const;
    Point ternarySearchRight(const Point& a, const Point& b) const;

private:
    Point start;
    Point end;

    double _x;
    double _y;
    double _z;
};


double d_square(double x){
    return x * x;
}

double d_abs(double x){
    return (x >= 0.0) ? x : -x;
}

Point::Point(double _x, double _y, double _z): x(_x), y(_y), z(_z){}

Point operator+(const Point& a, const Point& b){
    return Point (a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator-(const Point& a, const Point& b){
    return Point (a.x - b.x, a.y - b.y, a.z - b.z);
}

Point operator/(const Point& a, int den){
    return Point(a.x / den, a.y / den, a.z / den);
}

double Point::distance2ToPoint(Point other){
    return d_square(x - other.x) + d_square(y - other.y) + d_square(z - other.z);
}

double Point::distanceToPoint(Point other){
    return sqrt(distance2ToPoint(other));
}

Point Vector3::ternarySearchLeft(const Point& a, const Point& b) const{
    return a + (b - a) / 3;
}

Point Vector3::ternarySearchRight(const Point& a, const Point& b) const{
    return b - (b - a) / 3;
}

Vector3::Vector3(Point a): start(Point(0, 0, 0)), end(a), _x(a.x), _y(a.y), _z(a.z){}

Vector3::Vector3(Point _start, Point _end): Vector3(_start - _end){
    start = _start;
    end = _end;
}

double Vector3::distance2ToPoint(Point point){
    double dst2_to_start = start.distance2ToPoint(point);
    double dst2_to_end = end.distance2ToPoint(point);

    bool flag = true;

    if(dst2_to_start > dst2_to_end){
        std::swap(dst2_to_start, dst2_to_end);
        flag = false;
    }

    Point closest_point = (flag) ? start : end;

    double length2 = _x * _x + _y * _y + _z * _z;

    Point direct = point - closest_point;

    double vec_mul2 = d_square(direct.y * _z - direct.z * _y) +
                      d_square(direct.x * _y - direct.y * _x) +
                      d_square(direct.x * _z - direct.z * _x);


    double normal2 = vec_mul2 / length2;

    if(dst2_to_start + length2 - dst2_to_end > 0){
        return normal2;
    }

    return dst2_to_start;
}

double Vector3::distanceToPoint(Point point){
    return sqrt(distance2ToPoint(point));
}


double Vector3::distance2ToVector(Vector3 other){
    Point left = start;
    Point right = end;
    double epsilon = 0.000000001;

    if(other.distance2ToPoint(left) > other.distance2ToPoint(right)){
        std::swap(left, right);
    }

    while(left.distance2ToPoint(right) > d_square(epsilon)){
         Point m1 = ternarySearchLeft(left, right);
         Point m2 = ternarySearchRight(left, right);

        if(other.distance2ToPoint(m1) < other.distance2ToPoint(m2)){
            right = m2;
        } else {
            left = m1;
        }
    }

    return (other.distance2ToPoint(right) + other.distance2ToPoint(left)) / 2;
}

double Vector3::distanceToVector(Vector3 other){
    return sqrt(distance2ToVector(other));
}

int main()
{
    double x1, y1, z1, x2, y2, z2;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    double x3, y3, z3, x4, y4, z4;
    std::cin >> x3 >> y3 >> z3 >> x4 >> y4 >> z4;
    Vector3 first = Vector3(Point(x1, y1, z1), Point(x2, y2, z2));
    Vector3 second = Vector3(Point(x3, y3, z3), Point(x4, y4, z4));

    printf("%.6f\n", first.distanceToVector(second));

    return 0;
}

