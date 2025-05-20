#include "sdf.h"


float sdf::d_sphere(const glm::vec3& p, const float& r) {
	return glm::length(p) - r;
}
float sdf::d_box(const glm::vec3& p, const glm::vec3& size) {
	glm::vec3 q = abs(p) - size;
	return glm::length(glm::max(q, 0.f)) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.f);
}
float sdf::d_cylinder(const glm::vec3& p, float h, float r) {
	glm::vec2 d = glm::abs(glm::vec2(glm::length(glm::vec2(p.x, p.z)), p.y)) - glm::vec2(r, h);
	return glm::min(glm::max(d.x, d.y), 0.f) + glm::length(glm::max(d, 0.f));
}
float sdf::d_cylinder(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, float r) {
	return d_cylinder(p, a, glm::normalize(b - a), r, glm::length(b - a));
}
float sdf::d_cylinder(const glm::vec3& p, const glm::vec3& a, const glm::vec3& axis, float r, float l) {
	glm::vec3 b = a + axis * l;

	glm::vec3  ba = b - a;
	glm::vec3  pa = p - a;
	float baba = glm::dot(ba, ba);
	float paba = glm::dot(pa, ba);
	float x = glm::length(pa * baba - ba * paba) - r * baba;
	float y = std::abs(paba - baba * 0.5f) - baba * 0.5f;
	float x2 = x * x;
	float y2 = y * y * baba;

	float d = (std::max(x, y) < 0.0f) ? -std::min(x2, y2) : (((x > 0.0f) ? x2 : 0.0f) + ((y > 0.0f) ? y2 : 0.0f));

	return glm::sign(d) * sqrt(abs(d)) / baba;
}
float sdf::d_smoothUnion(float d1, float d2, float r) {
	float h = std::max(r - abs(d1 - d2), 0.0f) / r;
	float cube = h * h * h * r * (1.0f / 6.0f);
	return (std::min(d1, d2) - cube) / std::max(1.f, r);
}
float sdf::d_torus(glm::vec3 p, glm::vec3 c, glm::vec3 axis, float r, float s) {
	glm::vec3 n = p - c;
	float z = dot(n, axis);
	z *= z;
	float d = dot(n, n);
	float y = r - sqrt(d - z);
	y = sqrt(y * y + z);
	return y - s;
}

