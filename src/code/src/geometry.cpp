#include "geometry.h"
#define CONVHULL_3D_ENABLE
#define CONVHULL_3D_USE_SINGLE_PRECISION
#include "convhull_3d.h"

#include <iostream>

bool geo::sphere_inclusion_check(const glm::vec4& s, const glm::vec3& p) {
	return glm::length(p - glm::vec3(s)) <= s.w;
}

bool geo::sphere_intersection_check(const glm::vec4& sA, const glm::vec4& sB) {
	float d = glm::length(glm::vec3(sA) - glm::vec3(sB));
	return (d < sA.w + sB.w) && (d > std::abs(sA.w - sB.w));
}

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

	float x = (sA.w * sA.w - sB.w * sB.w + a * a) / (2.f * a);
	float y = (sB.w * sB.w - sC.w * sC.w - (x - a) * (x - a) + (x - b) * (x - b) + c * c) / (2.f * c);
	float z2 = sA.w * sA.w - x * x - y * y;

	if (z2 < 0.f) return false;

	float z = std::sqrt(z2);

	// return in global frame
	p1 = A + x * ex + y * ey + z * ez;
	p2 = A + x * ex + y * ey - z * ez;
	return true;
}

std::vector<glm::vec4> geo::convex_hull(const std::vector<glm::vec3>& pointset) {

	srand(0);

	std::vector<ch_vertex> vertices;
	vertices.resize(pointset.size());
	for (int i = 0; i < pointset.size(); i++) {

		vertices[i].x = pointset[i].x + float(rand()) / float(RAND_MAX) * 0.02f - 0.01f;
		vertices[i].y = pointset[i].y + float(rand()) / float(RAND_MAX) * 0.02f - 0.01f;
		vertices[i].z = pointset[i].z + float(rand()) / float(RAND_MAX) * 0.02f - 0.01f;
	}

	int* faces = nullptr;
	int nb_faces;
	convhull_3d_build(vertices.data(), int(vertices.size()), &faces, &nb_faces);


	std::vector<glm::vec4> hplanes;
	
	for (int i = 0; i < nb_faces; i++) {
		
		ch_vertex cvA = vertices[faces[3 * i]];
		ch_vertex cvB = vertices[faces[3 * i + 1]];
		ch_vertex cvC = vertices[faces[3 * i + 2]];

		glm::vec3 A = glm::vec3(cvA.x, cvA.y, cvA.z);
		glm::vec3 B = glm::vec3(cvB.x, cvB.y, cvB.z);
		glm::vec3 C = glm::vec3(cvC.x, cvC.y, cvC.z);

		glm::vec3 n = glm::normalize(glm::cross(B - A, C - A));
		float h = glm::dot(A, n);

		hplanes.push_back(glm::vec4(n, h));

	}

	free(faces);

	return hplanes;
}






