#include <MyApplication.h>
#include <IntroToOpenGL.h>

int main()
{
	BaseApplication * app;

	app = new IntroToOpenGL();
	if (app->startup())
		app->run();

	app->shutdown();
	delete app;

	return false;
}
