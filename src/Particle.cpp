//std
#include <cmath>
#include <cstring>

//ext
#include "external/cpp/inc/GL/glew.h"

//particles
#include "particles/inc/Barrier.hpp"
#include "particles/inc/Particle.hpp"

namespace particles
{
	//constructors
	Particle::Particle(void) : 
		m_status(true), m_mass(1), m_radius(0), m_color(1, 1, 1), m_position(0, 0, 0)
	{
		return;
	}

	//destructor
	Particle::~Particle(void)
	{
		return;
	}

	//physics
	void Particle::update(double h, float* vbo_data)
	{
		//data
		const double g = 9.81;
		const double s = m_status ? 2 : 1;
		math::vec3 a = math::vec3(0, -g, 0);
		//update
		m_position += h * m_velocity + h * h / s * a;
		m_velocity += h / s * a;
		//buffers
		vbo_data[6 * m_index + 5] = m_radius;
		vbo_data[6 * m_index + 2] = m_color[0];
		vbo_data[6 * m_index + 3] = m_color[1];
		vbo_data[6 * m_index + 4] = m_color[2];
		vbo_data[6 * m_index + 0] = m_position[0];
		vbo_data[6 * m_index + 1] = m_position[1];
	}

	//buffers
	void Particle::setup_buffers(GLuint ibo, GLuint vbo)
	{
		//data
		float vbo_data[6];
		vbo_data[5] = m_radius;
		vbo_data[2] = m_color[0];
		vbo_data[3] = m_color[1];
		vbo_data[4] = m_color[2];
		vbo_data[0] = m_position[0];
		vbo_data[1] = m_position[1];
		const unsigned ibo_data = m_index;
		//transfer
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferSubData(GL_ARRAY_BUFFER, 6 * m_index * sizeof(float), 6 * sizeof(float), vbo_data);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_index * sizeof(unsigned), sizeof(unsigned), &ibo_data);
	}

	//static
	const std::vector<Barrier>* Particle::m_list_barriers = nullptr;
	const std::vector<Particle>* Particle::m_list_particles = nullptr;
}