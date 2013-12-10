
#include "Ship.h"
#include "../pgr.h"

using namespace Planet2;

struct ShipVertex {
	float position[3];
	float normal[3];
} shipVertices[3] = {
	{ { -0.1, 0.0, 0.0 }, { 0, 0, 1 } },
	{ { 0.1, 0.0, 0.0 }, { 0, 0, 1 } },
	{ { 0.0, 0.2, 0.0 }, { 0, 0, 1 } }
};

const unsigned short ship[1][3] = {
	{ 0, 1, 2 },
};


Ship::Ship() : _toggled(false) {

	// Compile shaders
// 	_vertexShader = compileShader(GL_VERTEX_SHADER, "#version 130\n in vec2 position; void main() { gl_Position = vec4(position,0,1); }");
// 	_fragmentShader = compileShader(GL_FRAGMENT_SHADER, "#version 130\n uniform vec3 color; out vec4 fragColor; void main() { fragColor = vec4(color,1); }");

	// Link shaders
// 	_shaderProgram = linkShader(2, _vertexShader, _fragmentShader);

	// Copy sphere to graphics card
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shipVertices), shipVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ship), ship, GL_STATIC_DRAW);
}

Ship::~Ship() {
	
}

void Ship::draw(GLuint positionAttrib, GLuint normalAttrib) {
// 	glUseProgram(_shaderProgram);
	glDisable(GL_DEPTH_TEST);
// 	glEnable(GL_BLEND);

	glEnableVertexAttribArray(positionAttrib);
	glEnableVertexAttribArray(normalAttrib);


	if (_toggled) {

	}
	else {
		// Draw ship
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ShipVertex), (void*)offsetof(ShipVertex, position));
		glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ShipVertex), (void*)offsetof(ShipVertex, normal));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glDrawElements(GL_TRIANGLES, sizeof(ship)/sizeof(**ship), GL_UNSIGNED_SHORT, NULL);
	}
// 	glDisable(GL_BLEND);
}

