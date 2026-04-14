#include "pgr.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"


#define SCENE_WIDTH  1.0f
#define SCENE_HEIGHT 1.0f
#define NEAR  0.01f
#define FAR  2000.0f

#define FPS 60

int WIN_WIDTH = 1920;
int WIN_HEIGHT = 1080;
const char * WIN_TITLE = "Hanzal Semestral Assignment";

float elapsedTime = 0.0f;

std::vector<Planet*> objects;
Renderer renderer;
Camera* camera;


enum Controls
{
    W,
    S,
    A,
    D,
    E,
    Q,
    CONTROLS_LENGTH
};

bool _input[CONTROLS_LENGTH];

void initApplication()
{
    renderer.Init();

    for (bool& b : _input)
        b = false;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    
    Transform t = {
        glm::vec3(100.0f, 0.0f, 20.0f),
        glm::vec3(0.0, 0.0f, 0.0f),
        glm::vec3(1.0, 1.0f, 1.0f),
    };
    camera = new Camera(t, WIN_WIDTH, WIN_HEIGHT, NEAR, FAR);
    camera->changeCameraProjectionType(Camera::ProjectionType::PERSPECTIVE);
}

void sceneInit()
{
    GLuint earth_texture = pgr::createTexture("Data/Earth.jpg");
    GLuint sun_texture = pgr::createTexture("Data/Sun.jpg");
    Material mat = {
       glm::vec3(1.0f, 1.0f, 1.0f),  // Ambient
       glm::vec3(1.0f, 1.0f, 1.0f),  // Diffuse
       glm::vec3(1.0f),              // Specular
       0.0f,                         // Shininess
       sun_texture
    };

    struct Transform t = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(90.0f, 0.0f, 12.0f),
        glm::vec3(10.0f, 10.0f, 10.0f)
    };

    struct Transform t2 = {
        glm::vec3(100.0f, 0.0f, 0.0f),
        glm::vec3(90.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

    struct Transform t3 = {
        glm::vec3(200.0f, 0.0f, 0.0f),
        glm::vec3(90.0f, 0.0f, 0.0f),
        glm::vec3(10.0f, 10.0f, 10.0f)
    };

    //Sun  =================================================================
    Planet* p1 = new Planet(t,Renderer::SPHERE, mat, 10000.0, 0.0f, objects);
    objects.push_back(p1);
    //======================================================================

    mat.shininess = 8.0f;
    mat.texture = earth_texture;
    Planet* p2 = new Planet(t2, Renderer::SPHERE, mat, 100.0f, 20.0f, objects);
    objects.push_back(p2);
    Planet* p3 = new Planet(t3, Renderer::SPHERE, mat, 10000.0, 20.0f, objects);
    objects.push_back(p3);
}


void onDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ViewProjectionMatrix VP = camera->getVPMatrixes();
    
    //==================================
    SpotLight sl = {
        camera->getPosition(),
        camera->getFrontVector(),

        glm::vec3(0.1f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.5f),

        10.0f,
        1.0f
    };
    renderer.changeSpotLightParams(sl);
    //==================================

    for (Planet* o : objects)
    {
        if (objects[1] == o && camera->getCameraControlType() == Camera::LOCKED)
            continue;

        o->renderObject(VP.viewMatrix, VP.projectionMatrix);
    }
    
    glutSwapBuffers();
}

void lateWorldUpdate(float deltaTime)
{
    glm::vec3 spherePosition = camera->getRadius() * camera->getSpherePosition();
    switch (camera->getCameraControlType())
    {
        case Camera::FREE:
            break;
        case Camera::LOCKED:
            camera->setPosition(objects[1]->getPosition() + glm::vec3(0.0, 0.0, 0.0));
            break;
        case Camera::SPHERICAL:
            camera->setPosition(objects[0]->getPosition() - spherePosition);
            break;
        default:
            break;
    }
}

void updateWorld(float deltaTime)
{
    for (Planet* o : objects)
        o->Update(deltaTime);

    lateWorldUpdate(deltaTime);
}




void onReshape(int width, int height)
{
    glutReshapeWindow(width, height);
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
    
    //==============================================
    //Camera Input==================================
    glm::vec3 cameraMotion = glm::vec3(0.0f);

    if (_input[W])
        cameraMotion += glm::vec3(1.0f, 0.0f, 0.0f);
    if (_input[S])
        cameraMotion += glm::vec3(-1.0f, 0.0f, 0.0f);
    if (_input[D])
        cameraMotion += glm::vec3(0.0f, 1.0f, 0.0f);
    if (_input[A])
        cameraMotion += glm::vec3(0.0f, -1.0f, 0.0f);
    
    //Accent
    if (_input[E])
        cameraMotion += glm::vec3(0.0f, 0.0f, 1.0f);
    //Deccent
    if (_input[Q])
        cameraMotion += glm::vec3(0.0f, 0.0f, -1.0f);

    camera->Move(cameraMotion , deltaTime);
    camera->Update(deltaTime);
    //==============================================
    //==============================================


    
    
    updateWorld(deltaTime);
    //lateWorldUpdate(deltaTime);

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
    case 'e':
        _input[E] = true;
        break;
    case 'q':
        _input[Q] = true;
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
    
    case 'e':
        _input[E] = false;
        break;
    case 'q':
        _input[Q] = false;
        break;
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

void keyboardSpecialCallback(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            camera->changeCameraControl(Camera::LOCKED);
            camera->changeCenterPoint(objects[0]->getPosition());
            break;
        case GLUT_KEY_F2:
            camera->changeCameraControl(Camera::SPHERICAL);
            camera->changeCenterPoint(objects[0]->getPosition());
            break;
        case GLUT_KEY_F3:
            camera->changeCameraControl(Camera::FREE);
            break;
        case GLUT_KEY_F4:
            renderer.changeActiveLightning();
            break;
    }
}

void mouseWheelCallback(int wheel, int direction, int x, int y) {
    
    camera->increaseRadius(glm::sign(-direction) * 5.0f);
}

void passiveMouseMotionCallback(int mouseX, int mouseY) 
{
    int cx = WIN_WIDTH / 2, cy = WIN_HEIGHT / 2;
    if (mouseX == cx && mouseY == cy) 
        return;

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

    //======================================
    //Callbacks-----------------------------
    glutDisplayFunc(onDisplay);
    glutTimerFunc(33, timerCallback, 0);
    glutReshapeFunc(onReshape);
    glutKeyboardFunc(keyboardCallback);
    glutKeyboardUpFunc(keyboardUpCallback);
    glutSpecialFunc(keyboardSpecialCallback);
    glutPassiveMotionFunc(passiveMouseMotionCallback);
    glutMouseWheelFunc(mouseWheelCallback);
    glutWarpPointer(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    //======================================
    glutSetCursor(GLUT_CURSOR_NONE);

    if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
        pgr::dieWithError("Pgr init failed, required OpenGL not supported?");

    sceneInit();
    initApplication();
    glutFullScreen();
    

    glutMainLoop();

  
  

    delete camera;
    for (Planet* p : objects)
        delete p;
    return 0;
}
