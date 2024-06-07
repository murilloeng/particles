//particles
#include "particles/inc/Particle.hpp"

namespace particles
{
	//constructors
	Particle::Particle(void) : m_radius(0), m_position(0, 0, 0)
	{
		return;
	}
	Particle::Particle(const math::vec3& position, double radius) : m_radius(radius), m_position(position)
	{
		return;
	}
	
	//destructor
	Particle::~Particle(void)
	{
		return;
	}

	//draw
	void Particle::draw(float*, unsigned*) const
	{
		return;
	}
}