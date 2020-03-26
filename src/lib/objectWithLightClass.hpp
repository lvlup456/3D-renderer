#ifndef objectWithLightClass_HPP
#define objectWithLightClass_HPP

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
#include "lightClass.hpp"

class ObjectWithLight : public Object{
    public:
        Light& lumiere;
        GLuint LightID;

    ObjectWithLight(const char* objPath,const char* FragmentShaderPath,const char* VertexShaderPath, Light& lum ): Object(objPath,FragmentShaderPath,VertexShaderPath), lumiere(lum){
	    LightID = glGetUniformLocation(Object::programID, "LightPosition_worldspace");
        
    }

    void draw(){
        
            glUseProgram(Object::programID);
            affectMVP();
            glm::vec3 coord = lumiere.getLightPose();
            glUniform3f(LightID, coord.x,coord.y,coord.z );
            glUniform1f(TimeID, (float) cos(glfwGetTime()));

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
            glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // 3rd attribute buffer : normals
            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // 12*3 indices starting at 0 -> 12 triangles

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);;

    }


};

#endif