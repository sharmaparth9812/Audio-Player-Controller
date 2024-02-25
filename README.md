# Audio Player Controller

Description: Provide a brief overview of the audio player controller, its purpose, and its features. It's written in C++ using Eclipse CDT environment.

Table of Contents:

    Installation
    Usage
    Features
    Dependencies
    Contributing


# Installation
Describing the steps to install and set up the audio player controller.


    git clone https://github.com/sharmaparth9812/Audio-Player-Controller.git
    cd Audio-Player-Controller
    make


# Usage
Explanation on how to use the audio player controller. I have included relevant code snippets to demonstrate basic functionalities.

    int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	cout << "Systemintegration started" << endl;

	/*
	 * todo: Check if Test_Lab01_SoundFilterPlayTest() works
	 * comment out after check
	 */
	string sndname = "jazzyfrenchy";
	string sndf = ".\\files\\sounds\\" + sndname + ".wav";
	string sndfw = ".\\files\\sounds\\" + sndname + "_filtered.wav";
	string fltf = ".\\files\\filters\\2000Hz_lowpass_Order6.txt";
	Test_Lab01_SoundFilterPlayTest(sndf, sndfw, fltf);

	CAudioPlayerController myController; 	// create the controller

	try {
		myController.run();					// run the controller
	} catch (CException &e) {
		cout << e << endl;
	}
	cout << "Systemintegration finished" << endl;
}


# Features
List of the main features of the audio player controller.

    Sound file selection
    Filter application
    Amplitude scaling options
    Play/Pause functionality


# Dependencies
 Dependencies that users need to install before using the audio player controller.

    PortAudio API
    IO Warrior Kit


# Contributing
Feel free to contribute by submitting bug reports or proposing new features. Fork the repository, make your changes, and create a pull request.

