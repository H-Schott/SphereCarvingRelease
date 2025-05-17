#include <glm/glm.hpp>
#include <string>
#include <array>

namespace sdf {
	enum class shape {SPHERE, SPHERES, BOX, CYLINDER, CAD};

	static const std::array<std::string, 5> shape_name_list = { "Sphere",
																"Spheres",
																"Box",
																"Cylinder",
																"CAD" };

	float d_sphere(const glm::vec3& p, const float& r);
	float d_box(const glm::vec3& p, const glm::vec3& size);
	float d_cylinder(glm::vec3 p, float h, float r);

	float evaluation(const sdf::shape& s, const glm::vec3& p);
	std::string glsl_txt(const sdf::shape& s);
}

