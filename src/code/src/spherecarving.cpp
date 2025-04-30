#include "spherecarving.h"

SphereCarving::SphereCarving(const sdf::shape& sdf_shape_) : sdf_shape(sdf_shape_) {

	// scale the initial_points on the initial_sphere
	point_set.resize(12);
	for (int i = 0; i < 12; i++) {
		point_set[i] = glm::vec3(initial_sphere) + glm::normalize(initial_points[i]) * initial_sphere.w;
	}
}

void SphereCarving::Iterate() {

	// evaluate on the point_set to augment the sphere_set
	for (int i = 0; i < point_set.size(); i++) {
		const glm::vec3& p = point_set[i];
		sphere_set.push_back(glm::vec4(p, sdf::evaluation(sdf_shape, p)));
	}

	// replace the point_set with every valid intersection of the sphere_set

}