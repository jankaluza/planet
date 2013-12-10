
#pragma once

#include "../GLee.h"

#include <SDL/SDL.h>

namespace Planet2 {

class Ship {
	public:
		Ship();
		virtual ~Ship();

		void draw(GLuint positionAttrib, GLuint normalAttrib);

		void toggle() {
			_toggled = !_toggled;
		}

		bool isToggled() {
			return _toggled;
		}

	private:
		GLuint _vbo;
		GLuint _ebo;

		GLuint _vertexShader;
		GLuint _fragmentShader;
		GLuint _shaderProgram;
		bool _toggled;

};


}
