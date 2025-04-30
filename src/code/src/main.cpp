#include "window.h"
#include "geometry.h"

int main() {
	Window* window = new Window("Sphere Carving", 1000, 500);
	
	window->ResetCamera();

	while (!window->Exit()) {
		window->Update();
	}

	return 0;
}
