#include "sdf.h"


float sdf::d_sphere(const glm::vec3& p, const float& r) {
	return glm::length(p) - r;
}
float sdf::d_box(const glm::vec3& p, const glm::vec3& size) {
	glm::vec3 q = abs(p) - size;
	return glm::length(glm::max(q, 0.f)) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.f);
}


float sdf::evaluation(sdf::shape s, const glm::vec3& p) {
	float d = 0.f;

	switch (s) {
	case sdf::shape::SPHERE:
		d = sdf::d_sphere(p, 1.f);
		break;
	case sdf::shape::CUBE:
		d = sdf::d_box(p, glm::vec3(1, 2, 3));
		break;
	default:
		d = sdf::d_sphere(p, 1.f);
		break;
	}

	return d;
}

std::string sdf::glsl_txt(sdf::shape s) {
	std::string txt = "";

	switch (s) {
	case sdf::shape::SPHERE:
		txt = R"(
			float sdf(vec3 p) {
				return d_sphere(p, 1.);
			}
		)";
		break;
	case sdf::shape::CUBE:
		txt = R"(
			float sdf(vec3 p) {
				return d_box(p, vec3(1, 2, 3));
			}
		)";
	default:
		txt = R"(
			float sdf(vec3 p) {
				return d_sphere(p, 1.);
			}
		)";
		break;
	}

	return txt;
}