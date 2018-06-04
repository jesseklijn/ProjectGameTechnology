#pragma once
#include <noise.h>
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

/// Generate a noise map to be used for the level generation. 
/// <br/><b>Parameters</b><br/>
/// imageSizeX and imageSizeY - The size of the noise map<br/>
/// fileDestName - Tells the function where the noise map should be saved and what the noise map is named.<br/>
/// <b>Usage</b><br/>
/// A new file will be created after running the program once. You can find the heightmap back in the fileDestName. <br/>
/// A noise map has many usages such as heightmapping and creating textures.<br/>
/// <b>How does it work?</b><br/>
/// First a Perlin module has to be made to create perlin noise. Here you can modify what kind of noise you want to create (IE you can set the frequency of the noise). <br/>
/// This will be passed on to the NoiseMapBuilderPlane where the noise map will be build.<br/>
/// After that a renderer module is used to render this build into an image. The writer is used to output the image which will create an image to the given file path.<br/>

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
}

/// Get the color of the pixel in the selected texture. 
/// <br/><b>Parameters</b><br/>
/// Texture - Texture of the object you want to get the pixel color from (this is used for determining the dimensions of the texture).<br/>
/// Image - Image of the texture (this is used for determining the dimensions of the texture) <br/>
/// X and Y - Coordinates of the pixel <br/>
/// <b>Usage</b><br/>
/// This function is currently used for heightmapping where the value determines the vertex's height. <br/>
/// <b>How does it work?</b><br/>
/// The default pixel color is black if a parameter is missing or not correct so it will return a black color if this is the case. <br/>
/// After the checks, the function will use the texture to get the original size (incase if it's scaled somewhere else). <br/>
/// The color of the pixel of the given coordinates will be calculated by using an irrlicht builtin function "getPixel". This value will be returned to the caller <br/>
video::SColor NoiseGenerator::getPixelColor(video::ITexture* texture, video::IImage* image, int x, int y)
 {
	// Set the default color of the pixel 
	video::SColor pixel = video::SColor(0, 0, 0, 0);

	// Does the texture exist?
	 if (texture == nullptr || image == nullptr)
		 return pixel;

	 dimension2du textureDimensions = texture->getOriginalSize();

	// Checks if the X and Y are positive so they can be used for the image
	 if (x < 0 || y < 0) 
	 {
		 return pixel;
	 }

	 // Is the given X and Y within the image size?
	 if (x > textureDimensions.Width || y > textureDimensions.Height)
	 {
		 return pixel;
	 }

	 return image->getPixel(x, y);
 }


