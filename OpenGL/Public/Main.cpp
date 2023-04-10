#include "Math/Vector.h"

#include "Shapes/Cube.h"
#include "Shapes/Plane.h"
#include "Shapes/Sphere.h"
#include "Scene.h"


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

	Material* Mat0 = new Material();
	Mat0->SetTextureParam("container2.png", ETextureCategory::Diffuse);
	Mat0->SetTextureParam("container2_specular.png", ETextureCategory::Specular);

	Material* lightMat = new Material();
	lightMat->SetTextureParam("matrix.jpg", ETextureCategory::Emission);

	Material* floorMat = new Material();
	floorMat->SetTextureParam("floor.jpg", ETextureCategory::Diffuse);
	floorMat->SetTextureParam("container2_specular.png", ETextureCategory::Specular);

	Material* grassMat = new Material();
	grassMat->SetTextureParam("blending_transparent_window.png", ETextureCategory::Diffuse);

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