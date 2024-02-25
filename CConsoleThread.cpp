/**
 * \file CConsoleThread.cpp
 * \brief implementation of CConsoleThread
 *
 * \date Jan 12, 2023
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 * \author Parth Sharma
 */
#include <iostream>
using namespace std;

#include "CConsoleThread.h"

CConsoleThread::CConsoleThread() {
	m_outThreadHandle = pthread_t { }; // initializes a struct with 0
	m_outMut = PTHREAD_MUTEX_INITIALIZER;
	m_outCond = PTHREAD_COND_INITIALIZER;
	m_outTextChanged = false;

	// keyboard monitoring thread
	m_inThreadHandle = pthread_t { };
	m_inMut = PTHREAD_MUTEX_INITIALIZER;
	m_inCond = PTHREAD_COND_INITIALIZER;
	m_inTextChanged = false;

	m_lastError = E_OK;
	m_state = S_NOTREADY;
}

/*
 * at the first call, an instance of CConsoleThread is created
 * at all subsequent calls, the address of the one and only object is returned
 * this method to create the one and only instance is thread save, becaus it is created
 * at a defined time (at the first call to instance)
 */
CConsoleThread* CConsoleThread::getInstance() {
	static CConsoleThread instance;
	return &instance;
}

CConsoleThread::~CConsoleThread() {
	close();
}

void CConsoleThread::open() {
	// evaluate state
	if (m_state == S_READY)
		return;

	// set device state to ready
	m_state = S_STARTING;

	// binary pattern output
	pthread_mutex_init(&m_outMut, 0);
	pthread_cond_init(&m_outCond, 0);
	int rc = pthread_create(&m_outThreadHandle, NULL, outThreadHandler,
			(void*) this);
	if (rc != 0) {
		// release resources
		pthread_mutex_destroy(&m_outMut);
		pthread_cond_destroy(&m_outCond);
		// put the object in a definite state
		m_lastError = E_BPTHREADFAILED;	// set error value
		m_state = S_NOTREADY;
		// throw an exception
		throw(CException(CException::SRC_IOConsole, getLastError(),
				getLastErrorStr()));
	}
	// busy waiting
	while (m_state != S_READY)
		;

	// keyboard monitoring
	m_state = S_STARTING;
	pthread_mutex_init(&m_inMut, 0);
	pthread_cond_init(&m_inCond, 0);
	rc = pthread_create(&m_inThreadHandle, NULL, inThreadHandler, (void*) this);
	if (rc != 0) {
		// terminate binary pattern output thread
		pthread_kill(m_outThreadHandle, 0); // signal number always 0 for windows
		// release resources
		pthread_mutex_destroy(&m_outMut);
		pthread_cond_destroy(&m_outCond);
		pthread_mutex_destroy(&m_inMut);
		pthread_cond_destroy(&m_inCond);
		m_lastError = E_KBTHREADFAILED;	// set error value
		m_state = S_NOTREADY;
		throw(CException(CException::SRC_IOConsole, getLastError(),
				getLastErrorStr()));
	}

	// busy waiting
	while (m_state != S_READY)
		;

	// set error to ok
	m_lastError = E_OK;
	return;
}

void CConsoleThread::close() {
	if (m_state == S_NOTREADY)
		return;

	// terminate both keyboard monitoring thread
	pthread_kill(m_inThreadHandle, 0);// kills the kb thread while it is waiting for an user input (signal number always 0 for windows)

	m_state = S_NOTREADY;			// will cause the thread to terminate

	pthread_mutex_lock(&m_outMut);
	pthread_cond_signal(&m_outCond);	// wake-up the bp thread to terminate
	pthread_mutex_unlock(&m_outMut);

	pthread_join(m_outThreadHandle, NULL); // waits for the bp thread to be terminated (no return value needed)
	pthread_mutex_destroy(&m_outMut);
	pthread_cond_destroy(&m_outCond);
	pthread_mutex_destroy(&m_inMut);
	pthread_cond_destroy(&m_inCond);
}

bool CConsoleThread::enterPressed() {
	if (m_state != S_READY) {
		m_lastError = E_KBTHREADNOTREADY;
		throw(CException(CException::SRC_IOConsole, getLastError(),
				getLastErrorStr()));
	}

	// the attribute is set by the thread, a detected key press is cleared once it has been read
	if (m_inTextChanged == true) {
		pthread_mutex_lock(&m_inMut);
		m_inTextChanged = false;
		pthread_mutex_unlock(&m_inMut);
		return true;
	} else {
		return false;
	}
}

