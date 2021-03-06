// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>


#define GLM_ENABLE_EXPERIMENTAL

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

# include "lib/controls.hpp"
# include "lib/glLib.hpp"
# include "lib/objloader.hpp"
# include "lib/shader.hpp"	

# include "lib/objectWithLightClass.hpp"
# include "lib/lightClass.hpp"

//imGUI
# include "lib/Vendor/imgui.h"
# include "lib/Vendor/imgui_impl_glfw.h"
# include "lib/Vendor/imgui_impl_opengl3.h"



float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


int main( void ){
	window = initWindow();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	//Object suzanne("obj/suzanne.obj","src/shader/SimpleFragmentShader.glsl","src/shader/SimpleVertexShader.glsl",lumiere);
	//Object room("obj/room.obj","src/shader/SimpleFragmentShader.glsl","src/shader/SimpleVertexShader.glsl",lumiere);
	Light lumiere(glm::vec3(1,0,1),"obj/cube.obj","src/shader/LightFragmentShader.glsl","src/shader/LightVertexShader.glsl");
	//ObjectWithLight cube("obj/cube.obj","src/shader/SimpleFragmentShader.glsl","src/shader/SimpleVertexShader.glsl",lumiere);
	ObjectWithLight cube("obj/Porsche.obj","src/shader/SimpleFragmentShader.glsl","src/shader/SimpleVertexShader.glsl",lumiere);



	ObjectWithLight tab[] = {cube} ;

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);



	do{ 
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();

		float time = glfwGetTime();

		for (unsigned i =0; i < sizeof(tab)/sizeof(tab[0]) ; i++){
			glEnableVertexAttribArray(3); //pour le time
            glBindBuffer(GL_ARRAY_BUFFER, cos(time));
			
			tab[i].draw();
			tab[i].rotate(0.01,glm::vec3(0,1,0));

		}
		lumiere.draw();
		
		lumiere.translate(vec3(sin(time)* 0.05,cos(time)* 0.05,0));

		// render your GUI
		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::End();

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//lumiere.rotate((GLfloat) 0.01, glm::vec3(0,0,0));
		//lumiere.translate(glm::vec3(0,0,-0.01));

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	// Check if the ESC key was pressed or the window was closed
	}while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	
	glfwTerminate();

	return 0;
}