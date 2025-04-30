#include <glm/glm.hpp>
#include <string>

namespace sdf {
	enum class shape {SPHERE, BOX};

	float d_sphere(const glm::vec3& p, const float& r);
	float d_box(const glm::vec3& p, const glm::vec3& size);

	float evaluation(const sdf::shape& s, const glm::vec3& p);
	std::string glsl_txt(const sdf::shape& s);
}

