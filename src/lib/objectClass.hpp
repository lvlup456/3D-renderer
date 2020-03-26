#ifndef objectClass_HPP
#define objectClass_HPP

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
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

using namespace glm;

#include "objloader.hpp"


class Object{
    public:
        GLuint colorbuffer;
        GLuint normalsbuffer;
        GLuint vertexbuffer;

        GLuint programID;
        GLuint MatrixID;
        GLuint ViewMatrixID;
        GLuint ModelMatrixID;
        GLuint TimeID;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        glm::mat4 ModelMatrix;


        Object(const char* objPath,const char* FragmentShaderPath,const char* VertexShaderPath){
	        programID = LoadShaders( VertexShaderPath, FragmentShaderPath );           
            MatrixID = glGetUniformLocation(programID, "MVP");
	        ViewMatrixID = glGetUniformLocation(programID, "V");
	        ModelMatrixID = glGetUniformLocation(programID, "M");
	        TimeID = glGetUniformLocation(programID, "time");
            
            loadOBJ(objPath, vertices, uvs, normals);
            
            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);

            GLfloat g_color_buffer_data[vertices.size()*3];

            for (unsigned v = 0; v < vertices.size() ; v++){
                g_color_buffer_data[3*v+0] = 0.6;
                g_color_buffer_data[3*v+1] = 0.1;
                g_color_buffer_data[3*v+2] = 0.8;
            }

            glGenBuffers(1, &colorbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

            glGenBuffers(1, &normalsbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

            ModelMatrix = glm::mat4(1.0);

        }
        
        void affectMVP(){
            glm::mat4 ProjectionMatrix = getProjectionMatrix();
		    glm::mat4 ViewMatrix = getViewMatrix();

		    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

        }

        void draw(){
        	glUseProgram(Object::programID);
            affectMVP();
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
            glDisableVertexAttribArray(2);
        }

        void translate(glm::vec3 vect){
            ModelMatrix = glm::translate(ModelMatrix, vect);
        }
        void rotate(float angle, glm::vec3 vect){
            ModelMatrix = glm::rotate(ModelMatrix, angle, vect);
        }

        ~Object(){
            glDeleteBuffers(1, &vertexbuffer);
	        glDeleteBuffers(1, &colorbuffer); 
	        glDeleteBuffers(1, &normalsbuffer); 
	        glDeleteProgram(programID);
        }
            
};

#endif
