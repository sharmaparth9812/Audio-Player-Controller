/**
 * \file CFile.h
 *
 * \brief interface file of CFileBase, CSoundFile, CFilterFile
 * \date 14.11.2019
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 * \author Parth Sharma
 */
#ifndef FILE_H_
#define FILE_H_

#include "sndfile.h"
#include <string>
using namespace std;

/**
 * base class for files contains common properties and common behavior for
 * all files
 */

class CFileBase {
public:
	enum FILEMODES {
		FILE_MODEUNKNOWN = 0x00,
		FILE_READ = 0x01,
		FILE_WRITE = 0x02,
		FILE_WRITEAPPEND = 0x04
	};
	enum FILEERRORS {
		FILE_E_UNKNOWNOPENMODE,
		FILE_E_NOFILE,
		FILE_E_FILENOTOPEN,
		FILE_E_NOBUFFER,
		FILE_E_READ,
		FILE_E_CANTREAD,
		FILE_E_CANTWRITE,
		FILE_E_WRITE,
		FILE_E_SPECIAL
	};

private:
	unsigned int m_mode;	// file mode

protected:
	string m_path;			// complete file path

public:
	CFileBase(string path, unsigned int mode = FILE_MODEUNKNOWN);
	virtual ~CFileBase();

	/**
	 * prints content of the file on console
	 */
	virtual void print(void);

protected:
	/**
	 * Methods for the retrieval of file mode
	 */
	bool isFileW();
	bool isFileR();
	bool isFileWA();
	/**
	 * Methods for conversion of enum values into text
	 */
	string getErrorTxt(FILEERRORS err);
	string getModeTxt();
};

/**
 * Soundfile handling class
 */
class CSoundFile: public CFileBase {
private:
	/**
	 * pointer on soundfile
	 */
	SNDFILE *m_pSFile;
	/**
	 * contains metadata of the soundfile
	 */
	SF_INFO m_sfinfo;

public:
	/**
	 * initializes attributes
	 */
	CSoundFile(string path, const FILEMODES mode);
	~CSoundFile();

	/**
	 * opens the sound file in the constructor for further file operations
	 */
	void open();
	/**
	 * closes the sound file
	 */
	void close();
	/**
	 * \brief  reads content of sound file
	 *
	 * \params buf[in] - address of a buffer to store floating point audio data
	 * \params bufsize[in] - size of the buffer in floating point elements
	 * \return total number of samples (not frames!) read
	 */
	int read(float *buf, int bufsize);
	/**
	 * writes the content of the sound file
	 *
	 * \params buf[in] - address of a buffer to read from floating point audio data
	 * \params bufsize[in] - size of the buffer in floating point elements
	 * \return total number of samples (not frames!) written
	 */
	void write(float *buf, int bufsize);
	/**
	 * sets the file pointer of an open sound file back to the start
	 */
	void rewind();
	/**
	 * prints the soundfile properties contained in m_sfinfo on the console
	 */
	void print(void);
	/**
	 * \return number of frames
	 */
	int getNumFrames();
	/**
	 * \return sample rate [Hz]
	 */
	int getSampleRate();
	/**
	 * \return number of audio channels
	 */
	int getNumChannels();
	/**
	 * \return format of the audio file
	 *
	 * see doc of libsndfile on http://mega-nerd.com/libsndfile/api.html
	 */
	int getFormat();

	/**
	 * sets sample rate of audio file
	 *
	 * \param fs [in] sample rate in Hz
	 */
	void setSampleRate(int fs);
	/**
	 * sets number of audio channels
	 *
	 * \param numChan [in] number of channels
	 */
	void setNumChannels(int numChan);
	/**
	 * \param format format of audiofile
	 *
	 * see doc of libsndfile on http://mega-nerd.com/libsndfile/api.html
	 */
	void setFormat(int format);
};

/**
 * class to read files with the ASDD filter file format
 * derived from CFileBase
 * implements additional methods for parsing filter files
 * overloads the basic class' print method
 */
class CFilterFile: public CFileBase {
private:
	FILE *m_pFile;
	string m_filterType; // type of the filter (e.g. lowpass, shelving, delay, ...)
	string m_filterInfo;	// free textual filter description
	int m_order;			// filter order
	float *m_b;				// filter numerator coefficients
	int m_blen;				// number of filter numerator coefficients
	float *m_a;				// filter denominator coefficients
	int m_alen;				// number of filter denominator coefficients
	int m_fs;				// sampling frequency

public:
	/**
	 * initializes attributes
	 */
	CFilterFile(string path, FILEMODES mode = FILE_MODEUNKNOWN);
	~CFilterFile();
	/**
	 * opens a filter file
	 */
	void open();
	/**
	 * closes a filter file
	 */
	void close();
	/**
	 * reads the content of the file
	 *
	 * \param fs [in] sampling frequency for which the coefficients has to be read from the file
	 */
	int read(int fs);
	/**
	 * prints content of the filter file on console
	 */
	void print(void);
	/**
	 * \return order of the filter
	 */
	int getOrder();
	/**
	 * \return type of the filter (e.g. lowpass, shelving, delay, ...)
	 */
	string getFilterType();
	/**
	 * \return free textual filter description
	 */
	string getFilterInfo();
	/**
	 * \return pointer on array containing filter numerator coefficients
	 */
	float* getBCoeffs();
	/**
	 * \return number of filter numerator coefficients
	 */
	int getNumBCoeffs();
	/**
	 * \return pointer on array containing filter denominator coefficients
	 */
	float* getACoeffs();
	/**
	 * \return number of filter denominator coefficients
	 */
	int getNumACoeffs();
};

#endif /* FILE_H_ */
