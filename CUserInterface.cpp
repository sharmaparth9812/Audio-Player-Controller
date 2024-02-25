/**
 * \file CUserInterface.cpp
 * \brief implementation of CUserInterface
 *
 * \date 09.01.2020
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 * \author Parth Sharma
 */
#include <conio.h>
#include <iostream>
using namespace std;

#include "CUserInterface.h"

CUserInterface::CUserInterface() {
	m_playerCVDev = NULL;
}

void CUserInterface::init(PLAYER_CV_DEVS playerCtrlDev) {
	if (playerCtrlDev == CONSOLE) {
		m_playerCVDev = new CPlayerIOCtrls;
		printMessage("Console is used. ");
	} else if (playerCtrlDev == IOWARRIOR_EXT) {
		//todo task 1.2 comment OUT or delete after CIOWarriorExt can be used as C/V device
		//     and insert the code that creates a CIOWarriorExt object and prints an appropriate message
		m_playerCVDev = new CIOWarriorExt;
		printMessage("IOWarrior_EXT is used ");
	}

	try {
		m_playerCVDev->open();
	} catch (CException &e) {
		if (e.getSource() == CException::SRC_IOWarrior) {
			printMessage("IOWarrior can't be opened. Console is being used now. ");
			delete m_playerCVDev;
			m_playerCVDev = new CPlayerIOCtrls;
			// if this fails again, the method can't do anything else and
			// someone else may handle the error
			m_playerCVDev->open();
		}
	}
	// todo task 1.2c implement AmpMeter initialization here
	m_ampMeter.init(m_playerCVDev, CAmpMeter::SCALING_MODE_LIN, -2, 2, 0);
}

int CUserInterface::getListSelection(string *items, const string prompt) {
	int usel = CUI_UNKNOWN; // initialize user selection

	// how many list items have been passed?
	int numItems = 0;
	while (!items[numItems].empty())
		numItems++; // last item must be empty

	// display the input request (if any)
	if (!prompt.empty())
		printMessage(prompt + "\n");

	// displays the numbered list
	for (int i = 0; i < numItems; i++) {
		string item;
		item = "[" + to_string(i) + "]\t" + items[i] + "\n";
		printMessage(item);
	}
	// get the user selection
	usel = getUserInputInt("your number for selection: ");

	// check the user selection
	if ((usel >= 0) && (usel < numItems))
		return usel;
	else
		return CUI_UNKNOWN;
}

bool CUserInterface::keyPressed(bool bBlock) {
	if (bBlock) {
		while (m_playerCVDev->keyPressed() == false)
			; // wait for  the user to press the start button
		return true;
	} else
		return m_playerCVDev->keyPressed();
}

int CUserInterface::getUserInputInt(const string prompt) {
	// display the input request (if any)
	if (!prompt.empty())
		printMessage(prompt + "\n");
	return (int) m_console.readConsoleNumber();
}

float CUserInterface::getUserInputFloat(const string prompt) {
	// display the input request (if any)
	if (!prompt.empty())
		printMessage(prompt + "\n");
	return (float) m_console.readConsoleNumber();
}

double CUserInterface::getUserInputDouble(const string prompt) {
	// display the input request (if any)
	if (!prompt.empty())
		printMessage(prompt + "\n");
	return m_console.readConsoleNumber();
}

string CUserInterface::getUserInputString(const string prompt) {
	// display the input request (if any)
	if (!prompt.empty())
		printMessage(prompt + "\n");
	return m_console.readConsoleString();
}

string CUserInterface::getUserInputPath(const string prompt) {
	// display the input request (if any)
	if (!prompt.empty())
		printMessage(prompt + "\n");
	string path, modpath;
	path = m_console.readConsoleString();// user may provide a path with single backslashes
	unsigned int pathlen = path.length();
	unsigned int pos = 0, pos1 = 0;
	// single backslash is expanded to double backslash
	while ((pos1 = path.find("\\", pos)) < pathlen) {
		modpath = modpath + path.substr(pos, pos1 - pos) + "\\";
		pos = pos1 + 1;
	}
	modpath = modpath + path.substr(pos) + "\\";
	return modpath;
}

void CUserInterface::printMessage(const string msg) {
	m_console.writeConsole(msg);
}

void CUserInterface::visualizeAmplitude(float *databuf, int bufsize) {
	m_ampMeter.write(databuf, bufsize);
}

void CUserInterface::switchOffAmplitudeMeter() {
	m_ampMeter.write(0.);
}

void CUserInterface::setAmplitudeScaling(CAmpMeter::SCALING_MODE mode) {
	// todo implement setting of AmpMeter scaling here
	if(mode == CAmpMeter::SCALING_MODE_LIN)
		m_ampMeter.init(m_playerCVDev, mode, -2, 2, 0);
	else
		m_ampMeter.init(m_playerCVDev, mode, -2, 2, -30);
}

