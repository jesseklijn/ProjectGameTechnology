#pragma once
#include <irrlicht.h>
#include <vector>

/// NoiseGenerator class makes use of the LibNoise library to generate its noise map.
/// This allows the programmer to make use of the functions of LibNoise.
/// Look at the GenerateHeightMap function for more information on how it works
class NoiseGenerator
{
public:
	NoiseGenerator();
	~NoiseGenerator();

	/// Generate a heightmap (noise map) to be used for level generation by using the libnoise library. 
	/// It outputs an image in the project folder (The fileDestName determines where it outputs).
	bool GenerateHeightMap(const std::string fileDestName, int imageSizeX, int imageSizeY);

	/// Get the pixel color of the texture. It returns a SColor value to the caller. 
	irr::video::SColor GetPixelColor(irr::video::ITexture* texture, int x, int y);

	/// Sizes of the image to make it usable for outside uses
	int xSizeImage, ySizeImage;
};

