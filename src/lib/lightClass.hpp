#ifndef lightClass_HPP
#define lightClass_HPP

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

using namespace glm;

#include "objectClass.hpp"


class Light : public Object{
    public:
        glm::vec3 lightPose;


    Light(glm::vec3 pose,const char* objPath,const char* FragmentShaderPath,const char* VertexShaderPath): 
        Object(objPath,FragmentShaderPath,VertexShaderPath), 
        lightPose(pose){
    	    Object::translate(lightPose);
        }
   

    void translate(glm::vec3 vect){
        lightPose += vect;
        Object::translate(vect);
    }

    glm::vec3 getLightPose(){
        return lightPose;
    }

/*     ~Light(){

        glDeleteProgram(programID);
    } */

};

#endif