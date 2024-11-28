#include "ofApp.h"

#include "App.hpp"
#include "types.hpp"

#include <string>

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
	switch (key) {
	case 's': {
		app.KeyPress(App::Key::KeyS);
	} break;
	case 'w': {
		app.KeyPress(App::Key::KeyW);
	} break;
	case 'a': {
		app.KeyPress(App::Key::KeyA);
	} break;
	case 'd': {
		app.KeyPress(App::Key::KeyD);
	} break;

		/*case 0 * Down: {*/
		/*	app.KeyPress(App::Key::KeyDown);*/
		/*} break;*/
		/*case 0 * Up: {*/
		/*	app.KeyPress(App::Key::KeyUp);*/
		/*} break;*/
		/*case 0 * Left: {*/
		/*	app.KeyPress(App::Key::KeyLeft);*/
		/*} break;*/
		/*case 0 * R: {*/
		/*	app.KeyPress(App::Key::KeyRight);*/
		/*} break;*/

	case '1': {
		app.KeyPress(App::Key::Key1);
	} break;
	case '2': {
		app.KeyPress(App::Key::Key2);
	} break;
	case '3': {
		app.KeyPress(App::Key::Key3);
	} break;
	case 'U': {
		app.KeyPress(App::Key::KeyU);
	} break;
	case '4': {
		app.KeyPress(App::Key::Key4);
	} break;
	case 'I': {
		app.KeyPress(App::Key::KeyI);
	} break;

	case 'C': {
		app.KeyPress(App::Key::KeyC);
	}; break;
	case 'T': {
		app.KeyPress(App::Key::KeyT);
	} break;
	case 'V': {
		app.KeyPress(App::Key::KeyV);
	} break;
	case 'M': {
		app.KeyPress(App::Key::KeyM);
	} break;
	case 'R': {
		app.KeyPress(App::Key::KeyR);
	} break;
	case 'X': {
		app.KeyPress(App::Key::KeyX);
	} break;
	case 'L': {
		app.KeyPress(App::Key::KeyL);
	} break;
	case 'K': {
		app.KeyPress(App::Key::KeyK);
	} break;
	case 'J': {
		app.KeyPress(App::Key::KeyJ);
	} break;
	}
}

void ofApp::keyReleased(int key)
{
	(void) key;
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
