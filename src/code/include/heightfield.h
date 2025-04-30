#include "box.h"
#include <vector>
#include <string>

class HeightField {
public:
	int size_x = 0;
	int size_y = 0;
	Box2D box;
	std::vector<double> heights;

public:
	HeightField() {};
	HeightField(const Box2D& b, const std::string& filename, const double& hmin, const double& hmax);

	glm::ivec2 GetSize() const { return glm::ivec2(size_x, size_y); };
	int GetSizeX() const { return size_x; };
	int GetSizeY() const { return size_y; };
	int GetTotalSize() const { return size_x * size_y; };

	Box2D GetBox() const { return box; };
	glm::vec2 GetRange() const;
	double GetLipschitz() const;
	std::vector<float> GetFloats() const;

	void Save(const std::string& filename) const;
};