/*  Task:
    Даны n точек в пространстве. 
    Никакие 4 точки не лежат в одной плоскости. 
    Найдите выпуклую оболочку этих точек за O(n*log(n)). 
    developed by Crashdown    
*/
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <stack>
#include <set>


struct Point
{
    Point(double _x, double _y, double _z, int _index);
    Point();

    bool lessAngleThen(const Point& other) const;

    double x;
    double y;
    double z;

    int index;
};

struct Vector3 
{
    double x;
    double y;
    double z;

    Vector3(double _x, double _y, double _z): x(_x), y(_y), z(_z){}
    Vector3(const Point& start, const Point& end): x(end.x - start.x), y(end.y - start.y), z(end.z - start.z){}

    double length() const{
        return sqrt(x * x + y * y + z * z);
    }
};

struct Plane
{
    double A;
    double B;
    double C;
    double D;

    Plane(const Point& p1, const Point& p2, const Point& p3);
    Plane(const Point& p, const Vector3& v1, const Vector3& v2);
    Plane(double _A, double _B, double _C, double _D);

    Vector3 normal_vector() const;

    double angle_cos2(const Plane& other) const;
    double norm2() const;
};

double d_square(double x){
    return x * x;
}

double d_abs(double x){
    return (x >= 0.0) ? x : -x;
}

Vector3 vector_mul(const Vector3& a, const Vector3& b){
    double det1 = a.y * b.z - b.y * a.z;
    double det2 = a.x * b.z - b.x * a.z;
    double det3 = a.x * b.y - b.x * a.y;

    return Vector3(det1, -1 * det2, det3);
}

