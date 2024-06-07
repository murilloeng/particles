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

	//draw
	void Barrier::draw(unsigned* ibo_data, float* vbo_data) const
	{
		//ibo data
		ibo_data[2 * m_index + 0] = 2 * m_index + 0;
		ibo_data[2 * m_index + 1] = 2 * m_index + 1;
		//vbo data
		vbo_data[10 * m_index + 0] = m_x1[0];
		vbo_data[10 * m_index + 1] = m_x1[1];
		vbo_data[10 * m_index + 5] = m_x2[0];
		vbo_data[10 * m_index + 6] = m_x2[1];
		vbo_data[10 * m_index + 2] = vbo_data[10 * m_index + 7] = m_color[0];
		vbo_data[10 * m_index + 3] = vbo_data[10 * m_index + 8] = m_color[1];
		vbo_data[10 * m_index + 4] = vbo_data[10 * m_index + 9] = m_color[2];
	}
}