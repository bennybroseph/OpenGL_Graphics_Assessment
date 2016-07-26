#include "GUIApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

GUIApplication::GUIApplication()
	: m_camera(nullptr) {

}

GUIApplication::~GUIApplication() {

}

bool GUIApplication::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes
	Gizmos::create();

	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	ImGui_ImplGlfwGL3_Init(m_window, true);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1280;
	io.DisplaySize.y = 720;

	m_drawGrid = true;
	m_clearColour = vec4(0.25f, 0.25f, 0.25f, 1);

	return true;
}

void GUIApplication::shutdown() {

	ImGui_ImplGlfwGL3_Shutdown();

	// cleanup camera and gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool GUIApplication::update(float deltaTime) {
	
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);

	// clear the gizmos out for this frame
	Gizmos::clear();
	
	//Clear ImGui
	ImGui_ImplGlfwGL3_NewFrame();

	vec4 white(1, 1, 1, 0);
	vec4 black(0, 0, 0, 1);

	if (m_drawGrid) {
		// for now let's add a grid to the gizmos
		for (int i = 0; i < 21; ++i) {
			Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
							i == 10 ? white : black);
			Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
							i == 10 ? white : black);
		}
	}

	Gizmos::addTransform(mat4(1));
	
	// imgui example
	ImGui::Begin("Rendering Options");
		ImGui::ColorEdit3("clear color", glm::value_ptr(m_clearColour));
		ImGui::Checkbox("render grid", &m_drawGrid);
	ImGui::End();

	glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a);

	// return true, else the application closes
	return true;
}

void GUIApplication::draw() {

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(guiMatrix);

	ImGui::Render();
}