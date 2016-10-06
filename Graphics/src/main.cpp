#include <MyApplication.h>
#include <IntroToOpenGL.h>
#include <RenderingGeometry.h>

int main()
{
	BaseApplication * app;

	app = new RenderingGeometry();
	if (app->startup())
		app->run();

	app->shutdown();
	delete app;

	return false;
}
