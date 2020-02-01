/*  Task:
    Даны два выпуклых многоугольника на плоскости. 
    В первом n точек, во втором m. 
    Определите, пересекаются ли они за O(n + m). 
    Указание. Используйте сумму Минковского. 

    developed by Crashdown
*/
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>


struct Point
{
    Point(double _x, double _y);

    bool lessAngleThen(Point other);
    int quater();

    double x;
    double y;
};

struct Polygon
{
    Polygon(std::vector<Point>& points);
    Polygon(){}

    bool containsZero();

    std::vector<Point> buffer;
};

double d_square(double x){
    return x * x;
}

double d_abs(double x){
    return (x >= 0.0) ? x : -x;
}

Point::Point(double _x, double _y): x(_x), y(_y){}

Point operator+(Point& a, Point& b){
    return Point(a.x + b.x, a.y + b.y);
}

Point operator-(Point& a, Point& b){
    return Point(a.x - b.x, a.y - b.y);
}

Polygon::Polygon(std::vector<Point>& points){
    int min = 0;
    double epsilon = 0.000000001;

    for(int i = 1; i < points.size(); ++i){
        if(points[i].y < points[min].y ||
           (d_abs(points[i].y - points[min].y) < epsilon &&
            points[i].x < points[min].x)){

            min = i;
        }
    }

    for(int i = 0; i < points.size(); ++i){
        buffer.push_back(points[(i + min + 1) % points.size()]);
    }

    std::reverse(buffer.begin(), buffer.end());

    buffer.push_back(buffer[0]);
}

int Point::quater(){
    if(x >= 0 && y >= 0){
        return 1;
    }

    if(x < 0 && y >= 0){
        return 2;
    }

    if(x < 0 && y < 0){
        return 3;
    }

    return 4;
}

bool Point::lessAngleThen(Point other){

    double angle1 = atan2(y, x);
    double angle2 = atan2(other.y, other.x);
    if(angle1 < 0){
        angle1 += 2 * M_PI;
    }
    if(angle2 < 0){
        angle2 += 2 * M_PI;
    }

    return angle1 < angle2;
}

bool Polygon::containsZero(){
    for(int i = 0; i < buffer.size() - 1; ++i){
        double x1 = buffer[i].x;
        double y1 = buffer[i].y;
        double x2 = buffer[i + 1].x - buffer[i].x;
        double y2 = buffer[i + 1].y - buffer[i].y;
        double vec_mul = x1 * y2 - x2 * y1;
        if(vec_mul < 0){
            return false;
        }
    }
    return true;
}


Polygon minkowsky(Polygon& a, Polygon& b){
    Polygon sum;

    int i = 0;
    int j = 0;

    while(i < a.buffer.size() - 1 && j < b.buffer.size() - 1){
        sum.buffer.push_back(a.buffer[i] + b.buffer[j]);
        Point tmp1 = a.buffer[i + 1] - a.buffer[i];
        Point tmp2 = b.buffer[j + 1] - b.buffer[j];

        if(tmp1.lessAngleThen(tmp2)){
            ++i;
            continue;
        }

        if(tmp2.lessAngleThen(tmp1)){
            ++j;
            continue;
        }

        ++i;
        ++j;
    }
    return sum;
}


int main()
{
    int n;
    std::cin >> n;
    std::vector<Point> points1;
    for(int i = 0; i < n; ++i){
        double x, y;
        std::cin >> x >> y;
        points1.push_back(Point(x, y));
    }

    int m;
    std::cin >> m;
    std::vector<Point> points2;
    for(int i = 0; i < m; ++i){
        double x, y;
        std::cin >> x >> y;
        points2.push_back(Point(-x, -y));
    }

    Polygon a = Polygon(points1);
    Polygon b = Polygon(points2);

    Polygon sum = minkowsky(a, b);

    if(sum.containsZero()){
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}
