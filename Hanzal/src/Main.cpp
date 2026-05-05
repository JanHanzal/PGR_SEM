#include "pgr.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"
#include "Configuration.h"
#include "Materials.h"
#include "Spline.h"


int WIN_WIDTH = WINDOWED_WIDTH;
int WIN_HEIGHT = WINDOWED_HEIGHT;
bool FULLSCREEN = true;


float elapsedTime = 0.0f;
bool lockedMouse = true;


std::vector<Planet*> objects;
std::vector<Fire*> fire;
Renderer renderer;
Camera* camera;
Spline* spline;

size_t follow_planet = 0;


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
    if (!objects.empty())
    {
        for (auto it = objects.begin(); it != objects.end();)
        {
            Planet* p = (Planet*)(*it);
            delete* it;
            it = objects.erase(it);
        }
    }

    

    default_planet_material.texture = sun_texture;
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


    
    Material earth_mat = default_planet_material;
    earth_mat.shininess = 8.0f;
    earth_mat.texture = earth_texture;
    Planet* earth = new Planet(
        { EARTH_POSITION, EARTH_ROTATION, EARTH_SCALE },
        Renderer::SPHERE,
        earth_mat,
        EARTH_MASS,
        500.0f,
        false,
        objects
    );
    objects.push_back(earth);



    Material saturn_mat = default_planet_material;
    saturn_mat.texture = saturn_texture;
    Planet* saturn = new Planet(
        {SATURN_POSITION, SATURN_ROTATION, SATURN_SCALE},
        Renderer::SPHERE,
        saturn_mat,
        SATURN_MASS,
        50.0f, 
        false,
        objects
    );
    objects.push_back(saturn);



    Material moon_mat = default_planet_material;
    moon_mat.texture = moon_texture;
    Planet* moon = new Planet(
        { glm::vec3(200), SATURN_ROTATION, EARTH_SCALE},
        Renderer::SPHERE,
        moon_mat,
        EARTH_MASS,
        25.0f,
        false,
        objects
    );
    objects.push_back(moon);



    Material neptune_mat = pewter;
    neptune_mat.texture = neptune_texture;
    Planet* planet2 = new Planet(
        { glm::vec3(100, 500, 200), EARTH_ROTATION, EARTH_SCALE },
        Renderer::SPHERE,
        neptune_mat,
        EARTH_MASS,
        20.0f,
        false,
        objects
    );
    objects.push_back(planet2);



    Planet* planet3 = new Planet(
        { glm::vec3(100, 100, 300), EARTH_ROTATION, EARTH_SCALE },
        Renderer::SPHERE,
        brass,
        EARTH_MASS,
        40.0f,
        false,
        objects
    );
    objects.push_back(planet3);



    Planet* planet4 = new Planet(
        { glm::vec3(400.0f, 0.0f, 0.0f), SUN_ROTATION, SATURN_SCALE },
        Renderer::SPHERE,
        default_planet_material,
        EARTH_MASS,
        50.0f,
        false,
        objects
    );
    objects.push_back(planet4);



    Planet* planet5 = new Planet(
        { glm::vec3(400.0f, 0.0f, -40.0f), SUN_ROTATION, SATURN_SCALE },
        Renderer::SPHERE,
        default_planet_material,
        SUN_MASS,
        0.0f,
        true,
        objects
    );
    objects.push_back(planet5);



    Planet* spaceship = new Planet(
        { glm::vec3(-450.0f, 0.0f, -30.0f), glm::vec3(0.0f), SATURN_SCALE},
        Renderer::GHOUL,
        pewter,
        1000.0,
        250.0f,
        false,
        objects
    );
    objects.push_back(spaceship);

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
    for (Planet* planet : objects)
    {
        if (objects[1] == planet && camera->getCameraControlType() == Camera::LOCKED)
            continue;

        glStencilFunc(GL_ALWAYS, id + 1, 0);
        planet->renderObject(VP.viewMatrix, VP.projectionMatrix);
        id++;
    }

    // HROUDA ========================================================
    {
        glm::mat4 modelMatrix = spline->alignObject(renderer.hrouda.position, renderer.hrouda.direction, WORLD_UP_VECTOR);

        bool wireframed = false;
        bool movingTexture = false;
        bool isObjectSun = false;

        glStencilFunc(GL_ALWAYS, id + 1, 0);
        renderer.renderObject(Renderer::HROUDA, brass, modelMatrix, VP.viewMatrix, VP.projectionMatrix, wireframed, movingTexture, isObjectSun);
    }
    //=================================================================
    glDisable(GL_STENCIL_TEST);
    for (Fire* f : fire)
    {
        Transform fire_t = f->getTransform();
        renderer.renderFire(fire_t.position, fire_t.scale.x, f->getStartingTime(), elapsedTime, VP.viewMatrix, VP.projectionMatrix);
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
            camera->setPosition(objects[1]->getPosition());
            camera->changeCenterPoint(objects[SUN]->getPosition());
            break;
        case Camera::SPHERICAL:
            camera->setPosition(objects[follow_planet]->getPosition() - spherePosition);
            camera->changeCenterPoint(objects[follow_planet]->getPosition());

            //camera->setPosition(renderer.hrouda.position - spherePosition);
            //camera->changeCenterPoint(renderer.hrouda.position);
            break;
        default:
            break;
    }
}

