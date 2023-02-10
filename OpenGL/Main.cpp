#include "Vector.h"

#include "Cube.h"
#include "Scene.h"

using namespace std;


//GLenum DrawMode = GL_FILL;
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(window, true);
//	}
//	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//	{
//		DrawMode = GL_LINE;
//	}
//}

int main()
{
	Scene* scene = new Scene();
	scene->AddMesh(new Cube(Transform(Vector3f(500, 0, 0), Vector3f(0, 30, 0))));
	scene->AddMesh(new Cube(Transform(Vector3f(800, 500, 0), Vector3f(30, 30, 0))));

	//äÖÈ¾
	while (!scene->ShouldWindowClose())
	{
		//processInput(window);

#pragma region Rendering

		scene->Render();
#pragma endregion

		
	}
	scene->Release();
	return 0;
}