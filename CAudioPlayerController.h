#ifndef SRC_CAUDIOPLAYERCONTROLLER_H_
#define SRC_CAUDIOPLAYERCONTROLLER_H_
#include "CFile.h"
#include "CFilter.h"
#include "CUserInterface.h"
#include "CAudioOutStream.h"

class CAudioPlayerController {
private:

	CUserInterface m_ui;
	CFilterBase *m_pFilter;
	CSoundFile *m_pSFile;
	CAudioOutStream m_audioStream;

public:
	CAudioPlayerController();
	~CAudioPlayerController();
	void run();

private:
	void init();
	void chooseFilter();
	void play();
	void chooseSound();

	/**
	 * \brief Displays a menu with the options "linear scale" and "logarithmic scale" and
	 * \brief lets the user choose an option and calls m_ui.setAmplitudeScaling() according to
	 * \brief the users choice.
	 *
	 * if the users choice is invalid, an error message is shown and the scaling will not be changed.
	 *
	 */
	void chooseAmplitudeScale();

private:
	/**
	 * \brief user choice of filter from filter files stored in filePath
	 *
	 * only filter files that are containing the sampling frequency of the
	 * currently selected audio file
	 *
	 * \param fs[in] - appropriate sampling frequency
	 * \param chosenFile[out] - filter file chosen by the user
	 * \param filePath[in] - directory where the files are stored
	 * \param fileExt[in] - extension of the files
	 * \return id of the chosen filter in the menue list or CUI_UNKNOWN
	 */
	int _chooseFilterFile(string &chosenFile, string filePath =
			".\\files\\filters\\", string fileExt = ".txt");

	/**
	 * \brief creates filter from given filter file for given sampling frequency
	 * \param fs[in] - appropriate sampling frequency
	 * \param filterFile[int] - filter file
	 */
	void _createFilter(string filterFile);

	/**
	 * \brief lets the user enter the parameters of a delay filter
	 * \param delay_ms[out] - delay in milliseconds
	 * \param gFF[out] - feed forward gain (linear)
	 * \param gFB[out] - feed back gain (linear)
	 * \returns
	 * - true: entered parameters are valid
	 * - false: otherwise
	 */
	bool _configDelayFilter(int &delay_ms, float &gFF, float &gFB);

	/**
	 * \brief creates a delay filter for the given parameters
	 * \param delay_ms[in] - delay in milliseconds
	 * \param gFF[in] - feed forward gain (linear)
	 * \param gFB[in] - feed back gain (linear)
	 */
	void _createDelayFilter(int delay_ms, float gFF, float gFB);

	/*
	 * \brief adapt the current filter to a new sound file
	 *
	 * retrieves the filter information from the current filter and creates a
	 * new filter of the same type and configuration that matches the sampling
	 * frequency and the number of channels of the new sound file
	 */
	void _adaptFilter();

	/**
	 * \brief reads all filenames with the given extension from the given directory and writes them
	 * into a string array
	 * \param path[in] - directory where the files are stored
	 * \param ext[in] - extension of the files
	 * \param filelist[in/out] - array for the file names to be stored
	 * \param maxNumFiles[in/out] - size of the array
	 * \return number of files stored in the list (maxNumFiles at maximum)
	 */
	uint16_t _getFiles(string path, string ext, string *filelist,
			uint16_t maxNumFiles);
};
#endif /* SRC_CAUDIOPLAYERCONTROLLER_H_ */
