#include "box.h"

Box::Box(const glm::vec3& a, const glm::vec3& b) {
	bmin = glm::min(a, b);
	bmax = glm::max(a, b);
}

Box::Box(const glm::vec3& c, const float& l) {
	bmin = c - glm::vec3(l / 2.f);
	bmin = c + glm::vec3(l / 2.f);
}



// -------- Box2D ------------

Box2D::Box2D(const glm::vec2& a, const glm::vec2& b) {
	bmin = glm::min(a, b);
	bmax = glm::max(a, b);
}

Box2D::Box2D(const glm::vec2& c, const float& l) {
	bmin = c - glm::vec2(l / 2.f);
	bmin = c + glm::vec2(l / 2.f);
}