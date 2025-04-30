#include "geometry.h"

bool geo::sphere_intersection(const glm::vec4& sA, const glm::vec4& sB, const glm::vec4& sC, glm::vec3& p1, glm::vec3& p2) {
	// centers
	glm::vec3 A = sA;
	glm::vec3 B = sB;
	glm::vec3 C = sC;

	// compute relevant local frame
	glm::vec3 ez = glm::normalize(glm::cross(B - A, C - A));
	glm::vec3 ex = glm::normalize(B - A);
	glm::vec3 ey = glm::cross(ez, ex);

	// compute intersection in the local frame
	float a = glm::length(B - A);
	float b = glm::dot(C - A, ex);
	float c = glm::dot(C - A, ey);

	float x = sA.w * sA.w - sB.w * sB.w + a * a;
	float y = (sB.w * sB.w - sC.w * sC.w - (x - a) * (x - a) + (x - b) * (x - b) + c * c) / (2.f * c);
	float z2 = sA.w * sA.w - x * x - y * y;

	if (z2 < 0.f) return false;

	float z = std::sqrt(z2);

	// return in global frame
	p1 = A + x * ex + y * ey + z * ez;
	p2 = A + x * ex + y * ey - z * ez;
	return true;
}








