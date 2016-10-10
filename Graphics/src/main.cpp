#include <MyApplication.h>
#include <IntroToOpenGL.h>
#include <RenderingGeometry.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyObjLoader.h>
#include <CamerasAndProjections.h>
#include <DirectLighting.h>
#include <Texturing.h>
#include <ProceduralGeneration.h>

int main()
{
	BaseApplication * app;

	app = new ProceduralGeneration();
	if (app->startup())
		app->run();

	app->shutdown();
	delete app;

	return false;
}
