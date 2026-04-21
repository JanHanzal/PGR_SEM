#include "pgr.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"
#include "Configuration.h"
#include "Materials.h"
#include "Spline.h"

int WIN_WIDTH = 1920;
int WIN_HEIGHT = 1080;
const char* WIN_TITLE = "Hanzal Semestral Assignment";

float elapsedTime = 0.0f;
bool lockedMouse = true;

std::vector<Planet*> objects;
Renderer renderer;
Camera* camera;
Spline* spline;

PlanetsOrder follow_planet = PlanetsOrder::SUN;

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
    initMaterials();
    renderer.Init();
    
    for (bool& button : _input)
        button = false;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    
    spline = new Spline(glm::vec3(150.0, 0.0, -50.0), glm::vec3(0.0, 150.0, 50.0), glm::vec3(-150.0, 0.0, -50.0), glm::vec3(0.0, -150.0, 50.0));
    
    camera = new Camera({CAMERA_START_POSITION, CAMERA_START_ROTATION, glm::vec3(1.0)}, WIN_WIDTH, WIN_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
}

void initScene()
{
    Planet* sun = new Planet(
        {SUN_POSITION, SUN_ROTATION, SUN_SCALE},
        Renderer::SPHERE,
        default_planet_material,
        SUN_MASS,
        0.0f,
        true,
        objects
    );
    objects.push_back(sun);


    default_planet_material.shininess = 8.0f;
    default_planet_material.texture = earth_texture;
    
    Planet* earth = new Planet(
        { EARTH_POSITION, EARTH_ROTATION, EARTH_SCALE },
        Renderer::SPHERE,
        default_planet_material,
        EARTH_MASS,
        500.0f,
        false,
        objects
    );
    objects.push_back(earth);

    default_planet_material.texture = saturn_texture;
    Planet* saturn = new Planet(
        {SATURN_POSITION, SATURN_ROTATION, SATURN_SCALE},
        Renderer::SPHERE,
        default_planet_material,
        SATURN_MASS,
        50.0f, 
        false,
        objects
    );
    objects.push_back(saturn);
}


void onDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    ViewProjectionMatrix VP = camera->getVPMatrixes();

    renderer.renderSkybox(VP.viewMatrix, VP.projectionMatrix);

    //Dynamic spotlight position ======================================
    {
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
    }
    //=================================================================
    size_t id = 0;
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for (Planet* o : objects)
    {
        if (objects[EARTH] == o && camera->getCameraControlType() == Camera::LOCKED)
            continue;

        glStencilFunc(GL_ALWAYS, id + 1, 0);
        o->renderObject(VP.viewMatrix, VP.projectionMatrix);
        id++;
    }

    // SPACE MONKEY ===================================================
    {
        glm::mat4 modelMatrix = spline->alignObject(renderer.spaceMonkey.position, renderer.spaceMonkey.direction, WORLD_UP_VECTOR);

        bool wireframed = false;
        bool movingTexture = false;
        bool isObjectSun = false;

        glStencilFunc(GL_ALWAYS, id + 1, 0);
        renderer.renderObject(Renderer::MONKEY, pewter, modelMatrix, VP.viewMatrix, VP.projectionMatrix, wireframed, movingTexture, isObjectSun);
    }
    //=================================================================
    glDisable(GL_STENCIL_TEST);
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
            camera->setPosition(objects[EARTH]->getPosition());
            camera->changeCenterPoint(objects[SUN]->getPosition());
            break;
        case Camera::SPHERICAL:
            //camera->setPosition(objects[follow_planet]->getPosition() - spherePosition);
            camera->setPosition(renderer.spaceMonkey.position - spherePosition);
            camera->changeCenterPoint(renderer.spaceMonkey.position);
            break;
        default:
            break;
    }
}

void updateWorld(float deltaTime)
{
    for (Planet* o : objects)
        o->Update(deltaTime);

    // SPACE MONKEY ===================================================
    auto t = fmod(elapsedTime, 5.0) / 5.0;
    renderer.spaceMonkey.position = spline->get_Position_Catmull_Rom(t);
    renderer.spaceMonkey.direction = -spline->get_1stDerivative_Catmull_Rom(t);
    //=================================================================

    lateWorldUpdate(deltaTime);
}

