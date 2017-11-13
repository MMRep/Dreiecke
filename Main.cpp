#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "Geometry.hpp"
#include "Window.hpp"

std::vector<Line> lines;
std::vector<Triangle> triangles;

int main(int argc, char* argv[]) {
    Point A;
    Point B;
    int Cnt;

    // prepare rand() function 
    srand((unsigned int)time(NULL));
    
    // read file with coordinates
    std::ifstream file("dreiecke6.txt");
    file >> Cnt;
    for (auto i = 0; i < Cnt; ++i) {
        file >> A.x;
        file >> A.y;
        file >> B.x;
        file >> B.y;
        // create line and insert it into a list
        lines.push_back(Line(A, B));
    }

    // set up print format
    std::cout << std::fixed << std::setprecision(2);

    // n choose k  iterations with n = 3 and k = cardinality of lines
    for (unsigned int i = 0; i < lines.size(); ++i) {
        for (unsigned int j = 0; j < lines.size(); ++j) {
            for (unsigned int k = 0; k < lines.size(); ++k) {
                if (i >= j || j >= k || k == i) {
                    continue; // skip repeating combinations
                } else if (isTriangle(lines[i], lines[j], lines[k])) {
                    // if combination is a triangle, print the triangle's coordinates and add it to a list
                    Point P1 = Line::intersection(lines[i], lines[j]); // compute 1st vertex
                    Point P2 = Line::intersection(lines[j], lines[k]); // compute 2nd vertex
                    Point P3 = Line::intersection(lines[k], lines[i]); // compute 3rd vertex
                    triangles.push_back(Triangle(P1, P2, P3));
                    std::cout << std::setw(8) << P1.x << " " << std::setw(8) << P1.y << " "
                              << std::setw(8) << P2.x << " " << std::setw(8) << P2.y << " "
                              << std::setw(8) << P3.x << " " << std::setw(8) << P3.y << std::endl;
                }
            }
        }
    }

    InitWindow();
    StartWindow();
    return 0;
}