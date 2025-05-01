#include "spherecarving.h"
#include <iostream>

SphereCarving::SphereCarving(const sdf::shape& sdf_shape_) : sdf_shape(sdf_shape_) {

	// scale the initial_points on the initial_sphere
	point_set.resize(12);
	for (int i = 0; i < 12; i++) {
		point_set[i] = glm::vec3(initial_sphere) + glm::normalize(initial_points[i]) * initial_sphere.w;
	}
}


bool SphereCarving::ValidIntersectionCheck(const glm::vec3& p, const glm::ivec3& spheres_id) const {
	if (!geo::sphere_inclusion_check(initial_sphere, p)) return false;

	for (int i = 0; i < sphere_set.size(); i++) {
		if (i == spheres_id.x || i == spheres_id.y || i == spheres_id.z) continue;
		if (geo::sphere_inclusion_check(sphere_set[i], p)) return false;
	}

	return true;
}

void SphereCarving::SphereSetIntersections() {
	for (int i = 0; i < sphere_set.size(); i++) {

		const glm::vec4& sA = sphere_set[i];

		for (int j = i + 1; j < sphere_set.size(); j++) {

			const glm::vec4& sB = sphere_set[j];
			bool check_ij = geo::sphere_intersection_check(sA, sB);
			if (!check_ij) continue;

			for (int k = j + 1; k < sphere_set.size(); k++) {

				const glm::vec4& sC = sphere_set[k];
				bool check_ik = geo::sphere_intersection_check(sA, sC);
				if (!check_ik) continue;
				bool check_jk = geo::sphere_intersection_check(sB, sC);
				if (!check_jk) continue;

				std::cout << i << " " << j << " " << k << std::endl;
				
				glm::vec3 p1, p2;
				bool inter = geo::sphere_intersection(sA, sB, sC, p1, p2);

				if (!inter) continue;


				// check if intersections are valid
				if (ValidIntersectionCheck(p1, glm::ivec3(i, j, k))) point_set.push_back(p1);
				if (ValidIntersectionCheck(p2, glm::ivec3(i, j, k))) point_set.push_back(p2);
			}
		}
	}
}

void SphereCarving::Iterate() {

	// evaluate on the point_set to augment the sphere_set
	for (int i = 0; i < point_set.size(); i++) {
		const glm::vec3& p = point_set[i];
		sphere_set.push_back(glm::vec4(p, sdf::evaluation(sdf_shape, p)));
	}

	// replace the point_set with every valid intersection of the sphere_set
	point_set.clear();
	SphereSetIntersections();
}