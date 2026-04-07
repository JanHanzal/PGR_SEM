#include "pgr.h"

#ifndef __PGR_SEM_H
#define __PGR_SEM_H

#define WORLD_UP_VECTOR glm::vec3(0.0f, 0.0f, 1.0f)
#define CAMERA_ELEVATION_MAX 45

#define PHI transform.rotation.x
#define THETA transform.rotation.z


struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct ViewProjectionMatrix
{
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};


class Camera
{
    public:
        enum CameraType { ORTHO, PERSPECTIVE };
        float sensitivity = 0.05f;
        float speed = 10.0f;
        
    private:
        Transform transform;
        float width, heigth;
        float FOV = 60.0f;
        float aspect;
        float nearPlane = 0.1f, farPlane = 50.0f;

        Camera::CameraType cameraType = ORTHO;
        ViewProjectionMatrix vpMatrix;
        
        glm::vec3 center = { 0.0f, 0.0f, 0.0f };
        glm::vec3 upVector = { 0.0f, 0.0f, 1.0f };


        void updateCameraMatrixes()
        {
            if (cameraType == ORTHO)
            {
                vpMatrix.projectionMatrix = glm::ortho(
                    -width/2, width/2,
                    -heigth/2, heigth/2,
                    nearPlane, farPlane
                );
            }
            else
            {
                vpMatrix.projectionMatrix = glm::perspective(glm::radians(FOV), aspect, nearPlane, farPlane);
            }

            vpMatrix.viewMatrix = glm::lookAt(
                transform.position,
                center,
                upVector
            );
        }
        
        void changeVectors()
        {
            const glm::vec3 forwardVector = glm::normalize(center - transform.position);

            glm::vec3 right = glm::normalize(glm::cross(forwardVector, WORLD_UP_VECTOR));
            upVector = glm::normalize(glm::cross(right, forwardVector));
        }

    public:
        Camera(Transform _transform, float _width, float _heigth, float _near, float _far) : 
        transform(_transform), width(_width), heigth(_heigth), nearPlane(_near), farPlane(_far)
        {
            aspect = width / heigth;
            changeVectors();
        }

        void reshapeWindow(float _width, float _height)
        {
            width = _width;
            heigth = _height;
            aspect = width / heigth;
        }

        void changeCameraType()
        {
            cameraType = static_cast<Camera::CameraType>((cameraType + 1) % 2);
        }

        void changeCameraType(CameraType _cameraType)
        {
            if (cameraType == _cameraType)
                return;
            
            cameraType = _cameraType;
        }
    
        void changeCenterPoint(glm::vec3 newCenter)
        {
            center = newCenter;
            upVector = glm::normalize(glm::cross(center - transform.position, glm::vec3(0.0f, 1.0f, 0.0f)));
        }


        void rotateCamera(const float yaw, const float pitch)
        {
            PHI += sensitivity * yaw;
            
            if (PHI > 360.0f)
                PHI -= 360.0f;
            else if (PHI < 0.0f)
                PHI += 360.0f;

            const float theta_delta = sensitivity * pitch;
            if(fabs(transform.rotation.z + theta_delta) < CAMERA_ELEVATION_MAX)
                THETA += theta_delta;
        }

        void Move(glm::vec2 motion, float deltaTime)
        {
            glm::vec3 direction = glm::normalize(center - transform.position);
            glm::vec3 right = glm::normalize(glm::cross(direction, upVector));
            glm::vec3 _motion = (direction * motion.x) + (right * motion.y);

            transform.position += _motion * deltaTime * speed;
        }

        void Update(float timeDelta)
        {   
            //Rotation.x = phi, Rotation.y = theta
            glm::vec3 front = 
            { 
                cos(glm::radians(PHI)) * cos(glm::radians(THETA)), 
                sin(glm::radians(PHI)) * cos(glm::radians(THETA)),
                sin(glm::radians(THETA)) 
            };

            center = transform.position + glm::normalize(front);
            changeVectors();

            updateCameraMatrixes();
        }


        ViewProjectionMatrix getVPMatrixes()
        {
            return vpMatrix;
        }
};

#endif