#include <vector>
#include "geometry.h"
#include "sdf.h"

class SphereCarving {
private:

    static constexpr float gr = 1.618034;
    static constexpr glm::vec3 initial_points[12] = {
        glm::vec3(0, 1, gr),
        glm::vec3(0, 1, -gr),
        glm::vec3(0, -1, gr),
        glm::vec3(0, -1, -gr),

        glm::vec3(gr, 0, 1),
        glm::vec3(gr, 0, -1),
        glm::vec3(-gr, 0, 1),
        glm::vec3(-gr, 0, -1),

        glm::vec3(1, gr, 0),
        glm::vec3(1, -gr, 0),
        glm::vec3(-1, gr, 0),
        glm::vec3(-1, -gr, 0)
    };
    glm::vec4 initial_sphere;

    std::vector<glm::vec4> sphere_set;
    std::vector<glm::vec3> point_set;

    sdf::shape sdf_shape;
public:
    SphereCarving(const sdf::shape& sdf_shape_);

    void Iterate();
};