// cad_2
float c2_sdfBox2(glm::vec2 p2, glm::vec2 c, glm::vec2 d) {
	glm::vec2 q = abs(p2 - c) - d;
	return glm::length(glm::max(q, glm::vec2(0))) + glm::min(glm::max(q.x, q.y), 0.f);
}
float c2_cylinders(glm::vec3 p, glm::vec3 a, glm::vec3 axis, float l, float ri, float re, float e) {
	glm::vec3 n = p - a;
	float y = abs(dot(axis, n));
	float x = sqrt(dot(n, n) - y * y);
	glm::vec2 p2 = glm::vec2(x, y);
	float b1 = c2_sdfBox2(p2, glm::vec2(0, 0), glm::vec2(ri, l));
	float b2 = c2_sdfBox2(p2, glm::vec2(0, l - e / 2.f), glm::vec2(re, e / 2.f));
	return std::min(b1, b2);
}
float c2_disk2(glm::vec2 p2, glm::vec2 c, float r) {
	return length(p2 - c) - r;
}
float c2_tubular(glm::vec3 p, glm::vec3 a, glm::vec3 axis, float mr, float ht, float l) {
	glm::vec3 q = p - a;
	float s = dot(q, axis);
	float y = length(q - s * axis);
	s = s > l / 2. ? s - l : -s;
	y = abs(y - mr) - ht;
	if (s > 0.)return sqrt(s * s + glm::max(y, 0.f) * glm::max(y, 0.f));
	return std::max(y, s);
}
float c2_triangle2(glm::vec2 p2, glm::vec2 a, glm::vec2 b, glm::vec2 c) {
	glm::vec2 ab = b - a;
	glm::vec2 ap = p2 - a;
	glm::vec2 bc = c - b;
	glm::vec2 bp = p2 - b;
	glm::vec2 ca = a - c;
	glm::vec2 cp = p2 - c;
	float nab = glm::dot(glm::vec2(ab.y, -ab.x), ap);
	float nbc = glm::dot(glm::vec2(bc.y, -bc.x), bp);
	float nca = glm::dot(glm::vec2(ca.y, -ca.x), cp);
	if ((nab > 0.0f) || (nbc > 0.0f) || (nca > 0.0f)) return std::min(std::min(glm::distance(ab * glm::clamp(dot(ab, ap) / dot(ab, ab), 0.0f, 1.0f), ap), glm::distance(bc * glm::clamp(dot(bc, bp) / dot(bc, bc), 0.0f, 1.0f), bp)), glm::distance(ca * glm::clamp(dot(ca, cp) / dot(ca, ca), 0.0f, 1.0f), cp));
	return std::max(std::max(nab / glm::length(ab), nbc / glm::length(bc)), nca / glm::length(ca));
}
float c2_subtree_0(glm::vec3 p) {
	float a = glm::atan(p.y, p.x);
	float r = glm::length(glm::vec2(p));
	a += a < 0.0f ? 2.0f * 3.14159f : 0.0f;
	a = glm::mod(a, 1.0472f);
	a = a > 0.5f * 1.0472f ? 1.0472f - a : a;
	p = glm::vec3(r * cos(a), r * sin(a), p.z);
	return c2_cylinders(p, glm::vec3(7.79423f, 4.5f, -0.4f), glm::vec3(0, 0, 1), 0.5f, 0.3f, 0.4f, 0.2f);
}
float c2_subtree_1(glm::vec3 p) {
	float a = glm::atan(p.y, p.x);
	float r = glm::length(glm::vec2(p));
	a += a < 0.0f ? 2.0f * 3.14159f : 0.0f;
	a = glm::mod(a, 1.0472f);
	a = a > 0.5f * 1.0472f ? 1.0472f - a : a;
	p = glm::vec3(r * cos(a), r * sin(a), p.z);
	return c2_cylinders(p, glm::vec3(4.8f, 0, 5.6f), glm::vec3(0, 0, 1), 0.3f, 0.2f, 0.3f, 0.2f);
}
float c2_subtree_3(glm::vec2 p2) {
	float a = glm::atan(p2.y, p2.x);
	float r = glm::length(p2);
	a += a < 0.0f ? 2.0f * 3.14159f : 0.0f;
	a = glm::mod(a, 1.0472f);
	a = a > 0.5f * 1.0472f ? 1.0472f - a : a;
	p2 = glm::vec2(r * cos(a), r * sin(a));
	return c2_disk2(p2, glm::vec2(7.79423f, 4.5f), 1.4f);
}
float c2_subtree_2(glm::vec3 p) {
	glm::vec3 n = p - glm::vec3(0, 0, -0.8f);
	float s = glm::dot(glm::vec3(0, 0, 1), n);
	glm::vec3 q = glm::vec3(0, 0, -0.8f) + s * glm::vec3(0, 0, 1);
	glm::vec2 p2 = glm::vec2(glm::dot(glm::vec3(1, 0, 0), (p - q)), glm::dot(glm::vec3(0, 1, 0), (p - q)));
	float d = sdf::d_smoothUnion(c2_disk2(p2, glm::vec2(0, 0), 9), c2_subtree_3(p2), 1.4f);
	if (s <= 0) return glm::length(glm::vec2(s, std::max(d, 0.0f)));
	if (s >= 0.8f) return glm::length(glm::vec2(s - 0.8, std::max(d, 0.0f)));
	return std::max(d, std::max(-s, s - 0.8f));
}
float c2_subtree_5(glm::vec3 p) {
	glm::vec3 n = p - glm::vec3(0, 0, 0.7);
	float s = glm::dot(glm::vec3(0, 0, 1), n);
	glm::vec3 q = glm::vec3(0, 0, 0.7) + s * glm::vec3(0, 0, 1);
	glm::vec2 p2 = glm::vec2(glm::dot(glm::vec3(1, 0, 0), (p - q)), glm::dot(glm::vec3(0, 1, 0), (p - q)));
	float d = c2_triangle2(p2, glm::vec2(3.4641f, 2), glm::vec2(12.4641f, 2), glm::vec2(7.9641f, 9.79423f));
	if (s <= 0.0f) return glm::length(glm::vec2(s, std::max(d, 0.0f)));
	if (s >= 3.1f) return glm::length(glm::vec2(s - 3.1f, std::max(d, 0.0f)));
	return std::max(d, std::max(-s, s - 3.1f));
}
float c2_subtree_4(glm::vec3 p) {
	float a = glm::atan(p.y, p.x);
	float r = glm::length(glm::vec2(p));
	a += a < 0.0f ? 2.0f * 3.14159f : 0.0f;
	a = glm::mod(a, 1.0472f);
	a = a > 0.5f * 1.0472f ? 1.0472f - a : a;
	p = glm::vec3(r * cos(a), r * sin(a), p.z);
	return (c2_subtree_5(p) - (0.7f));
}
float c2_tree(glm::vec3 p) {
	float A = std::max(sdf::d_cylinder(p, glm::vec3(0, 0, -0.8f), glm::vec3(0, 0, 1), 9, 6.1f), -sdf::d_torus(p, glm::vec3(0, 0, 5), glm::vec3(0, 0, 1), 9, 5));
	float B = std::min(std::max(A, -c2_subtree_4(p)), sdf::d_cylinder(p, glm::vec3(0, 0, -1.4), glm::vec3(0, 0, 1), 8, 1.4f));
	return std::max(std::max(std::min(std::max(std::min(B, c2_subtree_2(p)), -(glm::length(glm::clamp(glm::dot(p - glm::vec3(0, 0, -2), glm::vec3(0, 0, 0.111111f)), 0.0f, 1.0f) * glm::vec3(0, 0, 9) + glm::vec3(0, 0, -2) - p) - 2)), std::max(std::min(c2_tubular(p, glm::vec3(0, 0, 4.8f), glm::vec3(0, 0, 1), 3.65f, 0.65f, 0.5f), c2_tubular(p, glm::vec3(0, 0, 5.3f), glm::vec3(0, 0, 1), 4.7f, 0.8f, 0.5f)), -sdf::d_torus(p, glm::vec3(0, 0, 5.4), glm::vec3(0, 0, 1), 3.7f, 0.1f))), -c2_subtree_1(p)), -c2_subtree_0(p));
}
// teapot
float tp_smax(float a, float b, float k) {
	return -sdf::d_smoothUnion(-a, -b, k);
}
float tp_smax(float a, float b) {
	return tp_smax(a, b, 0.1f);
}
float tp_sq(float x) {
	return x * x;
}
float tp_torus(float x, float y, float z, float R, float r) {
	glm::vec2 xz = glm::vec2(x, z);
	glm::vec2 q = glm::vec2(length(xz) - R, y);
	return glm::length(q) - r;
}
float tp_lid(float x, float y, float z) {
	float min_y = 0.06f;
	float ampli = 0.45f;
	y -= min_y;
	float v = std::sqrt(tp_sq(x) + tp_sq(y - 0.55f) + tp_sq(z)) - 1.4f;
	v = sdf::d_smoothUnion(v, tp_torus(y - 2.f, x, z, .2f, .08f), .1f);
	v = tp_smax(v, -std::sqrt(tp_sq(x) + tp_sq(y - 0.55f) + tp_sq(z)) + 1.3f);
	v = tp_smax(v, std::sqrt(tp_sq(x) + tp_sq(y - 2.5f) + tp_sq(z)) - 1.3f);

	v = tp_smax(v, -std::sqrt(tp_sq(x - .25f) + tp_sq(z - .35f)) + 0.05f, .05f);
	v = sdf::d_smoothUnion(v, tp_torus(x, (y - 1.45f) * .75f, z, .72f, .065f), .2f);
	return v;
}
float tp_nose(float x, float y, float z) {
	z -= std::sin((y + 0.8f) * 3.6f) * .15f;

	float v = std::sqrt(tp_sq(x) + tp_sq(z));

	v = std::abs(v - .3f + std::sin(y * 1.6f + .5f) * 0.18f) - .05f;
	v = tp_smax(v, -y - 1.f);
	v = tp_smax(v, y - 0.85f, .075f);

	return v;
}
float tp_final(glm::vec3 p) {
	float x = p.x;
	float y = p.y;
	float z = p.z;

	float v = 0.0f;
	v = sqrt(x * x + z * z) - 1.2f - sin(y * 1.5f + 2.0f) * .4f;
	v = tp_smax(v, abs(y) - 1.f, 0.3f);

	float v1 = sqrt(x * x * 4.f + tp_sq(y + z * .1f) * 1.6f + tp_sq(z + 1.2f)) - 1.0f;
	v1 = tp_smax(v1, -sqrt(tp_sq(z + 1.2f) + tp_sq(y + z * .12f + .015f) * 1.8f) + .8f, .3f);

	v = sdf::d_smoothUnion(v, tp_torus(y * 1.2f + .2f + z * .3f, x * .75f, z + 1.25f + y * .2f, .8f, .1f), .25f);
	v = sdf::d_smoothUnion(v, sqrt(tp_sq(x) + tp_sq(y - 1.1f) + tp_sq(z + 1.8f)) - .05f, .32f);

	float v3 = tp_nose(x, (y + z) * sqrt(.5f) - 1.6f, (z - y) * sqrt(.5f) - 1.1f);

	v = sdf::d_smoothUnion(v, v3, 0.2f);

	v = tp_smax(v, sdf::d_smoothUnion(sin(y * 1.4f + 2.0f) * 0.5f + .95f - sqrt(x * x + z * z), y + .8f, .2f));
	v = tp_smax(v, -sqrt(tp_sq(x) + tp_sq(y + .15f) + tp_sq(z - 1.5f)) + .12f);

	v = sdf::d_smoothUnion(v, tp_torus(x, y - 0.95f, z, 0.9f, .075f), 0.1f);
	v = sdf::d_smoothUnion(v, tp_torus(x, y + 1.05f, z, 1.15f, .05f), 0.15f);

	float anim = 1.f;
	float v2 = tp_lid(x, y + .5f - 0.5f * anim, z);
	v = std::min(v, v2);

	return v;
}
float tp_tree(glm::vec3 p) {
	p = glm::vec3(p.x, p.z, p.y);
	const float scale = 0.5f;
	p *= 1.f / scale;
	return tp_final(p) * scale * 0.8f;
}



