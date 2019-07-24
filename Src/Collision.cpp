/**
* @file Collision.cpp
*/
#include "Collision.h"

namespace Collision
{
	/**
	* ‹…‚Æ‹…‚ªÕ“Ë‚µ‚Ä‚¢‚é’²‚×‚é
	*
	* @param s0 ”»’è‘ÎÛ‚Ì‹…‚»‚Ì1
	* @param s1 ”»’è‘ÎÛ‚Ì‹…‚»‚Ì2
	*
	* @retval true Õ“Ë‚µ‚Ä‚¢‚é
	* @retval false Õ“Ë‚µ‚Ä‚¢‚È‚¢
	*/
	bool TestSphereSphere(const Sphere& s0, const Sphere& s1)
	{
		const glm::vec3 m = s0.center - s1.center;
		const float radiusSum = s0.r + s1.r;
		return glm::dot(m, m) <= radiusSum * radiusSum;
	}
}