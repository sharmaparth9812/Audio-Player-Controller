/*
 * CAudioOutStream.h
 *
 *  Created on: Oct 31, 2022
 *      Author: Wirth
 *      Author: Parth Sharma
 */

#ifndef SRC_CAUDIOOUTSTREAM_H_
#define SRC_CAUDIOOUTSTREAM_H_
#include "portaudio.h"

class CAudioOutStream {
public:
	enum STATES
	{
		NOTREADY,READY,PLAYING
	};
private:
	PaStream* m_stream;
	STATES m_state;
public:
	CAudioOutStream();
	~CAudioOutStream();
	void open(int numChan, double fSample, long framesPerBuffer);
	void start();
	void play(float* pBuffer, int noFrames);
	void pause();
	void stop();
	void resume();
	void close();
};

#endif /* SRC_CAUDIOOUTSTREAM_H_ */
