#ifndef SRC_CUSERINTERFACE_H_
#define SRC_CUSERINTERFACE_H_
#include <string>
using namespace std;

#include "CConsoleIO.h"
#include "CPlayerIOCtrls.h"
#include "CAmpMeter.h"
#include "CIOWarrior.h"

#define CUI_UNKNOWN 0xffff // error value (maximum valid is CUI_UNKNOWN-1)

class CUserInterface {
public:
	/**
	 * devices for player control / visualization
	 */
	enum PLAYER_CV_DEVS {
		/**
		 * text console
		 */
		CONSOLE,
		/**
		 * IOWarrior with extension board
		 */
		IOWARRIOR_EXT
	};

private:
	/**
	 * Text console
	 *
	 * this object is used to communicate via command line for menue selection
	 * or entering parameters. This communication channel is necessary always
	 * because the IoWarrior classes no implementations to do that (not yet !!!)
	 */
	CConsoleIO m_console;
	/**
	 * control/visualization device
	 *
	 * contains elements for controlling the playing of the audio files
	 * (button(s)) and for displaying the audio signal amplitude (e.g. LED bargraph)
	 *
	 * this component can be attached either to text console or IoWarrior
	 */
	CPlayerCVDevice *m_playerCVDev;
	/**
	 * amplitude meter
	 *
	 * uses the attached control/visualization device to display the signal amplitude
	 */
	CAmpMeter m_ampMeter;

public:
	/**
	 * initializes private attributes
	 */
	CUserInterface();

	/**
	 * \brief Initializes the user interface.
	 *
	 * Creates an device object for player control/visualization and connects
	 * the amplitude meter with this device. If the device is not available,
	 * the console is used.
	 *
	 * The console is the default device.
	 *
	 * Throws an exception if the console device is not available.
	 *
	 * \param playerCVDev [in] device used for player control / visualization.
	 */
	void init(PLAYER_CV_DEVS playerCVDev = CONSOLE);

	/**
	 * \brief displays a menu and returns the user's choice
	 *
	 * \param items [in]: pointer to array containing items of menue list; last item MUST be an empty string
	 * \param prompt [in]: text printed for user information what to do
	 */
	int getListSelection(string *items, const string prompt = "");

	/**
	 * queries user defined path
	 *
	 * expands backslashes in the right manner
	 *
	 * \param prompt [in]: text printed for user information what to do
	 * \return path with correctly expanded backslashes
	 */
	string getUserInputPath(const string prompt = "");

	/**
	 * reads an integer number from console
	 *
	 * \param prompt [in]: text printed for user information what to do
	 * \return value entered by user
	 */
	int getUserInputInt(const string prompt = "");
	/**
	 * reads a float number from console
	 *
	 * \param prompt [in]: text printed for user information what to do
	 * \return value entered by user
	 */
	float getUserInputFloat(const string prompt = "");
	/**
	 * reads a double number from console
	 *
	 * \param prompt [in]: text printed for user information what to do
	 * \return value entered by user
	 */
	double getUserInputDouble(const string prompt = "");

	/**
	 * reads a string from console
	 *
	 * \param prompt [in]: text printed for user information what to do
	 * \return value entered by user
	 */
	string getUserInputString(const string prompt = "");

	/**
	 * prints message on console
	 *
	 * \param msg [in]: message to print
	 */
	void printMessage(string msg);

	/**
	 * waits for pressed ENTER key
	 *
	 * waiting either for pressed ENTER key (if Text Console is attached) or
	 * pressed button of IoWarrior base board (if IOWarrior is attached)
	 *
	 * In blocking mode it does not return before the user had pressed ENTER or
	 * IoW button.
	 *
	 * In blocking mode it returns immediately (see return value)
	 *
	 * \param bBlock [in] blocking mode
	 * \return
	 * - true: ENTER pressed (no IOWarrior connected) or
	 * - false: ENTER not pressed (only in non blocking mode)
	 */
	bool keyPressed(bool bBlock = false);

	/**
	 * Visualizes the amplitude of a data buffer on the LED line.
	 *
	 * \param databuf [in]: pointer on data buffer
	 * \param bufsize [in]: size of data buffer.
	 */
	void visualizeAmplitude(float *databuf, int bufsize);

	/**
	 * Switches the LEDs off.
	 */
	void switchOffAmplitudeMeter();

	/**
	 * sets amplitude meter's scaling mode
	 *
	 * \param mode [in]: scaling of amplitude meter instance
	 */
	void setAmplitudeScaling(CAmpMeter::SCALING_MODE mode);
};

#endif /* SRC_CUSERINTERFACE_H_ */
