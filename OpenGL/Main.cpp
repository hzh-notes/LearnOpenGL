#include "Vector.h"

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
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
	
	Mesh* cube1 = new Cube(Transform(Vector3f(150, -20, 0), Vector3f(0, 0, 0), Vector3f(0.5)));
	Mesh* cube2 = new Cube(Transform(Vector3f(150, 50, 0), Vector3f(0, 0, 0), Vector3f(0.5)));
	Mesh* sphere = new Sphere(Transform(Vector3f(0, 0, 100), Vector3f(0, 0, 0), Vector3f(0.05)));
	Mesh* floor = new Plane(Transform(Vector3f(0, 0, -150), Vector3f(0, 0, 0), Vector3f(10)));
	Mesh* grass = new Plane(Transform(Vector3f(75, 0, 0), Vector3f(0, -90, 0), Vector3f(1)));

	Material* Mat0 = new Material(0);
	Mat0->SetTextureParam("container2.png", ETextureCategory::Diffuse);
	Mat0->SetTextureParam("container2_specular.png", ETextureCategory::Specular);
	Mat0->Compile();

	Material* lightMat = new Material(1);
	lightMat->SetTextureParam("matrix.jpg", ETextureCategory::Emission);
	lightMat->bEmission = true;
	lightMat->Compile();

	Material* floorMat = new Material(2);
	floorMat->SetTextureParam("floor.jpg", ETextureCategory::Diffuse);
	floorMat->SetTextureParam("container2_specular.png", ETextureCategory::Specular);
	floorMat->Compile();

	Material* grassMat = new Material(3);
	grassMat->SetTextureParam("blending_transparent_window.png", ETextureCategory::Diffuse);
	grassMat->Compile();

	cube1->SetMaterial(Mat0);
	cube2->SetMaterial(Mat0);
	sphere->SetMaterial(lightMat);
	floor->SetMaterial(floorMat);
	grass->SetMaterial(grassMat);

	scene->AddMesh(cube1);
	scene->AddMesh(cube2);
	scene->AddMesh(sphere);
	scene->AddMesh(floor);
	scene->AddMesh(grass);

	//äÖÈ¾
	while (!scene->ShouldWindowClose())
	{

#pragma region Rendering

		scene->Render();
#pragma endregion

		
	}
	delete scene, cube1, cube2, floor, grass, Mat0, floorMat, lightMat, grassMat;
	scene = nullptr;
	cube1 = nullptr;
	cube2 = nullptr;
	floor = nullptr;
	grass = nullptr;
	Mat0 = nullptr;
	lightMat = nullptr;
	floorMat = nullptr;
	grassMat = nullptr;
	return 0;
}