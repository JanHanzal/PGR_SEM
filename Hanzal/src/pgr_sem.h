#include "pgr.h"

#ifndef __PGR_SEM_H
#define __PGR_SEM_H

#define WORLD_UP_VECTOR glm::vec3(0.0f, 0.0f, 1.0f)

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


        void rotateCamera(float yaw, float pitch)
        {
            transform.rotation.x += yaw;
            transform.rotation.z += pitch;

            glm::vec3 rotationAxis = glm::cross(upVector, (transform.position - center));
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), rotationAxis);
            
            center = transform.position + glm::vec3(rotationMatrix * glm::vec4(center - transform.position, 0.0f));
            upVector = glm::vec3(rotationMatrix * glm::vec4(upVector, 0.0f));
        }

        void Move(glm::vec3 motion)
        {
            transform.position += motion;
        }

        void Update(float timeDelta)
        {
            glm::vec3 motion = glm::normalize(center - transform.position) * timeDelta;
            Move(motion);
            updateCameraMatrixes();
        }


        ViewProjectionMatrix getVPMatrixes()
        {
            updateCameraMatrixes();
            return vpMatrix;
        }
};

#endif