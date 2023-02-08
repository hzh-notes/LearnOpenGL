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
	scene->AddMesh(new Cube(Transform(Vector3f(200, 0, 0), Vector3f(0, 30, 0))));
	scene->AddMesh(new Cube(Transform(Vector3f(200, 100, 0), Vector3f(0, 30, 0))));

	//渲染
	while (!scene->ShouldWindowClose())
	{
		//processInput(window);

		

#pragma region Rendering

		// 激活着色器
		// 更新uniform颜色
		/*if (Program)
		{
			float timeValue = glfwGetTime();
			float greenValue = sin(timeValue) / 2.0f + 0.5f;
			Program->SetUniform4F("ColorTest", Vector4f(1.0f, greenValue, 1.0f, 1.0f));
			
		}*/
		
		
		//glPolygonMode(GL_FRONT_AND_BACK, DrawMode);

		//
		////绘制
		////glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

		scene->Render();
#pragma endregion

		
	}
	scene->Release();
	return 0;
}