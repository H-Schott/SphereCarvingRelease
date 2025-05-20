#include <glm/glm.hpp>
#include <string>
#include <array>

namespace sdf {
	enum class shape {SPHERE, SPHERES, BOX, CYLINDER, CAD_1, CAD_2, TEAPOT};

	static const std::array<std::string, 7> shape_name_list = { "Sphere",
																"Spheres",
																"Box",
																"Cylinder",
																"CAD 1",
																"CAD 2",
																"Teapot"};

	float d_sphere(const glm::vec3& p, const float& r);
	float d_box(const glm::vec3& p, const glm::vec3& size);
	float d_cylinder(const glm::vec3& p, float h, float r);
	float d_cylinder(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, float r);
	float d_cylinder(const glm::vec3& p, const glm::vec3& a, const glm::vec3& axis, float r, float l);
	float d_torus(glm::vec3 p, glm::vec3 c, glm::vec3 axis, float r, float s);
	float d_smoothUnion(float d1, float d2, float r);

	float evaluation(const sdf::shape& s, const glm::vec3& p);
	std::string glsl_txt(const sdf::shape& s);
}

