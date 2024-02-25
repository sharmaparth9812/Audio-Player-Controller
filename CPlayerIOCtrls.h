/**
 * \file CPlayerIOCtrls.h
 *
 * \brief header file of the modified PlayerIO control class
 * 		  the class handles:
 *        - thread controlled key input
 *        - binary pattern output
 */
#ifndef CPlayerIOCtrls_H_
#define CPlayerIOCtrls_H_

#include "CException.h"
#include "CConsoleThread.h"
#include "CPlayerCVDevice.h"

/**
 * PlayerIO control class
 *
 * modified for ASDD-Lab05 (systemintegration)
 *
 * the class handles:
 * - thread controlled key input
 * - binary pattern output
 */
class CPlayerIOCtrls: public CPlayerCVDevice {
protected:
	CConsoleThread *m_thread;

public:
	/**
	 * Constructor
	 * initializes the attributes with initial values
	 */
	CPlayerIOCtrls();

	/**
	 * Destructor
	 */
	virtual ~CPlayerIOCtrls();

	/**
	 * \brief Starts both of the threads
	 *
	 * throws an exception if it failed to start the threads.
	 */
	void open();

	/**
	 * \brief stops the threads
	 */
	void close();

	/**
	 * \brief Prints a binary formatted byte on the screen.
	 *
	 * throws an exception if the object is not in ready state
	 *
	 * \param data [in] to show as binary 8-bit pattern on the screen
	 */
	void writeLEDs(uint16_t data);

	/**
	 * \brief monitors the return key
	 *
	 * throws an exception if the object is not in ready state
	 *
	 * \return
	 * - true:  user pressed ENTER
	 * - false: otherwise
	 */
	bool keyPressed();

	/**
	 * \return current state of the instance
	 */
	string getStateStr();

	/**
	 * \return last error of the instance
	 */
	string getLastErrorStr();

	/**
	 * \brief Queries the current state of instance
	 * \return current state of instance as number
	 */
	int getState();

	/**
	 * \brief Queries the last error of instance
	 *
	 * \return number of last error
	 */
	int getLastError();
};

#endif /* CPlayerIOCtrls_H_ */
