#include "Vector.h"

#include "Cube.h"
#include "Sphere.h"
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
	
	Mesh* cube1 = new Cube(Transform(Vector3f(150, 0, 0), Vector3f(0, 0, 0), Vector3f(0.5)));
	Mesh* cube2 = new Sphere(Transform(Vector3f(150, 0, 80), Vector3f(0, 0, 0), Vector3f(0.5)));
	scene->AddMesh(cube1);
	scene->AddMesh(cube2);

	Material mat1, mat2;
	mat1.diffuse = "awesomeface.png";
	mat1.specular = "container2_specular.png";
	mat1.emission = "matrix.jpg";
	mat2.diffuse = "awesomeface.png";
	mat2.specular = "lighting_maps_specular_color.png";
	mat2.emission = "matrix.jpg";

	cube1->SetMaterial(mat1);
	cube2->SetMaterial(mat2);
	//äÖÈ¾
	while (!scene->ShouldWindowClose())
	{
		//processInput(window);

#pragma region Rendering

		cube1->MeshTransform.Rotation.Z = glfwGetTime() * 10.f;
		cube2->MeshTransform.Rotation.Z = glfwGetTime() * 20.f;
		scene->Render();
#pragma endregion

		
	}
	scene->Release();
	return 0;
}