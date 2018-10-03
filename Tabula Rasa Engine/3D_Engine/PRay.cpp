#include "PRay.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PRay::PRay(math::vec origin, math::vec direction) : trPrimitive(), origin(origin), direction(direction)
{
	type = PrimitiveTypes::Primitive_Ray;

	//todo direction have to be calculated to the infinite for now is like an arrow

	float vertices_array[6] = {
		origin.x, origin.y, origin.z,
		direction.x, direction.y, direction.z
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint indices_array[2] = {
		0, 1
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 2, indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PRay::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glLineWidth(3.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glDrawElements(GL_LINES, vertices_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glLineWidth(1.0f);

	glDisableClientState(GL_VERTEX_ARRAY);

}