void updateWorld(float deltaTime)
{
    for (Planet* o : objects)
    {
        o->Update(deltaTime);
    }

    // SPACE HROUDA ===================================================
    //auto t = fmod(elapsedTime, 5.0) / 5.0;
    //renderer.hrouda.position = spline->get_Position_Catmull_Rom(t);
    //renderer.hrouda.direction = -spline->get_1stDerivative_Catmull_Rom(t);
    
    auto t = elapsedTime * 0.4;
    renderer.hrouda.position = spline->get_Position_Lissajous(t);
    renderer.hrouda.direction = spline->get_1stDerivative_Lissajous(t);
    
    //=================================================================
  
    lateWorldUpdate(deltaTime);
}

void timerCallback(int) 
{
   

    float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = currentTime - elapsedTime;
    elapsedTime = currentTime;
    
    // Erasing destroyed or "expired" objects
    if (!fire.empty())
    {
        for (auto it = fire.begin(); it != fire.end();)
        {
            if (elapsedTime - ((Fire*)(*it))->getStartingTime() > ANIMATION_LENGTH)
            {
                delete* it;
                it = fire.erase(it);
            }
            else 
                ++it;
        }
    }

    if (!objects.empty())
    {
        size_t i = 0;
        for (auto it = objects.begin(); it != objects.end();)
        {
            Planet* p = (Planet*)(*it);
            if (p->isPlanetDestroyed() && !p->isPlanetSun())
            {
                printf("Planet %i destroyed!\n", i);
                delete* it;
                it = objects.erase(it);
                
                if (i < follow_planet)
                {
                    follow_planet--;

                    if (follow_planet < 1)
                        follow_planet = 1;
                }
                else if (i == follow_planet)
                    follow_planet = 1;
                
            }
            else
            {
                ++it;
                ++i;
            }
        }
    }



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

void toggleFullScreen()
{
    FULLSCREEN = !FULLSCREEN;

    if (FULLSCREEN)
    {
        glutFullScreen();
    }
    else
    {
        glutLeaveFullScreen();

        glutReshapeWindow(WINDOWED_WIDTH, WINDOWED_HEIGHT);

        int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
        int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
        glutPositionWindow(
            (screenWidth - WINDOWED_WIDTH) / 2,
            (screenHeight - WINDOWED_HEIGHT) / 2
        );
    }
}


void onReshape(int width, int height)
{
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
        case 'E':
            _input[E] = true;
            break;
        case 'q':
        case 'Q':
            _input[Q] = true;
            break;
        case 'w':
        case 'W':
            _input[W] = true;
            break;
        case 's':
        case 'S':
            _input[S] = true;
            break;
        case 'a':
        case 'A':
            _input[A] = true;
            break;
        case 'd':
        case 'D':
            _input[D] = true;
            break;
        case 'r':
        case 'R':
            initScene();
            break;
        case 'f':
        case 'F':
            toggleFullScreen();
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
    if ((glutGetModifiers() & GLUT_ACTIVE_SHIFT) && (keyPressed == 'C' || keyPressed == 'c')) 
        camera->changeCameraProjectionType();
}

void keyboardUpCallback(unsigned char keyReleased, int mouseX, int mouseY) {

    switch (keyReleased) 
    {
        case 'e':
        case 'E':
            _input[E] = false;
            break;
        case 'q':
        case 'Q':
            _input[Q] = false;
            break;
        case 'w':
        case 'W':
            _input[W] = false;
            break;
        case 's':
        case 'S':
            _input[S] = false;
            break;
        case 'a':
        case 'A':
            _input[A] = false;
            break;
        case 'd':
        case 'D':
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
            {
                follow_planet++;
                if (follow_planet >= objects.size())
                {
                    follow_planet = 1;
                }
            }
            camera->changeCameraControl(Camera::SPHERICAL);
            break;
        case GLUT_KEY_F3:
            camera->changeCameraControl(Camera::FREE);
            break;
        case GLUT_KEY_F4:
            renderer.changeFogEnablement();
            break;
        case GLUT_KEY_F5:
            renderer.changeActiveLightning();
            break;
        case GLUT_KEY_F6:
            renderer.toggleDirectionalLightParams();
            break;
    }
}

void mouseWheelCallback(int wheel, int direction, int x, int y) {
    
    camera->increaseRadius(glm::sign(-direction) * 5.0f);
}

void passiveMouseMotionCallback(int mouseX, int mouseY) 
{
    if (!lockedMouse || camera->getCameraControlType() == Camera::LOCKED)
        return;

    int cx = WIN_WIDTH / 2, cy = WIN_HEIGHT / 2;
    if (mouseX == cx && mouseY == cy) 
        return;

    float deltaX = -(float)(mouseX - cx);
    float deltaY = -(float)(mouseY - cy);

    camera->rotateCamera(deltaX, deltaY);
    glutWarpPointer(cx, cy);
}

void mouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY) 
{
    if (lockedMouse)
        return;


    if ((buttonPressed == GLUT_LEFT_BUTTON) && (buttonState == GLUT_DOWN)) 
    {
        unsigned char planetId = 0;
        glReadPixels(mouseX, WIN_HEIGHT - mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &planetId);

        if (planetId == 0)
            printf("Clicked on background\n");
        
        else 
        { 
            auto it = objects.begin();
            std::advance(it, planetId - 1);
            Planet* selectedPlanet = (Planet *)(*it);
            Transform t = selectedPlanet->getTransform();

            //Calculates nearest possible position for explosion from the planet 
            glm::vec3 direction = t.position + glm::normalize(camera->getPosition() - t.position) * (t.scale.x + 1);
            t.position = direction;
            t.scale *= 2;
            fire.emplace_back(new Fire(t, elapsedTime));
        }
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

    if(FULLSCREEN)
        glutFullScreen();
    
    glutMainLoop();

  
  

    delete camera;
    delete spline;
    for (Planet* p : objects)
        delete p;
    for (Fire* f : fire)
        delete f;
    return 0;
}
