#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

#include <SKSLib.h>
#include "CFile.h"

/**
 * Constructor
 */
CFileBase::CFileBase(string path, unsigned int mode) {
	m_path = path;
	if ((mode != FILE_READ) && (mode != FILE_WRITE)
			&& (mode != FILE_WRITEAPPEND))
		mode = FILE_MODEUNKNOWN;
	if (mode == FILE_MODEUNKNOWN)
		m_mode = FILE_READ | FILE_WRITEAPPEND; // allow all operations
	else
		m_mode = mode;
}

CFileBase::~CFileBase() {
	//cout << "CFileBase destructor" << endl;
}

void CFileBase::print(void) {
	cout << "CFileBase[" << getModeTxt() << "]: " << m_path << endl;
}

/**
 * utility methods to get the open mode
 */
bool CFileBase::isFileW() {
	if ((m_mode & FILE_WRITE) == 0)
		return false;
	return true;
}

bool CFileBase::isFileR() {
	if ((m_mode & FILE_READ) == 0)
		return false;
	return true;
}

bool CFileBase::isFileWA() {
	if ((m_mode & FILE_WRITEAPPEND) == 0)
		return false;
	return true;
}

string CFileBase::getErrorTxt(FILEERRORS err) {
	switch (err) {
	case FILE_E_UNKNOWNOPENMODE:
		return string("unknown file open mode");
	case FILE_E_NOFILE:
		return string("file not found");
	case FILE_E_FILENOTOPEN:
		return string("file not open");
	case FILE_E_NOBUFFER:
		return string("no data buffer available");
	case FILE_E_CANTREAD:
		return string("file has been opened in write only mode");
	case FILE_E_READ:
		return string("error during read");
	case FILE_E_CANTWRITE:
		return string("file has been opened in read only mode");
	case FILE_E_WRITE:
		return string("error during write");
	case FILE_E_SPECIAL:
		return string("file error: ");
	default:
		return string("unknown error");
	}
}

// here, we may have a combination of different modes
string CFileBase::getModeTxt() {
	string sret;
	if (m_mode & FILE_READ)
		sret += "read mode";
	if (m_mode & FILE_WRITE) {
		if (sret.empty())
			sret = "write mode";
		else
			sret += " & write mode";
	}
	if (m_mode & FILE_WRITEAPPEND) {
		if (sret.empty())
			sret = "write/append mode";
		else
			sret += " & write/append mode";
	}
	if (sret.empty())
		sret = "unknown mode";
	return sret;
}

CSoundFile::CSoundFile(string path, const FILEMODES mode) :
		CFileBase(path, mode) {
	memset(&m_sfinfo, 0, sizeof(m_sfinfo));
	m_pSFile = NULL;
	//cout << "CSoundFile constructor" << endl;
}

CSoundFile::~CSoundFile() {
	close();
	//cout << "CSoundFile destructor" << endl;
}

/**
 * overloaded method
 */
void CSoundFile::print(void) {
	CFileBase::print();
	cout << "CSoundFile[" << hex << m_pSFile << dec << "]" << endl;
	cout << "Soundfile: channels(" << m_sfinfo.channels << ") frames("
			<< m_sfinfo.frames;
	cout << ") fs(" << m_sfinfo.samplerate / 1000. << "kHz)" << endl;
	cout << "Duration: " << m_sfinfo.frames / m_sfinfo.samplerate << "s"
			<< endl;
}

void CSoundFile::open() {
	int mode;
	if (isFileR() && (isFileWA() || isFileW()))
		mode = SFM_RDWR;
	else if (isFileW() || isFileWA())
		mode = SFM_WRITE;
	else if (isFileR())
		mode = SFM_READ;
	else
		throw CException(CException::SRC_File, FILE_E_UNKNOWNOPENMODE,
				getErrorTxt(FILE_E_UNKNOWNOPENMODE));

	m_pSFile = sf_open(m_path.c_str(), mode, &m_sfinfo);
	if (!m_pSFile)
		throw CException(CException::SRC_File, sf_error(m_pSFile),
				m_path + ": " + sf_strerror(m_pSFile));

	// apply clipping when file has been opened for write (avoids faulty samples)
//	if((mode == SFM_RDWR) || (mode == SFM_WRITE))
//		sf_command (m_pSFile, SFC_SET_CLIPPING, NULL, SF_TRUE);
}

