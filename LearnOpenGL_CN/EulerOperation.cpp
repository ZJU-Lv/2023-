#include "EulerOperation.hpp"

void swapLoop(Loop*& loop_1, Loop*& loop_2)
{
	Loop* temp = loop_1;
	loop_1 = loop_2;
	loop_2 = temp;
}


Solid* mvfs(Point _point, Vertex*& _vertex)
{
	Solid* createdSolid = new Solid();
	Face* createdFace = new Face();
	Loop* createdLoop = new Loop();

	_vertex = new Vertex();
	_vertex->point = new Point();
	_vertex->point->SetPoint(_point);

	createdSolid->sFace = createdFace;
	createdFace->fSolid = createdSolid;

	createdFace->fLoop = createdLoop;
	createdLoop->lFace = createdFace;

	return createdSolid;
}

HalfEdge* mev(Vertex* _vertex, Point _point, Loop* _loop)
{
	HalfEdge* he1 = new HalfEdge();
	HalfEdge* he2 = new HalfEdge();
	Edge* newEdge = new Edge();

	newEdge->firstHe = he1;
	newEdge->secondHe = he2;
	he1->edge = newEdge;
	he2->edge = newEdge;
	he1->adjacentHe = he2;
	he2->adjacentHe = he1;

	Vertex* newVertex = new Vertex();
	newVertex->point = new Point();
	newVertex->point->SetPoint(_point);

	he1->startV = _vertex;
	he1->endV = newVertex;
	he2->startV = newVertex;
	he2->endV = _vertex;

	he1->heLoop = _loop;
	he2->heLoop = _loop;

	he1->nextHe = he2;
	he2->prevHe = he1;
	if (nullptr == _loop->lHalfEdge)
	{
		he2->nextHe = he1;
		he1->prevHe = he2;
		_loop->lHalfEdge = he1;
	}
	else
	{
		HalfEdge* currentHe;
		for (currentHe = _loop->lHalfEdge; currentHe->nextHe->startV != _vertex; currentHe = currentHe->nextHe)
		{
		}
		he2->nextHe = currentHe->nextHe;
		currentHe->nextHe->prevHe = he2;

		he1->prevHe = currentHe;
		currentHe->nextHe = he1;
	}

	Solid* oldSolid = _loop->lFace->fSolid;
	Edge* existingEdge = oldSolid->edgeList;
	if (nullptr == existingEdge)
	{
		oldSolid->edgeList = newEdge;
	}
	else
	{
		while (existingEdge->nextE != nullptr)
		{
			existingEdge = existingEdge->nextE;
		}
		existingEdge->nextE = newEdge;
		newEdge->prevE = existingEdge;
	}
	return he1;
}

Loop* mef(Vertex* _vertex_1, Vertex* _vertex_2, Loop*& _largeLoop)
{
	HalfEdge* he1 = new HalfEdge();
	HalfEdge* he2 = new HalfEdge();
	Edge* newEdge = new Edge();

	newEdge->firstHe = he1;
	newEdge->secondHe = he2;
	he1->edge = newEdge;
	he2->edge = newEdge;
	he1->adjacentHe = he2;
	he2->adjacentHe = he1;

	he1->startV = _vertex_1;
	he1->endV = _vertex_2;
	he2->startV = _vertex_2;
	he2->endV = _vertex_1;

	HalfEdge* currentHE;
	HalfEdge* startHE;
	int repeatLoopCheck;

	startHE = currentHE = _largeLoop->lHalfEdge;
	repeatLoopCheck = 0;
	while (currentHE->startV != _vertex_1)
	{
		if (currentHE == startHE && repeatLoopCheck == 2)
		{
			cout << "Error: EulerOperation::mef cannot find vertex." << endl;
			return nullptr;
		}
		if (currentHE == startHE && repeatLoopCheck != 2)
		{
			repeatLoopCheck++;
		}
		currentHE = currentHE->nextHe;
	}
	HalfEdge* firstHalfEdge = currentHE;

	startHE = currentHE = _largeLoop->lHalfEdge;
	repeatLoopCheck = 0;
	while (currentHE->startV != _vertex_2)
	{
		if (currentHE == startHE && repeatLoopCheck == 2)
		{
			cout << "Error: EulerOperation::mef cannot find vertex." << endl;
			return nullptr;
		}
		if (currentHE == startHE && repeatLoopCheck != 2)
		{
			repeatLoopCheck++;
		}
		currentHE = currentHE->nextHe;
	}
	HalfEdge* secondHalfEdge = currentHE;

	he2->nextHe = firstHalfEdge;
	he2->prevHe = secondHalfEdge->prevHe;
	he1->nextHe = secondHalfEdge;
	he1->prevHe = firstHalfEdge->prevHe;
	firstHalfEdge->prevHe->nextHe = he1;
	firstHalfEdge->prevHe = he2;
	secondHalfEdge->prevHe->nextHe = he2;
	secondHalfEdge->prevHe = he1;

	_largeLoop->lHalfEdge = he1;
	Loop* newLoop = new Loop();
	newLoop->lHalfEdge = he2;

	Solid* oldSolid = _largeLoop->lFace->fSolid;

	Face* newFace = new Face();
	newFace->fLoop = newLoop;
	newLoop->lFace = newFace;
	newFace->fSolid = oldSolid;

	Face* currentFace;
	for (currentFace = oldSolid->sFace; currentFace->nextF != nullptr; currentFace = currentFace->nextF)
	{
	}
	currentFace->nextF = newFace;
	newFace->prevF = currentFace;

	Edge* existingEdge = oldSolid->edgeList;
	if (nullptr == existingEdge)
	{
		oldSolid->edgeList = newEdge;
	}
	else
	{
		while (existingEdge->nextE != nullptr)
		{
			existingEdge = existingEdge->nextE;
		}
		existingEdge->nextE = newEdge;
		newEdge->prevE = existingEdge;
	}
	if (newLoop->size() > _largeLoop->size())
	{
		swapLoop(newLoop, _largeLoop);
	}
	return newLoop;
}

