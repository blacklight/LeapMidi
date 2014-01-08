#include <iostream>
#include "leapmidi.h"

using namespace std;
using namespace Leap;

int main(int argc, char **argv, char **envp) {
	if (argc == 1) {
		cerr << "Usage: " << argv[0] << " <basenote>" << endl
			<< "\tThe basenote can be any number starting from 0 (lowest C)" << endl
			<< "\tRemember that the scale is divided in 12 half-tones" << endl;
		return 1;
	}

	const int basenote = atoi(argv[1]);
	SoundClient soundClient;
	LeapListener listener(basenote, soundClient);
	Controller controller;
	controller.addListener(listener);

	cout << "Press Enter to quit..." << endl;
	cin.get();

	controller.removeListener(listener);
	return 0;
}

