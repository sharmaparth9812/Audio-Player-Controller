#ifndef CCONSOLEIO_H_
#define CCONSOLEIO_H_

#include "CException.h"
#include "CConsoleThread.h"

class CConsoleIO {
private:
	CConsoleThread* m_thread;

public:
	CConsoleIO();
	virtual ~CConsoleIO();
	void writeConsole(const string text);
	double readConsoleNumber();
	string readConsoleString();
};

#endif /* CCONSOLEIO_H_ */
