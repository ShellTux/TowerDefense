#include "ofApp.h"

#include "App.hpp"
#include "types.hpp"

#include <string>

static App::Key AppKeyFromOFKey(const i32 key)
{
	switch (key) {
#define KEY(ENUM, GLFW_KEY, OF_KEY) \
	case OF_KEY:                \
		return App::Key::ENUM;
		KEYS_OF_IMPLEMENTED
#undef KEY
	}

	return App::Key::KeyNone;
}

void ofApp::initApp(const std::string &title,
                    const u32 fieldRows,
                    const u32 fieldCols,
                    const u8 fieldWaves)
{
	app = App(title,
	          ofGetWidth(),
	          ofGetHeight(),
	          fieldRows,
	          fieldCols,
	          fieldWaves);
}

void ofApp::setup()
{
	app.setup();
}

void ofApp::update()
{
	app.updateDimensions(ofGetWidth(), ofGetHeight());
	app.update();
}

void ofApp::draw()
{
	app.draw();
}

void ofApp::keyPressed(int key)
{
	switch (AppKeyFromOFKey(key)) {
	case App::Key::KeyNone:
		return;
	default: {
		app.KeyPress(AppKeyFromOFKey(key));
	} break;
	}
}

void ofApp::keyReleased(int key)
{
	switch (AppKeyFromOFKey(key)) {
	case App::Key::KeyNone:
		return;
	default: {
		app.KeyRelease(AppKeyFromOFKey(key));
	} break;
	}
}

void ofApp::mouseMoved(int x, int y)
{
	(void) x;
	(void) y;
}

void ofApp::mouseDragged(int x, int y, int button)
{
	(void) x;
	(void) y;
	(void) button;
}

void ofApp::mousePressed(int x, int y, int button)
{
	(void) x;
	(void) y;
	(void) button;
}

void ofApp::mouseReleased(int x, int y, int button)
{
	(void) x;
	(void) y;
	(void) button;
}

void ofApp::mouseEntered(int x, int y)
{
	(void) x;
	(void) y;
}

void ofApp::mouseExited(int x, int y)
{
	(void) x;
	(void) y;
}

void ofApp::mouseScrolled(ofMouseEventArgs &mouseEvent)
{
	app.MouseScroll(mouseEvent.scrollX, mouseEvent.scrollY);
}

void ofApp::windowResized(int w, int h)
{
	(void) w;
	(void) h;
}

void ofApp::gotMessage(ofMessage msg)
{
	(void) msg;
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
	(void) dragInfo;
}
