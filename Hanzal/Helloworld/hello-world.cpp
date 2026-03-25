/**
 * \file hello-world.cpp
 * \brief Your first OpenGL application.
 * \author Tomas Barak
 */

#include "pgr.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"


#define SCENE_WIDTH  1.0f
#define SCENE_HEIGHT 1.0f
#define SCENE_DEPTH  1.0f

const int WIN_WIDTH = 512;
const int WIN_HEIGHT = 512;
const char * WIN_TITLE = "Hello World";

std::vector<Object*> objects;
SCommonShaderProgram sc;
Camera camera;

void start()
{
    glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}


void onDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //-------------------------------------------------
    const glm::mat4 orthoProjectionMatrix = glm::ortho(
        -SCENE_WIDTH, SCENE_WIDTH,
        -SCENE_HEIGHT, SCENE_HEIGHT,
        -10.0f * SCENE_DEPTH, 50.0f * SCENE_DEPTH
    );
    const glm::mat4 orthoViewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 2.0f),
        glm::vec3(objects[0]->getPosition()),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // setup camera & projection transform
    glm::mat4 viewMatrix = orthoViewMatrix;
    glm::mat4 projectionMatrix = orthoProjectionMatrix;
    for (Object* o : objects)
    {
        o->renderObject(sc, viewMatrix, projectionMatrix);
    }
    //--------------------------------------------------
    glutSwapBuffers();
}


int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutCreateWindow(WIN_TITLE);

  

  glutDisplayFunc(onDisplay);

  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");


  initializeShaderPrograms(sc);

  const char* cube = "Data/ghoul.obj";
  struct Transform t = { 
      glm::vec3(0.0f, 0.0f, 0.0f), 
      glm::vec3(0.0f, 0.0f, 0.0f), 
      glm::vec3(0.5f, 0.5f, 0.5f)
  };

  Object* obj = new Object(t, cube, sc);
  objects.push_back(obj);

  start();

  glutMainLoop();
  return 0;
}
