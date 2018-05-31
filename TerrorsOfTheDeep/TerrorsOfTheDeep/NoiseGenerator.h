#pragma once
#include <irrlicht.h>
#include <vector>
#include <module/perlin.h>

class NoiseGenerator
{
public:
	NoiseGenerator();
	~NoiseGenerator();

	// Generate a heightmap (noise map) to be used for level generation by using the libnoise library. 
	// It outputs an image in the project folder (The fileDestName determines where it outputs).
	void GenerateHeightMap(const std::string fileDestName, int imageSizeX = 256, int imageSizeY = 256);

	// Get the pixel color of the texture. This is currently used for heightmapping to get the pixel color of the noise map.
	// The pixel color determines the height.
	irr::video::SColor getPixelColor(irr::video::ITexture* texture, irr::video::IImage* image, int x = 0, int y = 0);

	// Sizes of the image to make it usable for outside uses (outside the function)
	int xSizeImage, ySizeImage;
};

