#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BasicDefinition.hpp"

using namespace std;

Solid* mvfs(Point _point, Vertex*& _vertex);
HalfEdge* mev(Vertex* _vertex, Point _point, Loop* _loop);
Loop* mef(Vertex* _vertex_1, Vertex* _vertex_2, Loop*& _largeLoop);
Loop* kemr(Vertex* _vertex_1, Vertex* _vertex_2, Loop*& _largeLoop);
void kfmrh(Loop* _outter_loop, Loop* _inner_loop);
Solid* sweep(Face* _face, glm::vec3 direction, float distance);