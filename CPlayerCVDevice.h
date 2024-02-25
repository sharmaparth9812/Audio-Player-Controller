/**
 * \file CPlayerCVDevice.h
 *
 * \brief interface of CPlayerCVDevice
 * \date Jan 9, 2023
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 * \author Parth Sharma
 */

#ifndef CPLAYERCVDEVICE_H_
#define CPLAYERCVDEVICE_H_
#include <cstdint>
#include <string>
using namespace std;

/**
 * \brief Base class for audio player control devices
 *
 * Strategy interface (pure virtual class).
 *
 * Provides an functional interface for a class supporting one button
 * start/pause/resume control and control of up to 16 LEDs for amplitude level
 * visualization.
 */
class CPlayerCVDevice {
public:
	CPlayerCVDevice(){};
	virtual ~CPlayerCVDevice(){};

	/**
	 * \brief Opens the device.
	 */
	virtual void open()=0;

	/**
	 * \brief Closes the device.
	 */
	virtual void close()=0;

	/**
	 * \brief Visualizes data on the device.
	 * \param data [in] data to visualize
	 */
	virtual void writeLEDs(uint16_t data)=0;

	/**
	 * \brief monitors the a single button for start/stop/resume control.
	 */
	virtual bool keyPressed()=0;

	/**
	 * \brief Queries the current state of the player controls as state name.
	 */
	virtual string getStateStr()=0;

	/**
	 * \brief Queries the last error message of the player controls.
	 */
	virtual string getLastErrorStr()=0;
};

#endif /* CPLAYERCVDEVICE_H_ */