void CSoundFile::close() {
	if (m_pSFile != NULL) {
		sf_close(m_pSFile);
		m_pSFile = NULL;
	}
}

int CSoundFile::read(float *buf, int bufsize) {
	if (m_pSFile == NULL)
		throw CException(CException::SRC_File, FILE_E_FILENOTOPEN,
				getErrorTxt(FILE_E_FILENOTOPEN));
	if (buf == NULL)
		throw CException(CException::SRC_File, FILE_E_NOBUFFER,
				getErrorTxt(FILE_E_NOBUFFER));
	if (!isFileR())
		throw CException(CException::SRC_File, FILE_E_CANTREAD,
				getErrorTxt(FILE_E_CANTREAD));

	int szread = sf_read_float(m_pSFile, buf, bufsize);
	// returns 0 if no data left to read
	return szread;
}

void CSoundFile::rewind() {
	if (m_pSFile == NULL)
		throw CException(CException::SRC_File, FILE_E_FILENOTOPEN,
				getErrorTxt(FILE_E_FILENOTOPEN));

	sf_seek(m_pSFile, 0, SEEK_SET);
}
void CSoundFile::write(float *buf, int bufsize) {
	if (m_pSFile == NULL)
		throw CException(CException::SRC_File, FILE_E_FILENOTOPEN,
				getErrorTxt(FILE_E_FILENOTOPEN));
	if (buf == NULL)
		throw CException(CException::SRC_File, FILE_E_NOBUFFER,
				getErrorTxt(FILE_E_NOBUFFER));
	if (!(isFileW() || isFileWA()))
		throw CException(CException::SRC_File, FILE_E_CANTWRITE,
				getErrorTxt(FILE_E_CANTWRITE));

	int szwrite = sf_write_float(m_pSFile, buf, bufsize);
	if (szwrite != bufsize) {
		close();
		throw CException(CException::SRC_File, FILE_E_WRITE,
				getErrorTxt(FILE_E_WRITE));
	}
}

int CSoundFile::getNumFrames() {
	return m_sfinfo.frames;
}

int CSoundFile::getSampleRate() {
	return m_sfinfo.samplerate;
}

int CSoundFile::getFormat() {
	return m_sfinfo.format;
}

int CSoundFile::getNumChannels() {
	return m_sfinfo.channels;
}

void CSoundFile::setSampleRate(int fs) {
	if (fs <= 0)
		throw CException(CException::SRC_File, -1,
				"Invalid sample rate (must be > 0)!");
	m_sfinfo.samplerate = fs;
}

void CSoundFile::setNumChannels(int numChan) {
	if (numChan <= 0)
		throw CException(CException::SRC_File, -1,
				"Invalid number of channels (must be > 0)!");
	m_sfinfo.channels = numChan;
}

void CSoundFile::setFormat(int format) {
	m_sfinfo.format = format;
}

// < ---------- SAMPLE-START ----------->

CFilterFile::CFilterFile(string path, FILEMODES mode) :
		CFileBase(path, mode) {
	m_pFile = NULL;	// file pointer
	m_fs = 0;		// sample rate of the filter to load
	m_order = 0;	// filter order
	m_b = NULL;		// filter numerator coefficients
	m_blen = 0;		// number of filter numerator coefficients
	m_a = NULL;		// filter denominator coefficients
	m_alen = 0;		// number of filter denominator coefficients
}

CFilterFile::~CFilterFile() {
	close();
}

void CFilterFile::open() {
	if (!isFileR())
		throw CException(CException::SRC_File, FILE_E_SPECIAL,
				getErrorTxt(FILE_E_SPECIAL)
						+ "Writing is not implemented for this class!");
	m_pFile = fopen(m_path.c_str(), "r");
	if (m_pFile == NULL)
		throw CException(CException::SRC_File, FILE_E_NOFILE,
				getErrorTxt(FILE_E_NOFILE));
}

void CFilterFile::close() {
	if (m_pFile != NULL)
		fclose(m_pFile);
	if (m_b)
		delete[] m_b;
	if (m_a)
		delete[] m_a;
}

