#include <switch.h>
#include <iostream>
using namespace std;

#include <CApp.h>

int main(int argc, char *argv[]) {

	/* Initializing RomFS */
	romfsInit();
	chdir("romfs:/");

	CApp theApp;

   	 return EXIT_SUCCESS;
}
