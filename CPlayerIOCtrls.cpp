/**
 * \file CPlayerIOCtrls.cpp
 *
 * \brief implementation of CPlayerIOCtrls
 * \date 30.10.2020
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 * \author Parth Sharma
 */
#include "windows.h"
#include "ctype.h"
#include <iostream>
using namespace std;

#include "CException.h"
#include "CPlayerIOCtrls.h"

CPlayerIOCtrls::CPlayerIOCtrls() {
	/*
	 * get the address of the one and only console thread object
	 */
	m_thread = CConsoleThread::getInstance();
}

CPlayerIOCtrls::~CPlayerIOCtrls() {
	close();
}

/**
 * INTERFACE of CPlayerCVDevice: implementation for the console
 */
void CPlayerIOCtrls::open() {
	m_thread->open();
	return;
}

void CPlayerIOCtrls::close() {
	// nothing to do, but must be provided, because of the mandatory interface of
	// player control/visualization devices
}

void CPlayerIOCtrls::writeLEDs(uint16_t data) {
	string binPattern;
	for (int i = 0; i < 16; i++) {
		data & 0x8000 ? binPattern += "1" : binPattern += "0";
		data <<= 1;
	}
	/*
	 * print only a carriage return (\r) and no line feed (\n) so
	 * the bit pattern is always printed in the same line in the
	 * external console window
	 *
	 * in the eclipse console view \r is always printed with an
	 * additional line feed, so the patterns are printed in
	 * subsequent lines
	 */
	binPattern += '\r';

	m_thread->writeConsole(binPattern);
	return;
}

bool CPlayerIOCtrls::keyPressed() {
	return m_thread->enterPressed();
}

string CPlayerIOCtrls::getStateStr() {
	return m_thread->getStateStr();
}

string CPlayerIOCtrls::getLastErrorStr() {
	return m_thread->getLastErrorStr();
}

int CPlayerIOCtrls::getState() {
	// besser: static_cast<int>(...)
	return (int) m_thread->getState();
}

int CPlayerIOCtrls::getLastError() {
	// besser: static_cast<int>(...)
	return (int) m_thread->getLastError();
}

