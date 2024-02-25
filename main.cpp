/**
 * \file main.cpp
 * \brief main, test and auxiliary functions for ASDD Lab05
 *
 * \date 17.01.2022
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 * \author Parth Sharma
 */

#include "CAudioPlayerController.h"
#include <iostream>

/**
 * horizontal divider for test list output
 */
string hDivider(80, '-');

/*
 * *****************************************************
 * declarations of test functions (to be called in main)
 * *****************************************************
 */
// laboratory tasks
void Test_Lab01_SoundFilterPlayTest(string &soundfile, string &sndfile_w,
		string &fltfile);

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
//	Test_Lab01_SoundFilterPlayTest(sndf, sndfw, fltf);

	CAudioPlayerController myController; 	// create the controller

	try {
		myController.run();					// run the controller
	} catch (CException &e) {
		cout << e << endl;
	}
	cout << "Systemintegration finished" << endl;
}

// todo insert Test_Lab01_SoundFilterPlayTest() here
void Test_Lab01_SoundFilterPlayTest(string &soundfile, string &sndfile_w,
		string &fltfile) {

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



