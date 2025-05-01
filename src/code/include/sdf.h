#include <glm/glm.hpp>
#include <string>
#include <array>

namespace sdf {
	enum class shape {SPHERE, BOX, CYLINDER, CAD};
	static const std::array<sdf::shape, 4> shape_list = { sdf::shape::SPHERE,
														  sdf::shape::BOX,
														  sdf::shape::CYLINDER,
														  sdf::shape::CAD };
	static const std::array<std::string, 4> shape_name_list = { "Sphere",
																"Box",
																"Cylinder",
																"CAD" };

	float d_sphere(const glm::vec3& p, const float& r);
	float d_box(const glm::vec3& p, const glm::vec3& size);
	float d_cylinder(glm::vec3 p, float h, float r);

	float evaluation(const sdf::shape& s, const glm::vec3& p);
	std::string glsl_txt(const sdf::shape& s);
}

