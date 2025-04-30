#include <glm/glm.hpp>

// axis aligned box

class Box {
public:
	glm::vec3 bmin = glm::vec3(-1);
	glm::vec3 bmax = glm::vec3(1);

public:
	Box() {};
	Box(const glm::vec3& a, const glm::vec3& b);
	Box(const glm::vec3& c, const float& l);
};


class Box2D {
public:
	glm::vec2 bmin = glm::vec2(-1);
	glm::vec2 bmax = glm::vec2(1);

public:
	Box2D() {};
	Box2D(const glm::vec2& a, const glm::vec2& b);
	Box2D(const glm::vec2& c, const float& l);
};
