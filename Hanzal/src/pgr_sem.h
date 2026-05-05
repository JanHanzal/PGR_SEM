#ifndef __PGR_SEM_H
#define __PGR_SEM_H

#include "pgr.h"
#include "Renderer.h"

extern Renderer renderer;

#define WORLD_UP_VECTOR glm::vec3(0.0f, 0.0f, 1.0f)
#define CAMERA_ELEVATION_MAX 80
#define RADIUS_MIN 20.0
#define RADIUS_MAX 100.0

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
        enum ProjectionType { ORTHO, PERSPECTIVE };
        enum CameraControlType { FREE, LOCKED, SPHERICAL };

        float sensitivity = 0.05f;
        float speed = 10.0f;
        
    private:
        Transform transform;
        float width, heigth;
        float FOV = 60.0f;
        float aspect;
        float nearPlane = 0.1f, farPlane = 50.0f;
        
        float radius = RADIUS_MIN;
        

        Camera::ProjectionType projectionType = PERSPECTIVE;
        Camera::CameraControlType cameraControl = FREE;


        ViewProjectionMatrix vpMatrix;
        
        glm::vec3 center = { 0.0f, 0.0f, 0.0f };
        glm::vec3 upVector = WORLD_UP_VECTOR;


        void updateCameraMatrixes()
        {
            if (projectionType == ProjectionType::ORTHO)
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
            updateCameraMatrixes();
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

        void Move(glm::vec3 motion, float deltaTime)
        {
            if (cameraControl != FREE)
                return;

            glm::vec3 direction = glm::normalize(center - transform.position);
            glm::vec3 right = glm::normalize(glm::cross(direction, upVector));
            glm::vec3 _motion = (direction * motion.x) + (right * motion.y);

            transform.position += _motion * deltaTime * speed;
            transform.position += WORLD_UP_VECTOR * motion.z * deltaTime * speed;
        }

        void Update(float timeDelta)
        {   
            if (cameraControl == CameraControlType::FREE)
            {
                //Rotation.x = phi, Rotation.z = theta
                glm::vec3 front = getSpherePosition();
                center = transform.position + glm::normalize(front);
            }
            
            changeVectors();

            updateCameraMatrixes();
        }


        void reshapeWindow(float _width, float _height)
        {
            width = _width;
            heigth = _height;
            aspect = width / heigth;
        }

        
        void setPosition(glm::vec3 position)
        {
            transform.position = position;
        }

        glm::vec3 getPosition()
        {
            return transform.position;
        }

        glm::vec3 getFrontVector()
        {
            return (center - transform.position);
        }
        
        void changeCameraProjectionType()
        {
            projectionType = static_cast<ProjectionType>((projectionType + 1) % 2);
        }

        void changeCameraProjectionType(ProjectionType _projectionType)
        {         
            projectionType = _projectionType;
        }
    
        void changeCenterPoint(glm::vec3 newCenter)
        {
            center = newCenter;
            upVector = glm::normalize(glm::cross(center - transform.position, glm::vec3(0.0f, 1.0f, 0.0f)));
        }


        void changeCameraControl(CameraControlType _cameraControl)
        {
            cameraControl = _cameraControl;   
        }

        CameraControlType getCameraControlType()
        {
            return cameraControl;
        }
        


        void increaseRadius(float amount)
        {
            if (radius + amount < RADIUS_MAX && radius + amount >= RADIUS_MIN)
            { 
                radius += amount;
            }
           
        }
        
        float getRadius()
        {
            return radius;
        }


        glm::vec3 getSpherePosition()
        {
            glm::vec3 spherePosition = glm::vec3(
                cos(glm::radians(PHI)) * cos(glm::radians(THETA)),
                sin(glm::radians(PHI)) * cos(glm::radians(THETA)),
                sin(glm::radians(THETA))
            );

            return spherePosition;
        }

        ViewProjectionMatrix getVPMatrixes()
        {
            return vpMatrix;
        }
};

#endif