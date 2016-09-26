#include <MyApplication.h>

int main()
{
	BaseApplication * app;

	app = new MyApplication();
	if (app->startup())
		app->run();

	app->shutdown();
	delete app;

	return false;
}
