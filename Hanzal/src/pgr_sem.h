#include "pgr.h"

#ifndef __PGR_SEM_H
#define __PGR_SEM_H

#define SCENE_WIDTH 1.0f
#define SCENE_HEIGHT 1.0f


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
        glm::vec3 upVector = { 1.0f, 0.0f, 0.0f };


        void updateCameraMatrixes()
        {
            if (cameraType == ORTHO)
            {
                vpMatrix.projectionMatrix = glm::ortho(
                    -SCENE_WIDTH*aspect, SCENE_WIDTH*aspect,
                    -SCENE_HEIGHT*aspect, SCENE_HEIGHT*aspect,
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
        
    public:
        Camera(Transform _transform, float _width, float _heigth, float _near, float _far) : 
        transform(_transform), width(_width), heigth(_heigth), nearPlane(_near), farPlane(_far)
        {
            aspect = width / heigth;
        }

        void reshapeWindow(float _width, float _height)
        {
            width = _width;
            heigth = _height;
            aspect = width / heigth;

            updateCameraMatrixes();
        }

        void changeCameraType(CameraType _cameraType)
        {
            if (cameraType == _cameraType)
                return;
            
            cameraType = _cameraType;
            updateCameraMatrixes();
        }
    
        void changeCenterPoint(glm::vec3 newCenter)
        {
            center = newCenter;
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