float sdf::evaluation(const sdf::shape& s, const glm::vec3& p) {
	float d = 0.f;

	switch (s) {
	case sdf::shape::SPHERE:
		d = sdf::d_sphere(p, 1.f);
		break;
	case sdf::shape::SPHERES:
		d = sdf::d_sphere(p - glm::vec3(0.2, 0.3, 0.5), 0.15f);
		d = std::min(d, sdf::d_sphere(p - glm::vec3(0.3, -0.3, 0.1), 0.1f));
		d = std::min(d, sdf::d_sphere(p - glm::vec3(-0.2, 0., -0.4), 0.2f));
		d = std::min(d, sdf::d_sphere(p - glm::vec3(-0.4, -0.6, -0.2), 0.05f));
		break;
	case sdf::shape::BOX:
		d = sdf::d_box(p, glm::vec3(0.3, 0.6, 1.));
		break;
	case sdf::shape::CYLINDER:
		d = sdf::d_cylinder(p, 1.f, 0.3f);
		break;
	case sdf::shape::CAD_1:
		d = d_box(p, glm::vec3(0.3, 0.6, 1.));
		d = std::max(d, -d_box(p - glm::vec3(0.2, -0.4, 0.), glm::vec3(0.3, 0.6, 0.1)));
		glm::vec3 q = glm::vec3(p.z, p.x, p.y);
		d = std::max(d, -d_cylinder(q, 0.4f, 0.5));
		d = std::max(d, -d_cylinder(q + glm::vec3(0.85, 0., 0.45), 4.f, 0.1f));
		d = std::max(d, -d_cylinder(q + glm::vec3(-0.85, 0., 0.45), 0.4f, 0.1f));
		d = std::max(d, -d_cylinder(q + glm::vec3(0.85, 0., -0.45), 0.4f, 0.1f));
		d = std::max(d, -d_cylinder(q + glm::vec3(-0.85, 0., -0.45), 0.4f, 0.1f));
		break;
	case sdf::shape::CAD_2:
		d = c2_tree(p * 6.f) / 6.f;
		break;
	case sdf::shape::TEAPOT:
		d = tp_tree(p);
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
	case sdf::shape::SPHERES:
		txt = R"(
			float sdf(vec3 p) {
				float d = d_sphere(p - vec3(0.2, 0.3, 0.5), 0.15f);
				d = min(d, d_sphere(p - vec3(0.3, -0.3, 0.1), 0.1f));
				d = min(d, d_sphere(p - vec3(-0.2, 0., -0.4), 0.2f));
				d = min(d, d_sphere(p - vec3(-0.4, -0.6, -0.2), 0.05f));
				return d;
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
	case sdf::shape::CAD_1:
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
	case sdf::shape::CAD_2:
		txt = R"(
			float sdf(vec3 p) {
				return c2_tree(p * 6.f) / 6.f;
			}
		)";
		break;
	case sdf::shape::TEAPOT:
		txt = R"(
			float sdf(vec3 p) {
				return tp_tree(p * 1.f) / 1.f;
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