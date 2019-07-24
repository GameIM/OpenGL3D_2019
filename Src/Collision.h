/**
* @file Collision.h
*/
#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Collision
{
	/**
	* ‹…
	*/
	struct Sphere
	{
		glm::vec3 center = glm::vec3(0);//’†SÀ•W
		float r = 0;//”¼Œa
	};
	bool TestSphereSphere(const Sphere&, const Sphere&);
}
#endif 