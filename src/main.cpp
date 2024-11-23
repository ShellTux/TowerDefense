#include "App.hpp"

int main()
{
	App app("Tower Defense", 600, 600, 20, 20, 10);
	app.loop();

	return 0;
}
