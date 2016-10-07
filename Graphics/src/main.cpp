#include <MyApplication.h>
#include <IntroToOpenGL.h>
#include <RenderingGeometry.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyObjLoader.h>

int main()
{
	BaseApplication * app;

	app = new TinyObjLoader();
	if (app->startup())
		app->run();

	app->shutdown();
	delete app;

	return false;
}
