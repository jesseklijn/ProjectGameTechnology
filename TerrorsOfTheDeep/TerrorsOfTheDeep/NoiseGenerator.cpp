#pragma once
#include <noise/noise.h>
#include "noiseutils.h"
#include "NoiseGenerator.h"
#include <iostream>
#include "GameManager.h"


#pragma comment (lib, "libnoise.lib")

using namespace noise;
using namespace irr;

NoiseGenerator::NoiseGenerator()
{
}


NoiseGenerator::~NoiseGenerator()
{
}
// Generate a noise map to be used for the level generation. imageSizeX and imageSizeY defines the size of the noise map.
// fileDestName determines the noise map name and where the noise map will save.
 void NoiseGenerator::GenerateHeightMap(const std::string fileDestName, int imageSizeX, int imageSizeY)
{
	// Set image size to the value of the parameter to be used outside the function
	xSizeImage = imageSizeX;
	ySizeImage = imageSizeY;

	// Create noise modules and required items to create the noise map
	module::Perlin* myModule = new module::Perlin;
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;

	myModule->SetFrequency(1.0); // Defines the strength of the noise
	heightMapBuilder.SetSourceModule(*myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);

	// Set noise map size and the boundaries of the noise map
	heightMapBuilder.SetDestSize(xSizeImage, ySizeImage);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	// Render the image for the output of the noise map
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	// Output the noise map to the destination filepath 
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename(fileDestName);
	writer.WriteDestFile();
}

// Get the color of the pixel in the selected texture
video::SColor NoiseGenerator::getPixelColor(video::ITexture* texture, int x, int y)
 {
	// Set the default color of the pixel 
	video::SColor pixel = video::SColor(0, 0, 0, 0);

	// Does the texture exist?
	 if (texture == nullptr)
		 return pixel;

	// Checks if the X and Y are positive so they can be used for the image
	 if (x < 0 || y < 0) 
	 {
		 return pixel;
	 }

	 // Is the given X and Y within the image size?
	 if (x > texture->getSize().Width || y > texture->getSize().Height)
	 {
		 return pixel;
	 }

	// Convert ITexture into IImage to make use of the irrlicht pixel picker function
	  IImage* image = GameManager::driver->createImage(texture, vector2d<s32>(0, 0), texture->getSize());
	  pixel = image->getPixel(x, y);

	 return pixel;
 }


