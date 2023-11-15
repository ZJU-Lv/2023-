#include "DrawFunction.hpp"

using namespace std;

void DrawLoop(Loop* _loop, float _r, float _g, float _b)
{
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glEnd();
}

void DrawSolid(Solid* _solid)
{
	Face* startF = _solid->sFace;
	Face* f = _solid->sFace;
	do
	{
		if (f != nullptr)
			DrawFace(f);
		f = f->nextF;
	} while (nullptr != f && f != startF);
}

void DrawFace(Face* _face)
{
	Loop* startL;
	Loop* l;

	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_ALWAYS, 1, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	startL = _face->fLoop;
	l = _face->fLoop;
	do
	{

		if (l != nullptr && l->isInnerLoop())
		{
			DrawLoop(l);
		}
		l = l->nextL;
	} while (nullptr != l && l != startL);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_EQUAL, 0, ~0);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	startL = _face->fLoop;
	l = _face->fLoop;
	do
	{
		if (l != nullptr && !l->isInnerLoop())
		{
			DrawLoop(l);
		}
		l = l->nextL;
	} while (nullptr != l && l != startL);
	
	glDisable(GL_STENCIL_TEST);
}

struct Data
{
	GLdouble data[6];
	Data(glm::vec3 point, glm::vec3 color)
	{
		data[0] = point.x;
		data[1] = point.y;
		data[2] = point.z;
		data[3] = color.r;
		data[4] = color.g;
		data[5] = color.b;
	}
};

void CALLBACK tessBeginCB(GLenum which)
{
	glBegin(which);
}

void CALLBACK tessEndCB()
{
	glEnd();
}

void CALLBACK tessVertexCB(const GLvoid* data)
{
	const GLdouble* ptr = (const GLdouble*)data;

	glColor3dv(ptr + 3);
	glVertex3dv(ptr);
}


void DrawLoop(Loop* _loop)
{
	GLuint id = glGenLists(1);
	if (!id)
		return;

	GLUtesselator* tess = gluNewTess();
	if (!tess)
		return;

	GLdouble quad1[6] = { -1, 3, 0, 1, 1, 1 };
	GLdouble quad2[6] = { 0, 0, 0, 1, 1, 1 };
	GLdouble quad3[6] = { 1, 3, 0, 1, 1, 1 };
	GLdouble quad4[6] = { 0, 2, 0, 1, 1, 1 };
	GLdouble quad5[6] = { -1, 5, 0, 1, 1, 1 };

	gluTessCallback(tess, GLU_TESS_BEGIN, (void(CALLBACK*)())tessBeginCB);
	gluTessCallback(tess, GLU_TESS_END, (void(CALLBACK*)())tessEndCB);
	gluTessCallback(tess, GLU_TESS_VERTEX, (void(CALLBACK*)())tessVertexCB);

	glNewList(id, GL_COMPILE);
	glColor3f(1, 1, 1);
	gluTessBeginPolygon(tess, 0);
	gluTessBeginContour(tess);

	HalfEdge* startHe = _loop->lHalfEdge;
	HalfEdge* he = _loop->lHalfEdge;
	Vertex* startV = he->startV;

	vector<Data> dataList;
	do
	{
		glm::vec3 point = he->startV->point->p;
		glm::vec3 color = he->startV->point->color;
		Data data(point, color);
		dataList.push_back(data);
		he = he->nextHe;
	} while (nullptr != he && he != startHe);
	for (int i = 0; i < dataList.size(); i++)
	{
		gluTessVertex(tess, dataList[i].data, dataList[i].data);
	}

	gluTessEndContour(tess);
	gluTessEndPolygon(tess);
	glEndList();

	gluDeleteTess(tess);

	glCallList(id);
}