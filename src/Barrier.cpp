//ext
#include "external/cpp/inc/GL/glew.h"

//particles
#include "particles/inc/Barrier.hpp"

namespace particles
{
	//constructor
	Barrier::Barrier(void) : m_x1(0, 0, 0), m_x2(0, 0, 0), m_index(0), m_color(1, 1, 1)
	{
		return;
	}

	//destructor
	Barrier::~Barrier(void)
	{
		return;
	}

	//buffers
	void Barrier::setup_buffers(GLuint ibo, GLuint vbo)
	{
		//ibo data
		m_ibo_data[0] = 2 * m_index + 0;
		m_ibo_data[1] = 2 * m_index + 1;
		//vbo data
		m_vbo_data[0] = m_x1[0];
		m_vbo_data[1] = m_x1[1];
		m_vbo_data[5] = m_x2[0];
		m_vbo_data[6] = m_x2[1];
		m_vbo_data[2] = m_vbo_data[7] = m_color[0];
		m_vbo_data[3] = m_vbo_data[8] = m_color[1];
		m_vbo_data[4] = m_vbo_data[9] = m_color[2];
		//transfer
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferSubData(GL_ARRAY_BUFFER, 10 * m_index * sizeof(float), 10 * sizeof(float), m_vbo_data);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 2 * m_index * sizeof(unsigned), 2 * sizeof(unsigned), m_ibo_data);
	}
}