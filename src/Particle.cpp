//std
#include <cmath>

//particles
#include "particles/inc/Barrier.hpp"
#include "particles/inc/Particle.hpp"

namespace particles
{
	//constructors
	Particle::Particle(void) : m_status(true), m_mass(1), m_radius(0), m_color(1, 1, 1), m_position(0, 0, 0)
	{
		return;
	}

	//destructor
	Particle::~Particle(void)
	{
		return;
	}

	//draw
	unsigned Particle::vbo_size(void) const
	{
		return 5 * m_nv * sizeof(float);
	}
	unsigned Particle::ibo_size(void) const
	{
		return 3 * (m_nv - 2) * sizeof(unsigned);
	}
	void Particle::draw(unsigned* ibo_data, float* vbo_data) const
	{
		//ibo data
		for(unsigned i = 0; i < m_nv - 2; i++)
		{
			ibo_data[3 * (m_nv - 2) * m_index + 3 * i + 0] = m_nv * m_index + 0;
			ibo_data[3 * (m_nv - 2) * m_index + 3 * i + 1] = m_nv * m_index + i + 1;
			ibo_data[3 * (m_nv - 2) * m_index + 3 * i + 2] = m_nv * m_index + i + 2;
		}
		//vbo data
		for(unsigned i = 0; i < m_nv; i++)
		{
			vbo_data[5 * m_nv * m_index + 5 * i + 2] = m_color[0];
			vbo_data[5 * m_nv * m_index + 5 * i + 3] = m_color[1];
			vbo_data[5 * m_nv * m_index + 5 * i + 4] = m_color[2];
			vbo_data[5 * m_nv * m_index + 5 * i + 0] = m_position[0] + m_radius * cos(2 * M_PI * i / m_nv);
			vbo_data[5 * m_nv * m_index + 5 * i + 1] = m_position[1] + m_radius * sin(2 * M_PI * i / m_nv);
		}
	}

	//physics
	void Particle::update(double h)
	{
		//gravity
		const double g = 9.81;
		math::vec3 a = math::vec3(0, -g, 0);
		const math::vec3 position_old = m_position;
		//update
		if(!m_status)
		{
			m_velocity += h * a;
			m_position += h * m_velocity + h * h * a;
		}
		else
		{
			m_status = false;
			m_velocity += h / 2 * a;
			m_position += h * m_velocity + h * h / 2 * a;
		}
		//barriers
		for(const Barrier& barrier : *m_list_barriers)
		{
			//data
			const math::vec3 x1 = barrier.m_x1;
			const math::vec3 x2 = barrier.m_x2;
		}
	}

	//static
	const std::vector<Barrier>* Particle::m_list_barriers = nullptr;
	const std::vector<Particle>* Particle::m_list_particles = nullptr;
}