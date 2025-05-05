#include <vector>
#include "geometry.h"
#include "sdf.h"

class SphereCarving {
private:

    static constexpr float gr = 1.618034f;
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
    glm::vec4 initial_sphere = glm::vec4(0, 0, 0, 50);

    std::vector<glm::vec4> sphere_set;
    std::vector<glm::vec3> point_set;
    std::vector<glm::vec4> convex_hull;

    sdf::shape sdf_shape;

    float min_sphere_radius = 0.05f;

    bool ValidIntersectionCheck(const glm::vec3& p, const glm::ivec3& spheres_id) const;
    void SphereSetIntersections();
public:
    SphereCarving(const sdf::shape& sdf_shape_);
    
    void Iterate();
    void* GetSphereData() const { return (void*)sphere_set.data(); };
    glm::vec4 GetInitialSphere() const { return initial_sphere; };
    int GetSpheresetSize() const { return int(sphere_set.size()); };
    const std::vector<glm::vec3>& GetPointsetData() const { return point_set; };
    int GetPointsetSize() const { return int(point_set.size()); };
    void* GetConvexHullData() const { return (void*)convex_hull.data(); };
    int GetConvexHullSize() const { return int(convex_hull.size()); };
};
