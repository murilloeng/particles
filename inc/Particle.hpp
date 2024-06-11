#pragma once

//std
#include <vector>

//math
#include "Math/inc/linear/vec3.hpp"

namespace particles
{
	class Barrier;
}

namespace particles
{
	class Particle
	{
	public:
		//constructors
		Particle(void);

		//destructor
		~Particle(void);

		//physics
		void update(double, float*);

		//buffers
		void setup_buffers(GLuint, GLuint);

		//data
		bool m_status;
		double m_mass;
		double m_radius;
		unsigned m_index;
		math::vec3 m_color;
		math::vec3 m_position;
		math::vec3 m_velocity;
		static const std::vector<Barrier>* m_list_barriers;
		static const std::vector<Particle>* m_list_particles;
	};
}