Loop* kemr(Vertex* _vertex_1, Vertex* _vertex_2, Loop*& _largeLoop)
{
	Loop* newLoop = new Loop();

	HalfEdge* currentHE;
	for (currentHE = _largeLoop->lHalfEdge; !(_vertex_1 == currentHE->startV && _vertex_2 == currentHE->endV); currentHE = currentHE->nextHe)
	{

	}

	Edge* targetEdge = currentHE->edge;
	currentHE->nextHe->prevHe = currentHE->adjacentHe->prevHe;
	currentHE->adjacentHe->prevHe->nextHe = currentHE->nextHe;

	currentHE->prevHe->nextHe = currentHE->adjacentHe->nextHe;
	currentHE->adjacentHe->nextHe->prevHe = currentHE->prevHe;

	_largeLoop->lHalfEdge = currentHE->prevHe;
	newLoop->lHalfEdge = currentHE->nextHe;

	Face* existingFace = _largeLoop->lFace;
	newLoop->lFace = existingFace;

	Loop* currentLoop;
	for (currentLoop = existingFace->fLoop; nullptr != currentLoop->nextL; currentLoop = currentLoop->nextL)
	{
	}
	currentLoop->nextL = newLoop;
	newLoop->prevL = currentLoop;

	Solid* existingSolid = _largeLoop->lFace->fSolid;
	targetEdge = existingSolid->edgeList;
	if (targetEdge == currentHE->edge)
	{
		existingSolid->edgeList = currentHE->edge->nextE;
		existingSolid->edgeList->prevE = targetEdge->prevE;
	}
	else
	{
		while (targetEdge->nextE != currentHE->edge)
		{
			targetEdge = targetEdge->nextE;
		}
		targetEdge->nextE = currentHE->edge->nextE;
		targetEdge->nextE->prevE = targetEdge;
	}
	if (newLoop->size() > _largeLoop->size())
	{
		swapLoop(newLoop, _largeLoop);
	}
	return newLoop;
}

void kfmrh(Loop* _outer_loop, Loop* _inner_loop)
{
	Face* outerFace = _outer_loop->lFace;
	Face* innerFace = _inner_loop->lFace;

	Loop* currentLoop;
	for (currentLoop = outerFace->fLoop; nullptr != currentLoop->nextL; currentLoop = currentLoop->nextL)
	{
	}
	currentLoop->nextL = _inner_loop;
	_inner_loop->prevL = currentLoop;
	_inner_loop->lFace = outerFace;

	Solid* existingSolid = outerFace->fSolid;
	Face* currentFace = existingSolid->sFace;
	if (currentFace == innerFace)
	{
		existingSolid->sFace = currentFace->nextF;
		existingSolid->sFace->prevF = currentFace->prevF;
	}
	else
	{
		while (currentFace->nextF != innerFace)
		{
			currentFace = currentFace->nextF;
		}
		currentFace->nextF = innerFace->nextF;
		currentFace->nextF->prevF = currentFace;
	}

	return;
}

Solid* sweep(Face* _face, glm::vec3 direction, float distance)
{
	Solid* solid = _face->fSolid;
	Loop* currentLoop;
	HalfEdge* currentHe;
	for (currentLoop = _face->fLoop; currentLoop != nullptr; currentLoop = currentLoop->nextL)
	{
		currentHe = currentLoop->lHalfEdge;
		Vertex* originalVertex = currentHe->startV;
		glm::vec3 newPosition = originalVertex->point->p + distance * direction;
		Point newPoint(newPosition.x, newPosition.y, newPosition.z);

		HalfEdge* firstConnectionHe = mev(originalVertex, newPoint, currentLoop);
		Vertex* connectedVertex_1 = firstConnectionHe->endV;

		currentHe = currentHe->nextHe;
		Vertex* v = currentHe->startV;
		while (v != originalVertex)
		{
			glm::vec3 newPosition = v->point->p + distance * direction;
			Point newPoint(newPosition.x, newPosition.y, newPosition.z);

			HalfEdge* connectionHe = mev(v, newPoint, currentLoop);
			Vertex* connectedVertex_2 = connectionHe->endV;
			mef(connectedVertex_2, connectedVertex_1, currentLoop);
			connectedVertex_1 = connectedVertex_2;
			currentHe = currentHe->nextHe;
			v = currentHe->startV;
		}
		mef(firstConnectionHe->endV, connectedVertex_1, currentLoop);
	}
	return solid;
}