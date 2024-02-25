/**
 * \file CConsoleIO.cpp
 * \brief implementation of CConsoleIO
 *
 * \date Jan 12, 2023
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 * \author Parth Sharma
 */

#include <iostream>
using namespace std;

#include "CConsoleIO.h"

CConsoleIO::CConsoleIO() {
	/*
	 * get the address of the one and only console thread object
	 */
	m_thread=CConsoleThread::getInstance();
	m_thread->open();
}

CConsoleIO::~CConsoleIO() {
	// nothing to do
}

void CConsoleIO::writeConsole(const string text)
{
	if(m_thread->getState() != CConsoleThread::S_READY)
	{
		// to write text before initialization
		cout << text;
	}
	else
		m_thread->writeConsole(text);
	return;
}

double CConsoleIO::readConsoleNumber()
{
	if(m_thread->getState() != CConsoleThread::S_READY)
		return stod(readConsoleString());
	else
		return m_thread->readConsoleNumber();
}

string CConsoleIO::readConsoleString()
{
	char line[256];
	string retstr;
	if(m_thread->getState() != CConsoleThread::S_READY)
	{
		// cin operator >> does not empty the keyboard buffer -> hence
		// kbthread handler gets an empty string once it came if there where
		// user inputs before the thread started. Thats why we use getline
		cin.getline(line,256);
		retstr=line;
	}
	else
		retstr=m_thread->readConsoleString();
	return retstr;
}
