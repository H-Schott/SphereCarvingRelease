#include "sdf.h"


float sdf::d_sphere(const glm::vec3& p, const float& r) {
	return glm::length(p) - r;
}
float sdf::d_box(const glm::vec3& p, const glm::vec3& size) {
	glm::vec3 q = abs(p) - size;
	return glm::length(glm::max(q, 0.f)) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.f);
}
float sdf::d_cylinder(glm::vec3 p, float h, float r) {
	glm::vec2 d = glm::abs(glm::vec2(glm::length(glm::vec2(p.x, p.z)), p.y)) - glm::vec2(r, h);
	return glm::min(glm::max(d.x, d.y), 0.f) + glm::length(glm::max(d, 0.f));
}


float sdf::evaluation(const sdf::shape& s, const glm::vec3& p) {
	float d = 0.f;

	switch (s) {
	case sdf::shape::SPHERE:
		d = sdf::d_sphere(p, 1.f);
		break;
	case sdf::shape::BOX:
		d = sdf::d_box(p, glm::vec3(0.3, 0.6, 1.));
		break;
	case sdf::shape::CYLINDER:
		d = sdf::d_cylinder(p, 1.f, 0.3);
		break;
	case sdf::shape::CAD:
		d = d_box(p, glm::vec3(0.3, 0.6, 1.));
		d = glm::max(d, -d_box(p - glm::vec3(0.2, -0.4, 0.), glm::vec3(0.3, 0.6, 0.1)));
		glm::vec3 q = glm::vec3(p.z, p.x, p.y);
		d = glm::max(d, -d_cylinder(q, 0.4, 0.5));
		d = glm::max(d, -d_cylinder(q + glm::vec3(0.85, 0., 0.45), 4., 0.1));
		d = glm::max(d, -d_cylinder(q + glm::vec3(-0.85, 0., 0.45), 0.4, 0.1));
		d = glm::max(d, -d_cylinder(q + glm::vec3(0.85, 0., -0.45), 0.4, 0.1));
		d = glm::max(d, -d_cylinder(q + glm::vec3(-0.85, 0., -0.45), 0.4, 0.1));
		break;
	default:
		d = sdf::d_sphere(p, 1.f);
		break;
	}

	return d;
}

std::string sdf::glsl_txt(const sdf::shape& s) {
	std::string txt = "";

	switch (s) {
	case sdf::shape::SPHERE:
		txt = R"(
			float sdf(vec3 p) {
				return d_sphere(p, 1.);
			}
		)";
		break;
	case sdf::shape::BOX:
		txt = R"(
			float sdf(vec3 p) {
				return d_box(p, vec3(0.3, 0.6, 1.));
			}
		)";
		break;
	case sdf::shape::CYLINDER:
		txt = R"(
			float sdf(vec3 p) {
				return d_cylinder(p, 1., 0.3);
			}
		)";
		break;
	case sdf::shape::CAD:
		txt = R"(
			float sdf(vec3 p) {
				float d = d_box(p, vec3(0.3, 0.6, 1.));
				d = max(d, - d_box(p - vec3(0.2, -0.4, 0.), vec3(0.3, 0.6, 0.1)));
				p = p.zxy;
				d = max(d, - d_cylinder(p, 0.4, 0.5));
				d = max(d, - d_cylinder(p + vec3(0.85, 0., 0.45), 4., 0.1));
				d = max(d, - d_cylinder(p + vec3(-0.85, 0., 0.45), 0.4, 0.1));
				d = max(d, - d_cylinder(p + vec3(0.85, 0., -0.45), 0.4, 0.1));
				d = max(d, - d_cylinder(p + vec3(-0.85, 0., -0.45), 0.4, 0.1));
				return d;
			}
		)";
		break;
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