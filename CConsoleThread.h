#ifndef CCONSOLETHREAD_H_
#define CCONSOLETHREAD_H_

#include <pthread.h>
#include "CException.h"

/**
 * \brief provides threads for console input and console output
 *
 * realizes non-blocking input
 *
 * The class is designed as a singleton (design pattern) to ensure that only one
 * object exists. This is necessary, because it has access to the shared
 * resources cin and cout.
 */
class CConsoleThread {
public:
	/**
	 * \brief Error Codes
	 */
	enum ERRORS {
		E_OK,
		/**
		 * input thread is not ready (not started)
		 */
		E_KBTHREADNOTREADY,
		/**
		 * start of input thread failed
		 */
		E_KBTHREADFAILED,
		/**
		 * output thread is not ready (not started)
		 */
		E_BPTHREADNOTREADY,
		/**
		 * start of input thread failed
		 */
		E_BPTHREADFAILED
	};
	/**
	 * \brief Device States
	 *
	 * used to implement a simple state machine
	 */
	enum STATES {
		/**
		 * at least one thread is not yet started
		 */
		S_NOTREADY,
		/**
		 * threads are starting
		 */
		S_STARTING,
		/**
		 * all threads are up
		 */
		S_READY
	};

protected:
	/**
	 * handle of the output thread (NULL if no thread has been started at program start)
	 */
	pthread_t m_outThreadHandle;
	/**
	 * mutex for the condition of the output thread
	 */
	pthread_mutex_t m_outMut;
	/**
	 * condition of the boutput thread to wakeup an do its job
	 */
	pthread_cond_t m_outCond;

	/**
	 * variable for the output thread to get the text to show on the screen
	 */
	string m_outText;
	/**
	 * variable signalizes that the output text has been changed
	 */
	bool m_outTextChanged;

	/**
	 * handle of the input thread (NULL if no thread has been started at program start)
	 */
	pthread_t m_inThreadHandle;
	/**
	 * mutex for the condition of the inputthread
	 */
	pthread_mutex_t m_inMut;
	/**
	 * condition of the input thread to wakeup an do its job
	 */
	pthread_cond_t m_inCond;
	/**
	 * object for entered text
	 */
	string m_inText;
	/**
	 * indicates that new text has been entered and terminated by ENTER
	 */
	bool m_inTextChanged;

	/**
	 * saves the last error occurred (E_OK if no error occurred)
	 */
	ERRORS m_lastError;
	/**
	 * saves the current state of the instance (S_READY if all threads are started)
	 */
	STATES m_state;

private:
	/**
	 * Constructor is protected that only one single object can be created (Singleton)
	 *
	 * initializes the attributes with initial values
	 */
	CConsoleThread();
	/**
	 * Destructor is private that nobody can destroy the singleton except itself
	 *
	 * stops the threads (calling close)
	 */
	virtual ~CConsoleThread();


public:
	/*
	 * provides access to the single CConsoleThread object
	 */
	static CConsoleThread* getInstance();

	/**
	 * \brief Starts both of the threads.
	 *
	 * throws an exception if it failed to start the threads.
	 */
	void open();

	/**
	 * \brief Queries the current state of the instance as string
	 */
	string getStateStr();

	/**
	 * \brief Queries the last error of the instance as string
	 */
	string getLastErrorStr();

	/**
	 * \brief writes the text on the console window
	 *
	 * if the object is not in ready state it uses cout instead of the byte
	 * pattern thread
	 * \param text [in] text to be written on the console output
	 */
	void writeConsole(const string text);

	/**
	 * \brief waits for the user to enter a number (blocking)
	 *
	 * if the object is not in ready state it uses cin instead of the keyboard
	 * monitoring thread
	 *
	 * \return entered number
	 */
	double readConsoleNumber();

	/**
	 * \brief waits for the user to enter a string (blocking)
	 *
	 * if the object is not in ready state it uses cin instead of the keyboard
	 * monitoring thread
	 *
	 * \return entered string
	 */
	string readConsoleString();

	/**
	 * input thread: ENTER key monitoring (non-blocking)
	 *
	 * \return
	 * - true: ENTER pressed
	 * - false: ENTER not pressed
	 */
	bool enterPressed();

	/**
	 * \brief Prints the current state of the player IO control.
	 */
	void printState();

	/**
	 * \brief Queries the current state of instance
	 * \return current state of instance as number
	 */
	STATES getState();

	/**
	 * \brief Queries the last error of instance
	 *
	 * \return number of last error
	 */
	ERRORS getLastError();

private:
	/**
	 * \brief stops the threads
	 *
	 * because we have a Singleton that may be used by several other objects,
	 * close() must not be called unless the whole program is terminated)
	 */
	void close();

	/**
	 * \brief controls the behavior of output thread
	 *
	 * \param Obj pointer on the instance
	 */
	static void* outThreadHandler(void *Obj);
	/**
	 * \brief controls the behavior of input thread
	 *
	 * \param Obj pointer on the instance
	 */
	static void* inThreadHandler(void *Obj);
};

#endif /* CCONSOLETHREAD_H_ */
