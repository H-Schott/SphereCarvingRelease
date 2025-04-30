#include "window.h"


int main() {
	Window* window = new Window("Terrain Viewer", 1000, 500);
	
	window->ResetCamera();

	while (!window->Exit()) {
		window->Update();
	}

	return 0;
}
