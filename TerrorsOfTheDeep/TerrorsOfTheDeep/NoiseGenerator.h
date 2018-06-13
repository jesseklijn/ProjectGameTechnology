#pragma once
#include <irrlicht.h>
#include <vector>

class NoiseGenerator
{
public:
	NoiseGenerator();
	~NoiseGenerator();

	/// Generate a heightmap (noise map) to be used for level generation by using the libnoise library. 
	/// It outputs an image in the project folder (The fileDestName determines where it outputs).
	bool GenerateHeightMap(const std::string fileDestName, int imageSizeX, int imageSizeY);

	/// Get the pixel color of the texture. It returns a SColor value to the caller. 
	irr::video::SColor GetPixelColor(irr::video::ITexture* texture, int x = 0, int y = 0);

	/// Sizes of the image to make it usable for outside uses
	int xSizeImage, ySizeImage;
};