double scalar_mul(const Vector3& a, const Vector3& b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point::Point(double _x, double _y, double _z, int _index): x(_x), y(_y), z(_z), index(_index){}

Point::Point(): x(0), y(0), z(0), index(-1){}

Point operator+(const Point& a, const Point& b){
    return Point(a.x + b.x, a.y + b.y, a.z + b.z, -1);
}

Point operator-(const Point& a, const Point& b){
    return Point(a.x - b.x, a.y - b.y, a.z - b.z, -1);
}

Plane::Plane(double _A, double _B, double _C, double _D): A(_A), B(_B), C(_C), D(_D){}

Plane::Plane(const Point& p1, const Point& p2, const Point& p3){
    double det1 = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
    double det2 = (p2.x - p1.x) * (p3.z - p1.z) - (p3.x - p1.x) * (p2.z - p1.z);
    double det3 = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

    A = det1;
    B = -1 * det2;
    C = det3;
    D = - p1.x * det1 + p1.y * det2 - p1.z * det3;
}

Plane::Plane(const Point& p, const Vector3& v1, const Vector3& v2){
    double det1 = v1.y * v2.z - v2.y * v1.z;
    double det2 = v1.x * v2.z - v2.x * v1.z;
    double det3 = v1.x * v2.y - v2.x * v1.y;

    A = det1;
    B = -1 * det2;
    C = det3;
    D = - p.x * det1 + p.y * det2 - p.z * det3;
}

Vector3 Plane::normal_vector() const{
    return Vector3(A, B, C);
}

double Plane::norm2() const{
    //  returns squared length of norm vector
    return d_square(A) + d_square(B) + d_square(C); 
}

double Plane::angle_cos2(const Plane& other) const{
    //  returns squared cosine of angle between two planes
    return d_square(A * other.A + B * other.B + C * other.C) / norm2() / other.norm2();
}

int min_point(const std::vector< Point >& polygon){
    //  returns down left point of polygon
    int min = 0;
    for(int i = 1; i < polygon.size(); ++i){
        if(polygon[i].z < polygon[min].z){
            min = i;
            continue;
        }

        if(polygon[i].z == polygon[min].z && polygon[i].x < polygon[min].x){
            min = i;
            continue;
        }

        if(polygon[i].z == polygon[min].z && polygon[i].x == polygon[min].x && polygon[i].y < polygon[min].y){
            min = i;
        }
    }

    return min;
}

struct Edge
{
    int first;
    int second;

    Edge(int _first, int _second): first(_first), second(_second){}
    Edge(const Point& _first, const Point& _second): first(_first.index), second(_second.index){}

    bool operator<(const Edge& other) const {
        if(first != other.first){
            return first < other.first;
        }
        return second < other.second;
    }
};

struct Plane_set
{
    //  not sorted verticies
    int first;
    int second;
    int third;
    //  sorted verticies
    int s_first;
    int s_second;
    int s_third;


    Plane_set(int _first, int _second, int _third): first(_first), second(_second), third(_third){
        s_first = _first;
        s_second = _second;
        s_third = _third;

        if(s_first > s_second){
            std::swap(s_first, s_second);
        }
        
        if(s_second > s_third){
            std::swap(s_second, s_third);
        }

        if(s_first > s_second){
            std::swap(s_first, s_second);
        }
    }

    void _sort(){
        if(second < first && second < third){
            std::swap(first, second);
            std::swap(second, third);
            return;
        }

        if(third < first && third < second){
            std::swap(second, third);
            std::swap(first, second);
        }
    }

    bool operator<(const Plane_set& other) const {
        if(s_first != other.s_first){
            return s_first < other.s_first;
        }

        if(s_second != other.s_second){
            return s_second < other.s_second;
        }

        return s_third < other.s_third;
    }
};

std::vector< Plane_set > min_convex_hull(const std::vector< Point >& polygon){
    //  P0 (_first_point) - down left point of polygon
    int _first_point = min_point(polygon);
    Point P0 = polygon[_first_point];
    //  M & N - random points in alpha
    Point M = Point(P0.x + 1, P0.y, P0.z, -1);
    Point N = Point(P0.x, P0.y + 1, P0.z, -1);

    Vector3 horizontal = Vector3(0, 0, 1);

    //  P1 (_second_point) - point from polygon which creates min angle between planes alpha and (M N P1)
    int _second_point = -1;
    double angle = 1.0;
    for(int i = 0; i < polygon.size(); ++i){
        if(i == _first_point){
            continue;
        }

        Vector3 forming = Vector3(P0, polygon[i]);
        double tmp = scalar_mul(horizontal, forming) / forming.length();
        if(tmp < angle){
            _second_point = i;
            angle = tmp;
        }
    }

    Point P1 = polygon[_second_point];

    //  P2 (_third_point) - point from polygon which creates min angle between planes beta and (P0 P1 P2)
    int _third_point = -1;
    angle = 0;
    for(int i = 0; i < polygon.size(); ++i){
        if(i == _first_point || i == _second_point){
            continue;
        }

        Vector3 forming1 = Vector3(P0, P1);
        Vector3 forming2 = Vector3(P0, polygon[i]);
        Vector3 normal = vector_mul(forming1, forming2);

        double tmp = d_abs(scalar_mul(normal, horizontal)) / normal.length();
        if(tmp > angle){
            _third_point = i;
            angle = tmp;
        }
    }
    Point P2 = polygon[_third_point];

    //  sort points in required order
  
    //  struct Plane_set stores index of 2 poligon in straight order
    std::set< Plane_set > planes;
    //  struct Edge stores indexes of 2 points from polygon in straight order
    std::stack< Edge > edges;

    std::set< Edge > used;

    if(scalar_mul(vector_mul(horizontal, Vector3(P0, P1)), Vector3(P0, P2)) >= 0){
        std::swap(P1, P2);
    }

    edges.push(Edge(P0, P1));
    edges.push(Edge(P1, P2));
    edges.push(Edge(P2, P0));

    Plane_set bottom = Plane_set(P0.index, P1.index, P2.index);
    bottom._sort();
    planes.insert(bottom);

    while(!edges.empty()){
        Edge edge = edges.top();
        edges.pop();

        if(used.find(edge) != used.end()){
            continue;
        }

        used.insert(edge); 

        int vertex = 0;
        for(int i = 0; i < polygon.size(); ++i){

            if(scalar_mul(vector_mul(Vector3(polygon[edge.first], polygon[edge.second]), 
                                     Vector3(polygon[edge.first], polygon[vertex])), 
                          Vector3(polygon[edge.first], polygon[i])) < 0){
                vertex = i;
            }
        }

        if(vertex == edge.first || vertex == edge.second){
            continue;
        }

        Plane_set next = Plane_set(edge.second, edge.first, vertex);
        next._sort();
        planes.insert(next);

        edges.push(Edge(edge.first, vertex));
        edges.push(Edge(vertex, edge.second));
    }

    std::vector< Plane_set > result;
    for(auto plane : planes){
        plane._sort();
        result.push_back(plane);
    }
    return result;
}


int main()
{
    int tests_num;
    std::cin >> tests_num;
    for(int test = 0; test < tests_num; ++test){
        int n;
        std::cin >> n;
        std::vector< Point > polygon;

        for(int i = 0; i < n; ++i){
            double x, y, z;
            std::cin >> x >> y >> z;
            polygon.push_back(Point(x, y, z, i));
        }

        std::vector< Plane_set > planes = min_convex_hull(polygon);
        std::sort(planes.begin(), planes.end(), [](Plane_set p1, Plane_set p2) {
            if(p1.first != p2.first){
                return p1.first < p2.first;
            }

            if(p1.second != p2.second){
                return p1.second < p2.second;
            }

            return p1.third < p2.third;
        });

        std::cout << planes.size() << "\n";
        for(Plane_set plane : planes){
            std::cout << 3 << " " << plane.first << " " << plane.second << " " << plane.third << "\n";
        }
    }

    return 0;
}
