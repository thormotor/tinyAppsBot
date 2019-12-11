// controller_http.cpp : Defines the entry point for the console application.

#include <conio.h>
#include <vld.h>  // Visual Leak Detector library (https://github.com/KindDragon/vld)

#include "logger.h"
#include "main_controller.h"


int main()
{

#ifdef WIN32
	// enable russian codepage support
	setlocale(LC_ALL, "ru-RU");
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif  // WIN32
	
	logger::LoggerInit();  // initialize logging global evironment
	auto logit = spdlog::get(LOG_MAIN);
	logit->info("************************* application started ****************************");
	
	try {
		
		MainController main_controller;  // create main object to manage all others
		main_controller.Start();  // start network working threads

		// start infinite loop for listening to keyboard
		// ----------------------------------------------------------------------------------
		const int K_ESCAPE = 27;
		int input_char;
		while ((input_char=_getch()) != K_ESCAPE) {
			continue;
		}
		// ----------------------------------------------------------------------------------

		logit->info("TERMINATION command");
		main_controller.Stop();  // stop network working threads

	} catch (std::exception& e) {
		logit->critical("MAIN exception: {}", e.what());
	}

	logit->info("************************* application terminated *************************");
	return 0;
}

