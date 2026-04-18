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
