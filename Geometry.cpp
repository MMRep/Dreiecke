#include "Geometry.hpp"

#include <cmath>

bool equal(const double& a, const double& b) {
    constexpr double EPSILON = 0.0001;
    double diff;

    diff = a - b;
    if (diff < EPSILON && (-diff) < EPSILON) {
        return true;
    }
    return false;
}

Point Point::operator-(const Point& P) const {
    return Point(x - P.x, y - P.y, z - P.z);
}

Point Point::operator+(const Point& P) const {
    return Point(x + P.x, y + P.y, z + P.z);
}

Point Point::operator*(const Point& P) const {
    // cross product
    return Point(y*P.z - z*P.y,
                 z*P.x - x*P.z,
                 x*P.y - y*P.x);
}

bool Point::operator==(const Point& P) const {
    return (equal(x, P.x) && equal(y, P.y) && equal(z, P.z));
}

bool Point::operator!=(const Point& P) const {
    return (!equal(x, P.x) || !equal(y, P.y) || !equal(z, P.z));
}

bool Point::isLocatesOn(const Line& L) const {
    return ((x > L.A.x && x < L.B.x) ^ (x < L.A.x && x > L.B.x) || equal(x, L.A.x) || equal(x, L.B.x))
        && ((y > L.A.y && y < L.B.y) ^ (y < L.A.y && y > L.B.y) || equal(y, L.A.y) || equal(y, L.B.y))
        && ((z > L.A.z && z < L.B.z) ^ (z < L.A.z && z > L.B.z) || equal(z, L.A.z) || equal(z, L.B.z));
}

bool Line::isParallelTo(const Line& L) const {
    Point RA(B - A);     // Richtungsvektor
    Point RB(L.B - L.A); // Richtungsvektor
    Point P(RA * RB);
    return (P == NullVector);
}

bool Line::intersects(const Line& L) const {
    Point S = Line::intersection(*this, L);
    return (S.isLocatesOn(*this) && S.isLocatesOn(L));
}

Point Line::intersection(const Line& A, const Line& B) {
    float s;
    float t;
    Point RA(A.B - A.A); // Richtungsvektor der Geraden A
    Point RB(B.B - B.A); // Richtungsvektor der Geraden B
    //
    // xA + s * xRA = xB + t * xRB                                                                  | -xA
    // s * xRA = xB + t * xRB - xA                                                                  | :xRA
    // s = (xB + t * xRB - xA) / xRA                                                                | Setze (xB + t * xRB - xA) / xRA für s  in yA + s * yRA = yB + t * yRB ein.
    // yA + ((xB + t * xRB - xA) / xRA) * yRA = yB + t * yRB                                        | TU
    // yA + (xB * yRA + t * xRB * yRA - xA * yRA) / xRA = yB + t * yRB                              | TU
    // yA + (xB * yRA) / xRA + (t * xRB * yRA) / xRA - (xA * yRA) / xRA = yB + t * yRB              | -(t * xRB * yRA) / xRA, -yB
    // yA + (xB * yRA) / xRA - (xA * yRA) / xRA - yB = t * yRB - (t * xRB * yRA) / xRA              | TU
    // yA + (xB * yRA) / xRA - (xA * yRA) / xRA - yB = (xRA * t * yRB - t * xRB * yRA) / xRA        | TU
    // yA + (xB * yRA) / xRA - (xA * yRA) / xRA - yB = (t * (xRA * yRB - xRB * yRA)) / xRA          | *xRA
    // yA * xRA + xB * yRA - xA * yRA - yB * xRA = t * (xRA * yRB - xRB * yRA)                      | :(xRA * yRB - xRB * yRA)
    //   (yA * xRA) / (xRA * yRB - xRB * yRA) 
    // + (xB * yRA) / (xRA * yRB - xRB * yRA)
    // - (xA * yRA) / (xRA * yRB - xRB * yRA)
    // - (yB * xRA) / (xRA * yRB - xRB * yRA) = t                                                   | TU
    // 
    t = ((A.A.y * RA.x) / (RA.x * RB.y - RB.x * RA.y))
      + ((B.A.x * RA.y) / (RA.x * RB.y - RB.x * RA.y))
      - ((A.A.x * RA.y) / (RA.x * RB.y - RB.x * RA.y))
      - ((B.A.y * RA.x) / (RA.x * RB.y - RB.x * RA.y));
    s = (B.A.x + t * RB.x - A.A.x) / RA.x;
    return Point(B.A.x + t * RB.x, B.A.y + t * RB.y, B.A.z + t * RB.z);
}

bool isTriangle(const Line& A, const Line& B, const Line& C) {
    bool cond1;
    bool cond2;
    bool cond3;
    // Bedingung 1: Keine 2 parallelen Geraden
    cond1 = !(A.isParallelTo(B) || B.isParallelTo(C) || C.isParallelTo(A));
    // Bedingung 2: Die Schnittpunkte der drei Geraden müssen auf den Strecken liegen
    cond2 = A.intersects(B) && B.intersects(C) && C.intersects(A);
    // Bedingung 3: Es existieren keine 2 gleichen Schnittpunkten
    Point S1 = Line::intersection(A, B);
    Point S2 = Line::intersection(B, C);
    Point S3 = Line::intersection(C, A);
    cond3 = (S1 != S2 && S2 != S3 && S3 != S1);
    return cond1 && cond2 && cond3;
}
