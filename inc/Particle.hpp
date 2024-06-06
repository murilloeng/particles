#pragma once

//math
#include "Math/inc/linear/vec3.hpp"

namespace particles
{
	class Particle
	{
	public:
		//constructors
		Particle(void);
		Particle(const math::vec3&, double);

		//destructor
		~Particle(void);

		//data
		double m_radius;
		math::vec3 m_position;
	};
}