int CFilterFile::read(int fs) {
	if (m_pFile == NULL)
		throw CException(CException::SRC_File, FILE_E_FILENOTOPEN,
				getErrorTxt(FILE_E_FILENOTOPEN));

	if (fs == 0)
		throw CException(CException::SRC_File, FILE_E_SPECIAL,
				getErrorTxt(FILE_E_SPECIAL)
						+ "Sampling rate must be specified for a filter!");
	m_fs = abs(fs);

	fseek(m_pFile, 0, SEEK_SET);		// start at the beginning of the file

	// get the header
	int bufsize = 360;
	char buf[bufsize];

	// get the header
	fgets(buf, bufsize, m_pFile);

	// parse the header
	string s = buf;
	int pos = 0, end = 0;
	end = s.find(";", pos);
	m_filterType = s.substr(pos, end - pos);
	pos = end + 1;
	end = s.find(";", pos);
	string sorder = s.substr(pos, end - pos);
	m_order = stoi(sorder);
	m_filterInfo = s.substr(end + 1);

	// read data
	int i, fsr;
	while ( NULL != fgets(buf, bufsize, m_pFile)) {
		fsr = stoi(buf);			// find fs
		if (m_fs != fsr) {
			char *pgot = buf;
			while (pgot)			// skip b coefficients
			{
				pgot = fgets(buf, bufsize, m_pFile);
				if (NULL != strrchr(buf, '\n'))
					break; // end of line has been read
			}
			while (pgot)			// skip a coefficients
			{
				pgot = fgets(buf, bufsize, m_pFile);
				if (NULL != strrchr(buf, '\n'))
					break;
			}
		} else {
			m_b = new float[m_order + 1];
			m_a = new float[m_order + 1];
			char sep;
			for (i = 0; i < m_order + 1; i++) {
				if (EOF == fscanf(m_pFile, "%f%c", &m_b[i], &sep))
					break;
				if (sep == '\n')
					break;
			}
			m_blen = i;
			if (sep != '\n')
				fscanf(m_pFile, "%c", &sep);
			for (i = 0; i < m_order + 1; i++) {
				if (EOF == fscanf(m_pFile, "%f%c", &m_a[i], &sep))
					break;
				if (sep == '\n')
					break;
			}
			m_alen = i;
			if (sep != '\n')
				fscanf(m_pFile, "%c", &sep);
			return ftell(m_pFile);			// returns the number of bytes read
		}
	}
	return 0;						// 0 if no appropriate sampling rate found
}

void CFilterFile::print(void) {
	CFileBase::print();
	cout << m_filterType << "fiter [order=" << m_order << ", fs=" << m_fs
			<< "] " << m_filterInfo << endl;
	cout << "coefficients b={";
	for (int i = 0; i < m_blen; i++)
		cout << m_b[i] << "\t";
	cout << "}" << endl << "coefficients a={";
	for (int i = 0; i < m_alen; i++)
		cout << m_a[i] << "\t";
	cout << "}" << endl;
}

int CFilterFile::getOrder() {
	return m_order;
}

string CFilterFile::getFilterType() {
	return m_filterType;
}

string CFilterFile::getFilterInfo() {
	return m_filterInfo;
}

float* CFilterFile::getBCoeffs() {
	return m_b;
}

int CFilterFile::getNumBCoeffs() {
	return m_blen;
}

float* CFilterFile::getACoeffs() {
	return m_a;
}

int CFilterFile::getNumACoeffs() {
	return m_alen;
}

/** Obsolete?
 * was used to fill the database - may be added later or not
 */
/*
 void CFilterFile::getFs(const char *path, int *fs, int &numFs) {
 FILE *pFile = fopen(path, "r");
 if (pFile == NULL)
 throw CException(CException::SRC_File, FILE_E_FILENOTOPEN,
 "Can't open file for reading sample rates!");
 int bufsize = 100;
 char buf[bufsize];

 // skip the header
 do {
 fgets(buf, bufsize, pFile);
 } while (NULL == strrchr(buf, '\n'));

 // read fs
 int i = 0;
 while ((NULL != fgets(buf, bufsize, pFile)) && (numFs > i)) {
 fs[i++] = stoi(buf);			// store fs
 for (int j = 0; j < 2; j++)	// skip coefficients
 {
 do {
 fgets(buf, bufsize, pFile);
 } while (NULL == strrchr(buf, '\n'));
 }
 }
 numFs = i;
 fclose(pFile);
 }
 */
// < ---------- SAMPLE-END ----------->
