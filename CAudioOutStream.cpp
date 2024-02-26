#include "CAudioOutStream.h"
#include <cstddef>
#include <SKSLib.h>
using namespace std;



CAudioOutStream::CAudioOutStream() {
	m_stream=NULL;
	m_state=NOTREADY;
}

CAudioOutStream::~CAudioOutStream() {
	// call the close method to ensure that all resources have been closed
	close();
	// released when the object is destroyed
	Pa_Terminate();
}

void CAudioOutStream::open(int numChan, double fSample,
	long framesPerBuffer) {
	// check the state of the object
	if(m_state != NOTREADY) // we have nothing to do, because the object has already been opened successfully
		return;

	if(m_state == NOTREADY){
		if(framesPerBuffer <= 0){
			CException myException(CException::SRC_SimpleAudioDevice, paNoError, "framesPerBuffer must be greater than 0");
			throw(myException);
		}

		PaError err = paNoError;
		err = Pa_Initialize();
		if(err != paNoError){
			CException myException(CException::SRC_SimpleAudioDevice, err, Pa_GetErrorText(err));
			throw(myException);
		}
		else{
			PaStreamParameters outParams;
			outParams.device = Pa_GetDefaultOutputDevice(); /* Default Output Device */
			if(outParams.device == paNoDevice){
				CException myException(CException::SRC_SimpleAudioDevice, paNoDevice, Pa_GetErrorText(paNoDevice));
				throw(myException);
			}
			outParams.channelCount = numChan;
			outParams.sampleFormat = paFloat32; /* 32 Bit Floating point Output */
			// Default Latency Value for Playing Sound Files
			outParams.suggestedLatency = Pa_GetDeviceInfo(outParams.device) -> defaultHighOutputLatency;
			outParams.hostApiSpecificStreamInfo = NULL;

			err = Pa_OpenStream(&m_stream, NULL, /* no input */&outParams, fSample, framesPerBuffer,
										paClipOff,	/* we won't output out of range samples so don't bother clipping them */
										NULL, 		/* no callback, use blocking API */
										NULL ); 	/* no callback, so no callback userData */
			if(err != paNoError){
				CException myException(CException::SRC_SimpleAudioDevice, err, Pa_GetErrorText(err));
				throw(myException);
			}

	}
	}

	// if all the actions have been successful
	m_state=READY;
}

void CAudioOutStream::start() {
	// check the state of the object

		if(m_state==NOTREADY) // this is an error
			{
				CException myException(CException::SRC_SimpleAudioDevice, -1, "First you have to open the audio stream!");
				throw(myException);
			}
			else if(m_state==PLAYING)
				return;
			// now we can handle the READY state
			PaError err= Pa_StartStream(m_stream);
			if( err != paNoError)
			{
				// this is another syntax for creating an throwing
				throw(CException(CException::SRC_SimpleAudioDevice, err, Pa_GetErrorText(err)));
			}
			m_state=PLAYING;

}

void CAudioOutStream::play(float* pBuffer, int noFrames)
{
	PaError err= paNoError;
	if(m_state == NOTREADY)throw(CException(CException::SRC_SimpleAudioDevice, paNoError, "Device is not initialized!"));
	else if(m_state == PLAYING)
	{
		err = Pa_WriteStream( m_stream, pBuffer, noFrames);
		if( err != paNoError )throw(CException(CException::SRC_SimpleAudioDevice, err, Pa_GetErrorText(err)));
	}
	else if(m_state == READY)throw(CException(CException::SRC_SimpleAudioDevice, paNoError, "First You have to Start the Device"));
}

void CAudioOutStream::stop() {

	// check the state of the object
	PaError err;
	if(m_state == NOTREADY)throw(CException(CException::SRC_SimpleAudioDevice, paNoError, "First you have to open the audio stream!"));
	else if(m_state == READY)
		return;
	else if(m_state == PLAYING)
	{
		err=Pa_StopStream(m_stream);
		if(err != paNoError)throw(CException(CException::SRC_SimpleAudioDevice, err, Pa_GetErrorText(err)));
		else
			m_state= READY;
	}
}

void CAudioOutStream::close()
{
	if(m_state == NOTREADY)return;
	stop();
	PaError err;
	err = Pa_CloseStream(m_stream);
	if(err != paNoError)throw(CException(CException::SRC_SimpleAudioDevice, err, Pa_GetErrorText(err)));
	m_state= NOTREADY;
}

void CAudioOutStream::pause()
{
	stop();
}

void CAudioOutStream::resume()
{
	start();
}

