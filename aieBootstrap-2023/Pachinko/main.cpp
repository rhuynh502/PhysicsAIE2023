#include "PachinkoApp.h"

int main() {
	
	// allocation
	auto app = new PachinkoApp();

	// initialise and loop
	app->run("Pachinko: Gamba Simulator", 540, 960, false);

	// deallocation
	delete app;

	return 0;
}