void CConsoleThread::writeConsole(const string text) {
	// check the state
	if (m_state != S_READY) {
		m_lastError = E_BPTHREADNOTREADY;
		throw(CException(CException::SRC_IOConsole, getLastError(),
				getLastErrorStr()));
	}

	// wait for thread to finish output
	pthread_mutex_lock(&m_outMut);
	while ((m_state == S_READY) && (m_outTextChanged == true)) {
		pthread_cond_wait(&(m_outCond), &(m_outMut)); // wait unlocks at entrance and re-locks at the end
	}

	// set the new pattern to transfer to the thread
	// critical region start
	m_outText = text;
	m_outTextChanged = true;				// binary output has been changed
	pthread_cond_signal(&m_outCond);		// wake-up the thread
	pthread_mutex_unlock(&m_outMut);
	// end of critical region
	return;
}

double CConsoleThread::readConsoleNumber() {
	// todo exception
	return stod(readConsoleString());
}

string CConsoleThread::readConsoleString() {
	pthread_mutex_lock(&m_inMut);
	while ((m_state == S_READY) && (m_inTextChanged == false)) {
		pthread_cond_wait(&(m_inCond), &(m_inMut)); // wait unlocks at entrance and re-locks at the end
	}
	m_inTextChanged = false;
	pthread_mutex_unlock(&m_inMut);
	return m_inText;
}

CConsoleThread::STATES CConsoleThread::getState() {
	return m_state;
}

CConsoleThread::ERRORS CConsoleThread::getLastError() {
	return m_lastError;
}

string CConsoleThread::getStateStr() {
	switch (m_state) {
	case S_NOTREADY:
		return string("console thread is not ready");
	case S_STARTING:
		return string("console thread is just starting");
	case S_READY:
		return string("console thread is ready");
	default:
		return string("Unknown State");
	}
}

string CConsoleThread::getLastErrorStr() {
	switch (m_lastError) {
	case E_BPTHREADNOTREADY:
		return string(
				"Error: binary pattern output thread is not active - please call the open method first");
	case E_BPTHREADFAILED:
		return string("Error: binary pattern output thread could not start. ");
	case E_KBTHREADNOTREADY:
		return string(
				"Error: keyboard monitoring thread is not active - please call the open method first");
	case E_KBTHREADFAILED:
		return string("Error: keyboard monitoring thread could not start. ");
	case E_OK:
		return string("No Error :-)");
	default:
		return string("Unknown Error :-(");
	}
}

void CConsoleThread::printState() {
	string statestr = "\nconsole thread state: \n" + getStateStr() + "\n"
			+ getLastErrorStr() + "\n";
	if (m_state == S_NOTREADY) {
		cout << statestr;
	} else
		writeConsole(statestr);
}

void* CConsoleThread::outThreadHandler(void *Obj) {
	CConsoleThread *pPIOC = (CConsoleThread*) Obj;
	cout << "output thread has been started" << endl;
	pPIOC->m_state = S_READY;

	while (1) {
		pthread_mutex_lock(&(pPIOC->m_outMut));
		// put thread to sleep while there is nothing to do
		// if there is something to do, the main thread (in our case) wakes up the thread by setting a condition
		while ((pPIOC->m_state == S_READY) && (pPIOC->m_outTextChanged == false)) {
			pthread_cond_wait(&(pPIOC->m_outCond), &(pPIOC->m_outMut)); // wait unlocks at entrance and re-locks at the end
		}

		if (pPIOC->m_state == S_NOTREADY) {
			pthread_mutex_unlock(&(pPIOC->m_outMut));
			break;
		} else {
			if (pPIOC->m_outTextChanged == true) {
				cout << pPIOC->m_outText;
				pPIOC->m_outTextChanged = false;
				// signals main thread that output is done
				pthread_cond_signal(&pPIOC->m_outCond);
			}
			pthread_mutex_unlock(&pPIOC->m_outMut);
		}
	}
	cout << "output thread terminates. " << endl;
	return NULL;
}

void* CConsoleThread::inThreadHandler(void *Obj) {
	CConsoleThread *pPIOC = (CConsoleThread*) Obj;
	cout << "input thread has been started" << endl;
	char line[256];
	pPIOC->m_state = S_READY;
	while (pPIOC->m_state == S_READY) {
		cin.getline(line, 256);
		pthread_mutex_lock(&(pPIOC->m_inMut));
		pPIOC->m_inTextChanged = true;
		pPIOC->m_inText = line;
		// signals main thread that input is available
		pthread_cond_signal(&pPIOC->m_inCond);
		pthread_mutex_unlock(&pPIOC->m_inMut);
	}
	return NULL;
}
