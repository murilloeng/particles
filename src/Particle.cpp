//std
#include <cmath>

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
	void Particle::update(double h, GLuint vbo)
	{
		//data
		const double g = 9.81;
		const double s = m_status ? 2 : 1;
		math::vec3 a = math::vec3(0, -g, 0);
		const math::vec3 dx = h * m_velocity + h * h / s * a;
		//update
		m_status = false;
		m_position += dx;
		m_velocity += h / s * a;
		//barriers
		// for(const Barrier& barrier : *m_list_barriers)
		// {
		// 	//data
		// 	const math::vec3 x1 = barrier.m_x1;
		// 	const math::vec3 x2 = barrier.m_x2;
		// }
		//buffers
		for(unsigned i = 0; i < m_nv; i++)
		{
			m_vbo_data[5 * i + 0] += dx[0];
			m_vbo_data[5 * i + 1] += dx[1];
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 5 * m_nv * m_index * sizeof(float), 5 * m_nv * sizeof(float), m_vbo_data);
	}

	//buffers
	void Particle::setup_buffers(GLuint ibo, GLuint vbo)
	{
		//ibo data
		for(unsigned i = 0; i < m_nv - 2; i++)
		{
			m_ibo_data[3 * i + 0] = m_nv * m_index + 0;
			m_ibo_data[3 * i + 1] = m_nv * m_index + i + 1;
			m_ibo_data[3 * i + 2] = m_nv * m_index + i + 2;
		}
		//vbo data
		for(unsigned i = 0; i < m_nv; i++)
		{
			m_vbo_data[5 * i + 2] = m_color[0];
			m_vbo_data[5 * i + 3] = m_color[1];
			m_vbo_data[5 * i + 4] = m_color[2];
			m_vbo_data[5 * i + 0] = m_position[0] + m_radius * cos(2 * M_PI * i / m_nv);
			m_vbo_data[5 * i + 1] = m_position[1] + m_radius * sin(2 * M_PI * i / m_nv);
		}
		//transfer
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferSubData(GL_ARRAY_BUFFER, 5 * m_nv * m_index * sizeof(float), 5 * m_nv * sizeof(float), m_vbo_data);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 3 * (m_nv - 2) * m_index * sizeof(unsigned), 3 * (m_nv - 2) * sizeof(unsigned), m_ibo_data);
	}

	//static
	const std::vector<Barrier>* Particle::m_list_barriers = nullptr;
	const std::vector<Particle>* Particle::m_list_particles = nullptr;
}