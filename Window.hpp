#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>

#include "Geometry.hpp"

extern std::vector<Line> lines;
extern std::vector<Triangle> triangles;

void InitWindow();
void StartWindow();

#endif // _WINDOW_HPP_