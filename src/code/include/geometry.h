#include <glm/glm.hpp>

namespace geo {


	bool sphere_inclusion_check(const glm::vec4& s, const glm::vec3& p);
	bool sphere_intersection_check(const glm::vec4& sA, const glm::vec4& sB);
	bool sphere_intersection(const glm::vec4& sA, const glm::vec4& sB, const glm::vec4& sC, glm::vec3& p1, glm::vec3& p2);

}