#include "pgr.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"


#define SCENE_WIDTH  1.0f
#define SCENE_HEIGHT 1.0f
#define NEAR  0.01f
#define FAR  200.0f

#define FPS 60

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 800;
const char * WIN_TITLE = "Hanzal Semestral Assignment";

float elapsedTime = 0.0f;

std::vector<Planet*> objects;
SCommonShaderProgram sc;
Camera *camera;



void InitApplication()
{
    glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    
    Transform t = {
        glm::vec3(0.0f, 0.0f, 130.0f),
        glm::vec3(0.0, 0.0f, 0.0f),
        glm::vec3(1.0, 1.0f, 1.0f),
    };
    camera = new Camera(t, WIN_WIDTH, WIN_HEIGHT, NEAR, FAR);
    camera->changeCameraType(Camera::CameraType::PERSPECTIVE);
}


void onDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //-------------------------------------------------
    ViewProjectionMatrix VP = camera->getVPMatrixes();
    for (Planet* o : objects)
    {
        o->renderObject(sc, VP.viewMatrix, VP.projectionMatrix);
    }
    //--------------------------------------------------
    glutSwapBuffers();
}

void updateWorld(float deltaTime)
{
    for (Planet* o : objects)
        o->Update(deltaTime);
}

void lateUpdateWorld(float deltaTime)
{
   //camera->changeCenterPoint(objects[0]->getPosition());
}


void onReshape(int width, int height)
{
    camera->reshapeWindow(width, height);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}
void timerCallback(int) 
{
    float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = currentTime - elapsedTime;
    elapsedTime = currentTime;
    
    
    updateWorld(deltaTime);
    //lateUpdateWorld(deltaTime);

    glutTimerFunc(1000/FPS, timerCallback, 0);
    glutPostRedisplay();
}



int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutCreateWindow(WIN_TITLE);
  

  glutDisplayFunc(onDisplay);
  glutTimerFunc(1000/FPS, timerCallback, 0);
  glutReshapeFunc(onReshape);


  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");


  initializeShaderPrograms(sc);

  const char* sphere = "Data/Sphere.obj";
  struct Transform t = { 
      glm::vec3(0.0f, 0.0f, 0.0f), 
      glm::vec3(0.0f, 0.0f, 0.0f), 
      glm::vec3(1.0f, 1.0f, 1.0f)
  };
  struct Transform t2 = {
      glm::vec3(70.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(3.0f, 3.0f, 3.0f)
  };


  Planet* obj = new Planet(t, sphere, sc, 1000.0f, nullptr);
  Planet* obj2 = new Planet(t2, sphere, sc, 100.0f, obj);
  objects.push_back(obj);
  objects.push_back(obj2);

  InitApplication();

  glutMainLoop();
  return 0;
}
