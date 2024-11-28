#include "ofApp.h"

#include "App.hpp"
#include "types.hpp"

#include <string>

static App::Key AppKeyFromOFKey(const i32 key)
{
	switch (key) {
	case 's': {
		return App::Key::KeyS;
	} break;
	case 'w': {
		return App::Key::KeyW;
	} break;
	case 'a': {
		return App::Key::KeyA;
	} break;
	case 'd': {
		return App::Key::KeyD;
	} break;

		/*case 0 * Down: {*/
		/*	return App::Key::KeyDown;*/
		/*} break;*/
		/*case 0 * Up: {*/
		/*	return App::Key::KeyUp;*/
		/*} break;*/
		/*case 0 * Left: {*/
		/*	return App::Key::KeyLeft;*/
		/*} break;*/
		/*case 0 * R: {*/
		/*	return App::Key::KeyRight;*/
		/*} break;*/

	case '1': {
		return App::Key::Key1;
	} break;
	case '2': {
		return App::Key::Key2;
	} break;
	case '3': {
		return App::Key::Key3;
	} break;
	case 'u': {
		return App::Key::KeyU;
	} break;
	case '4': {
		return App::Key::Key4;
	} break;
	case 'i': {
		return App::Key::KeyI;
	} break;

	case 'c': {
		return App::Key::KeyC;
	}; break;
	case 't': {
		return App::Key::KeyT;
	} break;
	case 'v': {
		return App::Key::KeyV;
	} break;
	case 'm': {
		return App::Key::KeyM;
	} break;
	case 'r': {
		return App::Key::KeyR;
	} break;
	case 'x': {
		return App::Key::KeyX;
	} break;
	case 'l': {
		return App::Key::KeyL;
	} break;
	case 'k': {
		return App::Key::KeyK;
	} break;
	case 'j': {
		return App::Key::KeyJ;
	} break;
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
