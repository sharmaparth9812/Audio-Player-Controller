#ifndef CIOWARRIOR_H_
#define CIOWARRIOR_H_

/*
 * The following two pre-processor code lines avoid build errors that occur as long as CIOWarrior is not
 * fully defined and implemented.
 */
//#define CIOW_COMPLETE		// includes CIOWarriorExt, CAmpMeter and the test functions in main in the build process
//#undef CIOW_COMPLETE		// comment out this line

#include <SKSLib.h>
#include <wtypes.h>
#include <iowkit.h>
#include "CPlayerCVDevice.h"

/**
 * \brief Controller for a bare IoW40Kit device
 */
class CIOWarrior : public CPlayerCVDevice {
public:
	// define the enums ERRORS and STATES here (see UML class diagram)
	enum ERRORS {E_OK, E_NODEVICE, E_INVALIDDEVICE, E_WRITEERRORS, E_DEVICENOTREADY, E_NOEXTENSION};
	enum STATES {S_NOTREADY, S_READY};
protected:
	// define the attributes according to the UML class diagram here
	IOWKIT_HANDLE m_handle;  						//Handle of the device
	IOWKIT40_IO_REPORT m_reportIn;					//Data read from the IOW Device
	IOWKIT40_IO_REPORT m_reportOut;					//Data sent to the IOWE Board to display pattern using LEDS
	ERRORS m_lastError;								//Saves the last error
	STATES m_state;									//Saves the current state of the device

public:
	// define the methods according to the UML class diagram here
	CIOWarrior(); 									//Constructor: Initializes the attributes
	virtual ~CIOWarrior();							//Destructor: Closes the Device
	void open();									//Opens the IOW Device
	void close();									//Closes the IOW Device
	void writeLEDs(uint8_t data);					//Writes the data to the LEDs
	bool keyPressed();								//Checks if the key is pressed or not and return true if it is pressed
	void printState();								//Prints the current state of the device
	STATES getState();								//Used to get the current state of the device
	string getStateStr();							//Used to get the current state in string format
	ERRORS getLastError();							//Used to get the last error occurred
	string getLastErrorStr();						//Used to get the last error in string format
	void printDeviceInfo();							//Prints the Device Information

protected:
	// define the protected method according to the UML class diagram here
	void writeReportOut(void);						//Writes the Data to the IOWExtension Board
};


/**
 * \brief class for IoWarrior Extension board of FBEIT
 * DO NOT CHANGE THIS CODE!!
 */
#ifdef CIOW_COMPLETE
class CIOWarriorExt: public CIOWarrior {
public:
	/**
	 * bitmasks of the buttons SW1 ... SW6 on the extension board
	 *
	 * to be used by readButtons()
	 *
	 * these values are not corresponding with the pin numbers of the buttons
	 * because they are not linear connected (see scheme of extension board)
	 */
	enum BUTTONS {
		BTN_1 = 0x1,
		BTN_2 = 0x2,
		BTN_3 = 0x4,
		BTN_4 = 0x8,
		BTN_5 = 0x10,
		BTN_6 = 0x20,
		/**
		 * masking all buttons
		 */
		BTN_ALL = 0x3f
	};
	/**
	 * \brief initializes IoWExtension
	 */
	CIOWarriorExt();
	/**
	 * \brief deinitializes IoWExtension
	 */
	~CIOWarriorExt();
	/**
	 * \brief opens IoW with extension
	 *
	 * calls detect() => throwing error if no extension is detected
	 *
	 * \exception
	 * - device not ready
	 */
	void open(void);
	/**
	 * \brief prints device info
	 *
	 * prints a hint that the extension is available and all infos of
	 * CIOWarrior::printDeviceInfo()
	 */
	void printDeviceInfo(void);
	/**
	 * \brief reads status of 6 custom buttons on extension board at port 0
	 *
	 * the state of the buttons is returned in Bit 0 (SW1) to Bit 5 (Sw6)
	 *
	 * a new report is only generated, if a change occurs at the input pins
	 * \see readButton()
	 *
	 * \return state of custom buttons SW1 ... SW6
	 * 1 if a transition on the pin from released (high) to pressed (low)
	 *
	 * \exception
	 * - device not ready
	 */
	uint8_t readButtons(uint8_t mask = CIOWarriorExt::BTN_ALL);
	/**
	 * \brief writes binary pattern on the LEDs D1 ... D16
	 *
	 * the pattern is high active
	 *
	 * \param value binary pattern
	 * \exception
	 * - device not ready
	 */
	void writeLEDs(uint16_t value);
private:
	/**
	 * \brief detects extension
	 * \exception
	 * - device not ready
	 * - extension not available
	 */
	void detect(void);
};
#endif /* CIOW_COMPLETE */
#endif /* CIOWARRIOR_H_ */
