#pragma once

// clang-format off
#include "ofMain.h"
// clang-format on

#include "App.hpp"
#include "types.hpp"

class ofApp : public ofBaseApp {
      public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void mouseScrolled(ofMouseEventArgs &mouseEvent);

	void initApp(const std::string &title,
	             const u32 fieldRows,
	             const u32 fieldCols,
	             const u8 fieldWaves);

	static constexpr char windowClass[] = "OpenGL";

      private:
	App app;
};
