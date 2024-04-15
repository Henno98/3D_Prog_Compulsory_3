#include "Trophy.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "Shaders/EBO.h"
#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"

Trophy::Trophy()
{
	Color = glm::vec3(0.f, 1.f, 1.f);
	trophycollected = false;
}
void Trophy::PlaceTrophy(glm::vec3 position, glm::vec3 size)
{


	if (!placed == false)
		return;

	TrophyMatrix = glm::translate(TrophyMatrix, position);
	placed = true;
	
	

}

void Trophy::DrawTrophy(glm::vec3 position, glm::vec3 size, Shader& shader, const char* uniform)
{
	if (!trophycollected == false)
		return;


	GLfloat Vertices[] = 
	{
		0,-size.y,0.f,Color.x,Color.y,Color.z,
		size.x,0.f,size.z,0,1,0,
		-size.x,0,size.z,1,0,1,
		size.x,0,-size.z,0,1,0,
		-size.x,0,-size.z,1,0,1,
		0,size.y,0.f,Color.x,Color.y,Color.z



	};

	GLuint trophyIndices[]
	{

		0,1,2,
		0,2,4,
		0,4,3,
		0,3,1,
		5,1,2,
		5,2,4,
		5,4,3,
		5,3,1,

	};

	
	

	//Draw Square
	VAO trophyVAO;
	trophyVAO.Bind();

	VBO trophyVBO(Vertices,sizeof(Vertices));
	trophyVBO.Bind();

	EBO trophyEBO(trophyIndices, sizeof(trophyIndices));
	trophyEBO.Bind();

	//Specify vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int modelloc = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(TrophyMatrix));

	PlaceTrophy(position, size);
	AABB.Position = position - glm::vec3(size.x / 2, 0, size.z / 2);
	AABB.Extent = size;

	glDrawElements(GL_TRIANGLES, sizeof(trophyIndices), GL_UNSIGNED_INT, nullptr);

	trophyVAO.Unbind();
	trophyVBO.Unbind();
	trophyEBO.Unbind();

	trophyVAO.Delete();
	trophyVBO.Delete();
	trophyEBO.Delete();
}

void Trophy::DestroyTrophy()
{
	trophycollected = true;
}
