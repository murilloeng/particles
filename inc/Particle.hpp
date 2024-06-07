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

		//draw
		void draw(float*, unsigned*) const;

		//data
		double m_radius;
		float m_color[3];
		math::vec3 m_position;
	};
}