#ifndef _GEOMETRY_HPP_
#define _GEOMETRY_HPP_

bool equal(const double& a, const double& b);

struct Line;

struct Point {
    float x;
    float y;
    float z;
    constexpr Point() : x(0.), y(0.), z(0.) {}
    Point(const float x, const float y, const float z = 0) : x(x), y(y), z(z) {}
    Point(const Point& P) : x(P.x), y(P.y), z(P.z) {}
    Point operator-(const Point& P) const;
    Point operator+(const Point& P) const;
    Point operator*(const Point& P) const;
    bool operator==(const Point& P) const;
    bool operator!=(const Point& P) const;
    bool  isLocatesOn(const Line& L) const;
};

static const Point NullVector;

struct Line {
    Point A;
    Point B;
    Line() = delete;
    Line(const Point& A, const Point& B) : A(A),   B(B) {}
    Line(const Line& L)                  : A(L.A), B(L.B) {}
    bool         isParallelTo(const Line& L) const;
    bool         intersects  (const Line& L) const;
    static Point intersection(const Line& A, const Line& B);
};

bool isTriangle(const Line& A, const Line& B, const Line& C);

struct Triangle {
    Point A;
    Point B;
    Point C;
    Triangle(const Point& A, const Point& B, const Point& C) : A(A), B(B), C(C) {}
};
#endif