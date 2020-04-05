#include "new_Application.h"

int main(int argc, char** argv) {
	Application& application = Application::getInstance();
	application.init(argc, argv);
	application.run();
	application.cleanup();
}