void timerCallback(int) 
{
    float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = currentTime - elapsedTime;
    elapsedTime = currentTime;

    updateWorld(deltaTime);
    
    //==============================================
    //Camera Input==================================
    glm::vec3 cameraMotion = glm::vec3(0.0f);

    if (_input[W]) cameraMotion += glm::vec3(1.0f, 0.0f, 0.0f);
    if (_input[S]) cameraMotion += glm::vec3(-1.0f, 0.0f, 0.0f);
    if (_input[D]) cameraMotion += glm::vec3(0.0f, 1.0f, 0.0f);
    if (_input[A]) cameraMotion += glm::vec3(0.0f, -1.0f, 0.0f);
    
    //Accent
    if (_input[E]) cameraMotion += glm::vec3(0.0f, 0.0f, 1.0f);
    //Deccent
    if (_input[Q]) cameraMotion += glm::vec3(0.0f, 0.0f, -1.0f);

    
    camera->Move(cameraMotion , deltaTime);
    camera->Update(deltaTime);
    //==============================================
    //==============================================


    glutTimerFunc(1000/FPS, timerCallback, 0);
    glutPostRedisplay();
}



void onReshape(int width, int height)
{
    glutReshapeWindow(width, height);
    camera->reshapeWindow(width, height);
    WIN_WIDTH = width;
    WIN_HEIGHT = height;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
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
    case ' ':
        lockedMouse = !lockedMouse;
        if (lockedMouse)
            glutSetCursor(GLUT_CURSOR_NONE);
        else
            glutSetCursor(GLUT_CURSOR_INHERIT);

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
        ;
    }
}

void keyboardSpecialCallback(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            camera->changeCameraControl(Camera::LOCKED);
            break;
        case GLUT_KEY_F2:
            if (camera->getCameraControlType() == Camera::SPHERICAL)
                follow_planet = static_cast<PlanetsOrder>((follow_planet + 1) % PLANETS_LENGTH);
            camera->changeCameraControl(Camera::SPHERICAL);
            break;
        case GLUT_KEY_F3:
            camera->changeCameraControl(Camera::FREE);
            break;
        case GLUT_KEY_F4:
            renderer.changeActiveLightning();
            break;
        case GLUT_KEY_F5:
            renderer.changeFogEnablement();
            break;
    }
}

void mouseWheelCallback(int wheel, int direction, int x, int y) {
    
    camera->increaseRadius(glm::sign(-direction) * 5.0f);
}

void passiveMouseMotionCallback(int mouseX, int mouseY) 
{
    if (!lockedMouse)
        return;

    int cx = WIN_WIDTH / 2, cy = WIN_HEIGHT / 2;
    if (mouseX == cx && mouseY == cy) 
        return;

    float deltaX = -(float)(mouseX - cx);
    float deltaY = -(float)(mouseY - cy);

    camera->rotateCamera(deltaX, deltaY);
    glutWarpPointer(cx, cy);
    glutPostRedisplay();
}

void mouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY) 
{
    if ((buttonPressed == GLUT_LEFT_BUTTON) && (buttonState == GLUT_DOWN)) 
    {
        unsigned char asteroidID = 0;
        glReadPixels(mouseX, WIN_HEIGHT - mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &asteroidID);

        if (asteroidID == 0) 
            printf("Clicked on background\n");
        
        else 
            printf("Clicked on object with ID: %d\n", (int)asteroidID);
    }
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
    glutTimerFunc(1000/FPS, timerCallback, 0);
    glutReshapeFunc(onReshape);
    glutKeyboardFunc(keyboardCallback);
    glutKeyboardUpFunc(keyboardUpCallback);
    glutSpecialFunc(keyboardSpecialCallback);
    glutPassiveMotionFunc(passiveMouseMotionCallback);
    glutMouseWheelFunc(mouseWheelCallback);
    glutWarpPointer(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    glutMouseFunc(mouseCallback);
    //======================================
    glutSetCursor(GLUT_CURSOR_NONE);

    if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
        pgr::dieWithError("Pgr init failed, required OpenGL not supported?");

    initApplication();
    initScene();
    glutFullScreen();
    

    glutMainLoop();

  
  

    delete camera;
    delete spline;
    for (Planet* p : objects)
        delete p;
    return 0;
}
