# Audio Player Controller

Description: A brief overview of the audio player controller, its purpose, and its features. It's written in C++ using Eclipse CDT environment.

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
Explanation on how to use the audio player controller. After successfully importing the project in Eclipse, open main.cpp, build that and run it on the Eclipse environment. I have included relevant code snippets to demonstrate basic functionalities.


 // Required header files
 
  	#include "CAudioPlayerController.h"
	#include <iostream>
 // Declarations of test functions (to be called in main)
 
	void Test_Lab01_SoundFilterPlayTest(string &soundfile, string &sndfile_w, string &fltfile);
// Main function implemented here
 
	int main(void) 
	{
		setvbuf(stdout, NULL, _IONBF, 0);
		cout << "Systemintegration started" << endl;

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

// Test01_SoundFilterPlayTest() implemented here

	void Test_Lab01_SoundFilterPlayTest(string &soundfile, string &sndfile_w, string &fltfile) 
 	{

		try{

			cout << endl << hDivider << endl << __FUNCTION__ << " started." << endl << endl;

			CSoundFile sndfile(soundfile.c_str(), CSoundFile::FILE_READ);

			sndfile.open();

			CSoundFile soundfile_f(sndfile_w.c_str(), CSoundFile::FILE_WRITE);

			//As the filtered Audio File should have the same characteristics as the original one

			soundfile_f.setFormat(sndfile.getFormat());
			soundfile_f.setNumChannels(sndfile.getNumChannels());
			soundfile_f.setSampleRate(sndfile.getSampleRate());
			soundfile_f.open();

			CFilterFile filterfile(fltfile.c_str(), CFilterFile::FILE_READ);

			filterfile.open();
			filterfile.read(sndfile.getSampleRate());

			CFilter fltr(fltfile, filterfile.getACoeffs(), filterfile.getBCoeffs(), filterfile.getOrder(), sndfile.getNumChannels());

			CAudioOutStream caudiostream;

			float block_Duration = 1; //Block Duration in seconds

			long framesPerBlock = sndfile.getSampleRate() * block_Duration;

			int sbufsize = sndfile.getNumChannels() * framesPerBlock;

			float *sbufBlock = new float[sbufsize];
			float *sbufBlock_f = new float[sbufsize];

			caudiostream.open(sndfile.getNumChannels(), sndfile.getSampleRate(), framesPerBlock);
			caudiostream.start();
			int readsize = 0;

			do {
				/**
				 * sndfile.read() needs the total buffer size to be played
				 */
				readsize = sndfile.read(sbufBlock, sbufsize);
				fltr.filter(sbufBlock, sbufBlock_f, framesPerBlock);
				soundfile_f.write(sbufBlock_f, readsize);
				caudiostream.play(sbufBlock_f, readsize / soundfile_f.getNumChannels());

				// repeat as long as there is a complete block

			} while ( sbufsize == readsize);

			caudiostream.stop();
			cout << endl << __FUNCTION__ << " finished." << endl << hDivider << endl;
			caudiostream.close();
			delete[] sbufBlock, 					//Destroy Original Signal Buffer
			delete[] sbufBlock_f,					//Destroy Filtered Signal Buffer
			sndfile.close();
			soundfile_f.close();

			cout << endl << __FUNCTION__ << " finished." << endl << hDivider << endl;
		}
		catch(CException &err)
		{
			err.print();
		}
	}



# Features
List of the main features of the audio player controller.

    Sound file selection
    Filter application
	Amplitude scaling options
    Play/Pause control using keyboard and IOWarrior40 board


# Dependencies

	This project relies on several external dependencies that users need to install before using the audio player controller. Make sure to install the following dependencies:

	1. **PortAudio API:** [PortAudio Documentation](http://files.portaudio.com/docs/v19-doxydocs/)

	2. **IO Warrior API:** [IO Warrior Documentation](https://lernen.h-da.de/pluginfile.php/558454/mod_book/chapter/4647/IowKit_V15_API.pdf)
	
	3. **SKSlib API:** [SKSlib Documentation](https://sks-asdd.h-da.io/skslib)

	4. **libsndfile API:** [libsndfile Documentation](http://mega-nerd.com/libsndfile/api.html)

	5. **pthread API:** [pthread Documentation](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html)

	Please follow the provided links to download and install the necessary libraries.


# Contributing
Feel free to contribute by submitting bug reports or proposing new features. Fork the repository, make your changes, and create a pull request.

