#include "types.hpp"

static constexpr u64 width  = 600;
static constexpr u64 height = 600;

#ifdef NOOF
	#include "App.hpp"

int main()
{
	App app("Tower Defense", width, height, 10, 10, 10);
	app.loop();

	return 0;
}
#else
	#include "ofApp.h"
	#include "ofMain.h"

int main()
{
	ofGLWindowSettings settings;
	settings.setSize(width, height);
	settings.windowMode = OF_WINDOW;

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();
}
#endif
