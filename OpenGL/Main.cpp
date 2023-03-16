#include "Vector.h"

#include "Cube.h"
#include "Scene.h"
#include "GLFW/glfw3.h"
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
	
	Mesh* cube1 = new Cube(Transform(Vector3f(150, 0, 0), Vector3f(0, 0, 10), Vector3f(0.5)));
	scene->AddMesh(cube1);
	scene->AddMesh(new Cube(Transform(Vector3f(100, 100, 0), Vector3f(30, 30, 0), Vector3f(0.7))));

	//äÖÈ¾
	while (!scene->ShouldWindowClose())
	{
		//processInput(window);

#pragma region Rendering

		cube1->MeshTransform.Rotation = Vector3f(0, 0, glfwGetTime() * 10.f);

		scene->Render();
#pragma endregion

		
	}
	scene->Release();
	return 0;
}