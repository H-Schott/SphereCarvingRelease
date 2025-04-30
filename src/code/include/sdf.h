#include <glm/glm.hpp>
#include <string>

namespace sdf {
	enum class shape {SPHERE, CUBE};

	float d_sphere(const glm::vec3& p, const float& r);
	float d_box(const glm::vec3& p, const glm::vec3& size);

	float evaluation(shape s, const glm::vec3& p);
	std::string glsl_txt(shape s);
}

