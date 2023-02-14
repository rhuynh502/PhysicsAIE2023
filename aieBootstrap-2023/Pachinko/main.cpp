#include "PachinkoApp.h"

int main() {
	
	// allocation
	auto app = new PachinkoApp();

	// initialise and loop
	app->run("AIE", 540, 960, false);

	// deallocation
	delete app;

	return 0;
}