#pragma once
#include <noise.h>
#include "noiseutils.h"
#include "NoiseGenerator.h"
#include <iostream>
#include "GameManager.h"


#pragma comment (lib, "libnoise.lib")

using namespace noise;
using namespace irr;
using namespace module;
using namespace utils;

NoiseGenerator::NoiseGenerator()
{
}


NoiseGenerator::~NoiseGenerator()
{
}

/// Generate a noise map to be used for the level generation. 
/// <br/><b>Parameters</b><br/>
/// imageSizeX and imageSizeY - The size of the noise map<br/>
/// fileDestName - Tells the function where the noise map should be saved and what the noise map is named.<br/>
/// <b>Usage</b><br/>
/// A new file will be created after running the program once. You can find the heightmap back in the fileDestName. <br/>
/// A noise map has many usages such as heightmapping and creating textures.<br/>
/// <b>How does it work?</b><br/>
/// First a Perlin module has to be made to create perlin noise. Here you can modify what kind of noise you want to create (I.E. you can set the frequency of the noise). <br/>
/// This will be passed on to the NoiseMapBuilderPlane where the noise map will be build.<br/>
/// After that a renderer module is used to render this build into an image. The writer is used to output the image which will create an image to the given file path.<br/>
/// <b>Output</b><br/>
/// A noise map that has generated and placed in the "fileDestName" path.

 bool NoiseGenerator::GenerateHeightMap(const std::string fileDestName, int imageSizeX, int imageSizeY)
{

	 // Is the fileDestName given in the parameter?
	 if (fileDestName == "")
	 {
		 return false;
	 }

	 // Checks if the X and Y are positive since the image size must be a positive and higher than 0
	 if (imageSizeX <= 0 || imageSizeY <= 0)
	 {
		 return false;
	 }

	// Set image size to the value of the parameter to be used outside the function
	xSizeImage = imageSizeX;
	ySizeImage = imageSizeY;

	// Create noise modules and required items to create the noise map
	Billow billow;
	RidgedMulti mountainTerrain;
	Billow baseFlatTerrain;
	ScaleBias flatTerrain;
	Perlin terrainType;
	Select terrainSelector;
	Turbulence finalTerrain;
	NoiseMap heightMap;
	NoiseMapBuilderPlane heightMapBuilder;

	billow.SetSeed(GameManager::gameSeed);
	mountainTerrain.SetSeed(GameManager::gameSeed);
	baseFlatTerrain.SetSeed(GameManager::gameSeed);
	terrainType.SetSeed(GameManager::gameSeed);
	finalTerrain.SetSeed(GameManager::gameSeed);

	baseFlatTerrain.SetFrequency(1.0);
	flatTerrain.SetSourceModule(0, baseFlatTerrain);
	flatTerrain.SetScale(0.25);
	flatTerrain.SetBias(-0.85);

	terrainType.SetFrequency(0.25);
	terrainType.SetPersistence(0.1);

	mountainTerrain.SetFrequency(0.2f);

	terrainSelector.SetSourceModule(0, flatTerrain);
	terrainSelector.SetSourceModule(1, mountainTerrain);
	terrainSelector.SetControlModule(terrainType);
	terrainSelector.SetBounds(0.0, 1000.0);
	terrainSelector.SetEdgeFalloff(0.2);

	finalTerrain.SetSourceModule(0, terrainSelector);
	finalTerrain.SetFrequency(1.0);
	finalTerrain.SetPower(0.1);

	heightMapBuilder.SetSourceModule(finalTerrain);
	heightMapBuilder.SetDestNoiseMap(heightMap);

	// Set noise map size and the boundaries of the noise map and also build the noise map
	heightMapBuilder.SetDestSize(xSizeImage, ySizeImage);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	// Render the image from the build 
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

	// Heightmap has successfully generated
	return true;
}

/// Get the color of the pixel in the selected texture. 
/// <br/><b>Parameters</b><br/>
/// Texture - Texture of the object you want to get the pixel color from (this is used for determining the dimensions of the texture).<br/>
/// Image - Image of the texture (this is used for determining the dimensions of the texture). <br/>
/// X and Y - Coordinates of the pixel. <br/>
/// <b>Usage</b><br/>
/// This function is currently used for heightmapping where the value determines the vertex's height. <br/>
/// <b>How does it work?</b><br/>
/// The default pixel color is black if a parameter is missing or not correct so it will return a black color if this is the case. <br/>
/// After the checks, the function will use the texture to get the original size (incase if it's scaled somewhere else). <br/>
/// The color of the pixel of the given coordinates will be calculated by using an irrlicht built-in function "getPixel". This value will be returned to the caller <br/>
/// <b>Output</b><br/>
/// A SColor value of the given image of the given pixel
video::SColor NoiseGenerator::GetPixelColor(video::ITexture* texture, int x, int y)
 {

	// Does the texture exist?
	if (texture == nullptr)
	{
		return NULL;
	}

	 // Checks if the X and Y are positive so they can be used for the image
	 if (x < 0 || y < 0)
	 {
		 return NULL;
	 }

	 // Set the default color of the pixel 
	 video::SColor pixel = video::SColor(0, 0, 0, 0);

	 // Get the original size of the texture (in case if it's scaled)
	 dimension2du textureDimensions = texture->getOriginalSize();

	 // Is the given X and Y within the image size?
	 if (x > textureDimensions.Width || y > textureDimensions.Height)
	 {
		 return pixel;
	 }
	 IImage* image = GameManager::driver->createImage(texture, vector2d<s32>(0, 0), texture->getOriginalSize());
	 pixel = image->getPixel(x, y);

	// Drop the image since its not being used anymore
	 image->drop();

	 return pixel;
 }


