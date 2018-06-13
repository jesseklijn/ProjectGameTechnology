#pragma once
#include "Lighting.h"
#include <irrlicht.h>
#include <vector3d.h>
#include <SMeshBufferTangents.h>
#include "GridMesh.h"
#include "Grid.h"

/// Create device for unit testing purposes
irr::IrrlichtDevice* deviceUnitTest =
createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(1920, 1080), 64,
	false, true, false, 0);

int xWidth, yHeight, zLength;

// Initialize Irrlicht components
irr::video::IVideoDriver* driverUnitTest = deviceUnitTest->getVideoDriver();

irr::core::array<irr::u16> DrawTriangles(int xSizeGrid, int ySizeGrid)
{

	// Check if the x and y size are higher than 0
	if (xSizeGrid < 0 || ySizeGrid < 0)
	{
		// Return nothing if one of the parameters are 0 or negative 
		return NULL;
	}

	SMeshBuffer* bufferMesh = new irr::scene::SMeshBuffer();

	// Create triangles for the mesh
	for (int y = 0, ri = 0; y < ySizeGrid; y++, ri += xSizeGrid)
	{
		for (int x = 0; x < xSizeGrid; x++)
		{
			// Draws the first triangle
			bufferMesh->Indices.push_back(x + ri + y);
			bufferMesh->Indices.push_back(xSizeGrid + ri + x + 1 + y);
			bufferMesh->Indices.push_back(x + ri + 1 + y);

			// Draws the second triangle (to make a quad)
			bufferMesh->Indices.push_back(x + ri + 1 + y);
			bufferMesh->Indices.push_back(xSizeGrid + ri + x + 1 + y);
			bufferMesh->Indices.push_back(xSizeGrid + ri + x + 2 + y);
		};
	}

	return bufferMesh->Indices;
}

video::SColor GetPixelColor(video::ITexture* texture, int x, int y)
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
	IImage* image = driverUnitTest->createImage(texture, vector2d<s32>(0, 0), texture->getOriginalSize());
	pixel = image->getPixel(x, y);

	// Drop the image since its not being used anymore
	image->drop();

	return pixel;
}

bool GenerateGridSize(int x, int y, int z)
{
	// Assign the grid size for the vertices to be generated
	xWidth = x, yHeight = y, zLength = z;

	if (xWidth >= 0 && yHeight >= 0 && zLength >= 0) {
		return true;
	}
	else
	{
		return false;
	}
}