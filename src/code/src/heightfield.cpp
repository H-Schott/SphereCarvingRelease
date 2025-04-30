#include "heightfield.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "write_16_png.h"

#include <iostream>

glm::vec2 HeightField::GetRange() const {

	double hmin = heights[0];
	double hmax = heights[0];

	for (int i = 0; i < size_x * size_y; i++) {
		hmin = std::min(hmin, heights[i]);
		hmax = std::max(hmax, heights[i]);
	}

	return glm::vec2(hmin, hmax);
}

double HeightField::GetLipschitz() const {
	return 1.;
}

std::vector<float> HeightField::GetFloats() const {
	return { heights.begin(), heights.end() };
}

HeightField::HeightField(const Box2D& b, const std::string& filename, const double& hmin, const double& hmax) {
	box = b;

	// read file
	int nb_chan;
	unsigned short* img = stbi_load_16(filename.c_str(), &size_x, &size_y, &nb_chan, STBI_grey);

	for (int i = 0; i < size_x * size_y; i++) {
		int c_val = img[i];// 256 * img[4 * i] + img[4 * i + 1];
		double d_val = double(c_val) / 65535.;
		heights.push_back(hmin + d_val * (hmax - hmin));
	}

	stbi_image_free(img);
}

void HeightField::Save(const std::string& filename) const {
	
	glm::vec2 range = GetRange();
	
	std::vector<unsigned char> img;

	for (int i = 0; i < size_x * size_y; i++) {
		double d_val = heights[i];
		d_val = (d_val - range.x) / (range.y - range.x);
		unsigned short us_val = unsigned short(d_val * 65535.);

		img.push_back((unsigned char)(us_val >> 8));
		img.push_back((unsigned char)(us_val & 0xFF));
	}

	write_16_png(filename.c_str(), img, size_x, size_y);
}
