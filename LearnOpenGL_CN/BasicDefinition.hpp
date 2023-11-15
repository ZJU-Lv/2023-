#pragma once

#include <iostream>
#include <glm/glm.hpp>

using namespace std;

class Face;
class HalfEdge;
class Edge;
class Loop;
class Vertex;
class Edge;
class Point;

class Solid
{
public:
	Solid() : prevS(nullptr), nextS(nullptr), sFace(nullptr), edgeList(nullptr) {}
	Solid* prevS, * nextS;
	Face* sFace;
	Edge* edgeList;
};

class HalfEdge
{
public:
	HalfEdge() : heLoop(nullptr), prevHe(nullptr), edge(nullptr), nextHe(nullptr), adjacentHe(nullptr), startV(nullptr), endV(nullptr) {}
	Loop* heLoop;
	HalfEdge* prevHe, * nextHe;
	HalfEdge* adjacentHe;
	Edge* edge;
	Vertex* startV, * endV;
};

class Edge
{
public:
	Edge() : prevE(nullptr), nextE(nullptr), firstHe(nullptr), secondHe(nullptr) {}
	Edge* prevE, * nextE;
	HalfEdge* firstHe, * secondHe;
};

class Loop
{
public:
	Loop() : lFace(nullptr), prevL(nullptr), nextL(nullptr), lHalfEdge(nullptr), innerLoop(false) {}
	Face* lFace;
	Loop* prevL, * nextL;
	HalfEdge* lHalfEdge;
	bool innerLoop;
	bool isInnerLoop() const {
		return innerLoop;
	}
	void SetInnerLoop(bool _state) {
		innerLoop = _state;
		return;
	}

	int size() {
		if (lHalfEdge == nullptr)
		{
			return 0;
		}
		else
		{
			int count = 0;
			HalfEdge* startHe = lHalfEdge;
			HalfEdge* he = lHalfEdge;
			do
			{
				count++;
				he = he->nextHe;
			} while (nullptr != he && he != startHe);
			return count;
		}
	}
};

class Face
{
public:
	Face() : fSolid(nullptr), prevF(nullptr), nextF(nullptr), fLoop(nullptr) {}
	Solid* fSolid;
	Face* prevF, * nextF;
	Loop* fLoop;

	int size() {
		if (fLoop == nullptr)
		{
			return 0;
		}
		else
		{
			int count = 0;
			Loop* startL = fLoop;
			Loop* l = fLoop;
			do
			{
				count++;
				l = l->nextL;
			} while (nullptr != l && l != startL);
			return count;
		}
	}
};

class Vertex
{
public:
	Vertex() : prevV(nullptr), nextV(nullptr), point(nullptr) {}
	Vertex* prevV, * nextV;
	Point* point;
};

class Point
{
public:
	Point() : p(0, 0, 0) {
		color = glm::vec3(rand() % 3 * 0.5f, 0.2f, 0.2f);
	}
	Point(double _x, double _y, double _z) : p(_x, _y, _z) {
		color = glm::vec3(rand() % 3 * 0.5f, 0.2f, 0.2f);
	}
	void SetPoint(double _x, double _y, double _z)
	{
		p.x = _x;
		p.y = _y;
		p.z = _z;
	}
	void SetPoint(Point _point)
	{
		p.x = _point.p.x;
		p.y = _point.p.y;
		p.z = _point.p.z;
	}
	friend inline ostream& operator<<(ostream& os, Point& point)
	{
		os << "( " << point.p.x << ", " << point.p.y << ", " << point.p.z << " )";
		return os;
	}
	glm::vec3 p;
	glm::vec3 color;
};