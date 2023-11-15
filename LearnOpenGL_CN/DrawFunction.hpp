#pragma once

#include "EulerOperation.hpp"
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <glut.h>

using namespace std;

void DrawSolid(Solid *_solid);
void DrawFace(Face *_face);
void DrawLoop(Loop *_loop);