/**
 * \file hello-world.cpp
 * \brief Your first OpenGL application.
 * \author Tomas Barak
 */

#include <iostream>
#include "pgr.h"
#include "Object.h"

const int WIN_WIDTH = 512;
const int WIN_HEIGHT = 512;
const char * WIN_TITLE = "Hello World";

GLuint shaderProgram = 0;
GLuint shaderProgram2 = 0;
GLuint shader3 = 0;
GLuint arrayBuffer = 0;
GLuint vao = 0;
GLuint ebo = 0;

std::string vertexShaderSrc =
"#version 140\n"
"in vec3 position;\n" // Zmìna z vec2 na vec3
"void main() {\n"
"  gl_Position = vec4(position, 1.0f);\n" // position už má v sobì X, Y i Z
"}\n"
;


std::string fragmentShaderSrc =
    "#version 140\n"
    "out vec4 color;"
    "void main() {\n"
    "  color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n"
    ;
std::string fragmentShaderSrc2 =
"#version 140\n"
"out vec4 color;"
"void main() {\n"
"  color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n"
;
std::string fragmentShaderSrc3 =
"#version 140\n"
"out vec4 color;"
"void main() {\n"
"  color = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n"
;
void init() {
    glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    GLuint shaders[] = {
      pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
      pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),
      0
    };
    GLuint shaders2[] = {
      pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
      pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc2),
      0
    };
    GLuint shaders3[]
    {
        pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
        pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc3)
    };
    shader3 = pgr::createProgram(shaders3);
    shaderProgram = pgr::createProgram(shaders);
    shaderProgram2 = pgr::createProgram(shaders2);
    static const float vertices[] = {
        // --- 1. Rovina (Bílý trojúhelník) - Rovnobìžná s obrazovkou ---
        // Všechny vrcholy mají Z = 0.0f, takže rovina leží naplocho "uprostøed" hloubky
        -0.7f, -0.5f,  0.0f,  // 0 - Vlevo dole
         0.7f, -0.5f,  0.0f,  // 1 - Vpravo dole
         0.0f,  1.0f,  0.0f,  // 2 - Nahoøe uprostøed

         // --- 2. Rovina (Èervený trojúhelník) - Naklonìná ---
         // Dole má Z = 0.6f (blíž k tobì), nahoøe má Z = -0.6f (dál od tebe)
         -0.5f, -0.7f,  0.6f,  // 3 - Vlevo dole (vystupuje z monitoru)
          0.5f, -0.7f,  0.6f,  // 4 - Vpravo dole (vystupuje z monitoru)
          0.0f,  0.8f, -0.1f   // 5 - Nahoøe uprostøed (zapadá do monitoru)
    };

    glGenBuffers(1, &arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    static const unsigned short faces[] = {
      0, 1, 2, // První trojúhelník
      3, 4, 5,  // Druhý trojúhelník
      0,2,3
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

  GLint positionLoc = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(positionLoc);
  glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);
  glBindVertexArray(vao);
  //glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_SHORT, 0);
  glUseProgram(shaderProgram2);
  glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_SHORT, (void*)(3*sizeof(unsigned short)));
  glUseProgram(shader3);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (void*)(6 * sizeof(unsigned short)));

  glutSwapBuffers();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutCreateWindow(WIN_TITLE);

  glutDisplayFunc(draw);

  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");

  init();

  std::cout << "Hello triangle!" << std::endl;

  glutMainLoop();
  return 0;
}
