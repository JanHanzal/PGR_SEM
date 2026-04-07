#include "pgr.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"


#define SCENE_WIDTH  1.0f
#define SCENE_HEIGHT 1.0f
#define NEAR  0.01f
#define FAR  200.0f

#define FPS 60

int WIN_WIDTH = 1920;
int WIN_HEIGHT = 1080;
const char * WIN_TITLE = "Hanzal Semestral Assignment";

float elapsedTime = 0.0f;

std::vector<Planet*> objects;
SCommonShaderProgram sc;
Camera *camera;

enum Controls
{
    W,
    S,
    A,
    D,
    CONTROLS_LENGTH
};

bool _input[CONTROLS_LENGTH];

void InitApplication()
{
    for (bool& b : _input)
        b = false;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    
    Transform t = {
        glm::vec3(0.0f, -40.0f, 0.0f),
        glm::vec3(90.0, 0.0f, 0.0f),
        glm::vec3(1.0, 1.0f, 1.0f),
    };
    camera = new Camera(t, WIN_WIDTH, WIN_HEIGHT, NEAR, FAR);
    camera->changeCameraType(Camera::CameraType::PERSPECTIVE);
}


void onDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ViewProjectionMatrix VP = camera->getVPMatrixes();
    for (Planet* o : objects)
    {
        o->renderObject(sc, VP.viewMatrix, VP.projectionMatrix);
    }
    
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
    WIN_WIDTH = width;
    WIN_HEIGHT = height;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void timerCallback(int) 
{
    float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = currentTime - elapsedTime;
    elapsedTime = currentTime;
    
    glm::vec2 cameraMotion = glm::vec2(0.0f);

    if (_input[W])
        cameraMotion += glm::vec2(1.0f, 0.0f);
    if (_input[S])
        cameraMotion += glm::vec2(-1.0f, 0.0f);
    if (_input[D])
        cameraMotion += glm::vec2(0.0f, 1.0f);
    if (_input[A])
        cameraMotion += glm::vec2(0.0f, -1.0f);

    camera->Move(cameraMotion , deltaTime);
    camera->Update(deltaTime);


    
    
    updateWorld(deltaTime);
    //lateUpdateWorld(deltaTime);

    glutTimerFunc(1000/FPS, timerCallback, 0);
    glutPostRedisplay();
}


void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY)
{
    switch (keyPressed)
    {
    case 27:
        glutLeaveMainLoop();
        break;

    case 'c':
        camera->changeCameraType();
        break;
    case 'w':
        _input[W] = true;
        break;
    case 's':
        _input[S] = true;
        break;
    case 'a':
        _input[A] = true;
        break;
    case 'd':
        _input[D] = true;
        break;
    default:
        break;
    }
}

void keyboardUpCallback(unsigned char keyReleased, int mouseX, int mouseY) {

    switch (keyReleased) {
    case 'w':
        _input[W] = false;
        break;
    case 's':
        _input[S] = false;
        break;
    case 'a':
        _input[A] = false;
        break;
    case 'd':
        _input[D] = false;
        break;
    default:
        ; // printf("Unrecognized key released\n");
    }
}

void passiveMouseMotionCallback(int mouseX, int mouseY) 
{
    int cx = WIN_WIDTH / 2, cy = WIN_HEIGHT / 2;
    if (mouseX == cx && mouseY == cy) return; // ignoruj warp event

    float deltaX = -(float)(mouseX - cx);
    float deltaY = -(float)(mouseY - cy);

    camera->rotateCamera(deltaX, deltaY);
    glutWarpPointer(cx, cy);
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
  glutKeyboardFunc(keyboardCallback);
  glutKeyboardUpFunc(keyboardUpCallback);
  glutPassiveMotionFunc(passiveMouseMotionCallback);
  glutWarpPointer(WIN_WIDTH / 2, WIN_HEIGHT / 2);

  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");

  initializeShaderPrograms(sc);

  //======================================
  //Init scene----------------------------
  const char* sphere = "Data/Sphere.obj";
  const char* earth = "Data/Earth.obj";
  struct Transform t = { 
      glm::vec3(0.0f, 0.0f, 0.0f), 
      glm::vec3(90.0f, 0.0f, 24.0f), 
      glm::vec3(2.0f, 2.0f, 2.0f)
  };
  struct Transform t2 = {
      glm::vec3(50.0f, 0.0f, 2.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.5f, 0.5f, 0.5f)
  };

  struct Transform t3 = {
      glm::vec3(-100.0f, 0.0f, -5.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.5f, 0.5f, 0.5f)
  };

  Planet* obj = new Planet(t, earth, sc, 1000.0f, nullptr);
  Planet* obj2 = new Planet(t2, earth, sc, 100.0f, obj);
  Planet* obj3 = new Planet(t3, earth, sc, 1000.0f, obj);

  objects.push_back(obj);
  objects.push_back(obj2);
  objects.push_back(obj3);
  //Init scene----------------------------
  //======================================

  InitApplication();

  glutMainLoop();
  return 0;
}
