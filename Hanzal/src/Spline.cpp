#include "Spline.h"

Spline::Spline(glm::vec3 _P0, glm::vec3 _P1, glm::vec3 _P2, glm::vec3 _P3) : P0(_P0), P1(_P1), P2(_P2), P3(_P3) {};


glm::vec3 Spline::get_Position_Catmull_Rom(float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

	glm::vec3 result;


	result = 0.5f * (
        (2.0f * P1) +
        (-P0 + P2) * t +
        (2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3) * t2 +
        (-P0 + 3.0f * P1 - 3.0f * P2 + P3) * t3
        );
    return result;
}

glm::vec3 Spline::get_1stDerivative_Catmull_Rom(const float t) {
    glm::vec3 result(1.0, 0.0, 0.0);

    float t2 = t * t;

    result = 0.5f * (
        (-P0 + P2) +
        (2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3) * (2.0f * t) +
        (-P0 + 3.0f * P1 - 3.0f * P2 + P3) * (3.0f * t2)
        );

    return result;
}

glm::mat4 Spline::alignObject(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {

    glm::vec3 z = -glm::normalize(front);

    if (z.length == 0)
        z = glm::vec3(0.0, 0.0, 1.0);

    glm::vec3 x = glm::normalize(glm::cross(up, z));

    if (x.length == 0)
        x = glm::vec3(1.0, 0.0, 0.0);

    glm::vec3 y = glm::cross(z, x);
    //mat4 matrix = mat4(1.0f);
    glm::mat4 matrix = glm::mat4(
        x.x, x.y, x.z, 0.0,
        y.x, y.y, y.z, 0.0,
        z.x, z.y, z.z, 0.0,
        position.x, position.y, position.z, 1.0
    );

    return matrix;
}

glm::vec3 Spline::get_Position_Lissajous(float t)
{
    float A = 150.0f;

    float a = 1.0f;
    float b = 2.0f;
    float c = 3.0f;

    return glm::vec3(
        A * std::sin(a * t),
        A * std::sin(b * t),
        A * std::cos(c * t)
    );
}

glm::vec3 Spline::get_1stDerivative_Lissajous(float t)
{
    float A = 150.0f;
    float a = 1.0f;
    float b = 2.0f;
    float c = 3.0f;

    return glm::vec3(
        A * a * std::cos(a * t),
        A * b * std::cos(b * t),
        -A * c * std::sin(c * t)
    );
}