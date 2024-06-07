#pragma once

//math
#include "Math/inc/linear/vec3.hpp"

namespace particles
{
	class Barrier
	{
	public:
		//constructors
		Barrier(void);

		//destructor
		~Barrier(void);

		//draw
		void draw(unsigned*, float*) const;

		//data
		math::vec3 m_x1;
		math::vec3 m_x2;
		unsigned m_index;
		math::vec3 m_color;
	};
}