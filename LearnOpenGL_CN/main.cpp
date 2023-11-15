#include <vector>
#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KeyController.hpp"
#include "EulerOperation.hpp"
#include "DrawFunction.hpp"


using namespace std;
using namespace glm;

#define PI 3.14159265358979323846

const glm::ivec2 SCREEN_SIZE(1280, 640);
const float FAR_PLANE = 100.0f;
const float NEAR_PLANE = 1.0f;

GLFWwindow* window;

int InitGLFW()
{
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Final Project", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    return 0;
}

void InitController()
{
    if (!Controller::Initialize())
    {
        std::cout << "Initialize controller fail!" << endl;
    }

    glfwSetKeyCallback(window, Controller::KeyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Solid* MainSolid()
{
    Solid* mainSolid;

    Vertex* topUpperLeft;
    mainSolid = mvfs(Point(-3, 1, 3), topUpperLeft);
    Loop* topLoop = mainSolid->sFace->fLoop;
    HalfEdge* topUpperHalfEdge = mev(topUpperLeft, Point(3, 1, 3), topLoop);
    Vertex* topUpperRight = topUpperHalfEdge->endV;
    HalfEdge* topLeftHalfEdge = mev(topUpperLeft, Point(-3, 1, -3), topLoop);
    Vertex* topBottomLeft = topLeftHalfEdge->endV;
    HalfEdge* topRightHalfEdge = mev(topUpperRight, Point(3, 1, -3), topLoop);
    Vertex* topBottomRight = topRightHalfEdge->endV;

    Loop* downLoop = mef(topBottomLeft, topBottomRight, topLoop);

    HalfEdge* upperLeftHalfEdge = mev(topUpperLeft, Point(-3, -1, 3), downLoop);
    Vertex* downUpperLeft = upperLeftHalfEdge->endV;
    HalfEdge* upperRightHalfEdge = mev(topUpperRight, Point(3, -1, 3), downLoop);
    Vertex* downUpperRight = upperRightHalfEdge->endV;
    HalfEdge* bottomLeftHalfEdge = mev(topBottomLeft, Point(-3, -1, -3), downLoop);
    Vertex* downBottomLeft = bottomLeftHalfEdge->endV;
    HalfEdge* bottomRightHalfEdge = mev(topBottomRight, Point(3, -1, -3), downLoop);
    Vertex* downBottomRight = bottomRightHalfEdge->endV;

    Loop* upperLoop = mef(downUpperLeft, downUpperRight, downLoop);
    Loop* rightLoop = mef(downUpperRight, downBottomRight, downLoop);
    Loop* bottomLoop = mef(downBottomRight, downBottomLeft, downLoop);
    Loop* leftLoop = mef(downBottomLeft, downUpperLeft, downLoop);

    HalfEdge* topBridge = mev(topUpperLeft, Point(-1.5, 1, 1.5), topLoop);
    Vertex* topInnerUpperLeft = topBridge->endV;

    HalfEdge* topInnerUpperHalfEdge = mev(topInnerUpperLeft, Point(1.5, 1, 1.5), topLoop);
    Vertex* topInnerUpperRight = topInnerUpperHalfEdge->endV;
    HalfEdge* topInnerRightHalfEdge = mev(topInnerUpperRight, Point(1.5, 1, -1.5), topLoop);
    Vertex* topInnerBottomRight = topInnerRightHalfEdge->endV;
    HalfEdge* topInnerLeftHalfEdge = mev(topInnerUpperLeft, Point(-1.5, 1, -1.5), topLoop);
    Vertex* topInnerBottomLeft = topInnerLeftHalfEdge->endV;
    Loop* downInnerLoop = mef(topInnerBottomLeft, topInnerBottomRight, topLoop);

    Loop* topInnerLoop = kemr(topUpperLeft, topInnerUpperLeft, topLoop);

    HalfEdge* upperLeftInnerHalfEdge = mev(topInnerUpperLeft, Point(-1.5, -1, 1.5), downInnerLoop);
    Vertex* downInnerUpperLeft = upperLeftInnerHalfEdge->endV;
    HalfEdge* upperRightInnerHalfEdge = mev(topInnerUpperRight, Point(1.5, -1, 1.5), downInnerLoop);
    Vertex* downInnerUpperRight = upperRightInnerHalfEdge->endV;
    HalfEdge* bottomLeftInnerHalfEdge = mev(topInnerBottomLeft, Point(-1.5, -1, -1.5), downInnerLoop);
    Vertex* downInnerBottomLeft = bottomLeftInnerHalfEdge->endV;
    HalfEdge* bottomRightInnerHalfEdge = mev(topInnerBottomRight, Point(1.5, -1, -1.5), downInnerLoop);
    Vertex* downInnerBottomRight = bottomRightInnerHalfEdge->endV;

    Loop* upperInnerLoop = mef(downInnerUpperLeft, downInnerUpperRight, downInnerLoop);
    Loop* rightInnerLoop = mef(downInnerUpperRight, downInnerBottomRight, downInnerLoop);
    Loop* bottomInnerLoop = mef(downInnerBottomRight, downInnerBottomLeft, downInnerLoop);
    Loop* leftInnerLoop = mef(downInnerBottomLeft, downInnerUpperLeft, downInnerLoop);

    kfmrh(downLoop, downInnerLoop);

    topInnerLoop->SetInnerLoop(true);
    downInnerLoop->SetInnerLoop(true);

    return mainSolid;
}

int main()
{
    InitGLFW();
    InitController();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    GLfloat deltaTime = 0, currentTime = 0;
    GLfloat startTime = glfwGetTime();
    GLfloat lastTime = glfwGetTime();

    Solid* mainSolid = MainSolid();

    while (!glfwWindowShouldClose(window))
    {
        Solid* drawSolid = mainSolid;
        glfwPollEvents();
        GLfloat newTime = glfwGetTime();
        Controller::Movement(newTime - lastTime);
        lastTime = newTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, SCREEN_SIZE.x / SCREEN_SIZE.y, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -10.0f);

        glRotatef(Controller::zAngle, 0.0f, 1.0f, 0.0f);
        glRotatef(Controller::yAngle, 1.0f, 0.0f, 1.0f);
        glRotatef(180.0, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0, 1.0f, 0.0f, 0.0f);

        DrawSolid(drawSolid);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}