/**
 * \file CFilter.h
 * \brief interface CFilterBase, CFilter, CDelayFilter
 *
 * \date 11.09.2019
 * \author A. Wirth <antje.wirth@h-da.de
 * \author Parth Sharma
 */
#ifndef CFILTER_H_
#define CFILTER_H_

/**
 * \brief filter base class
 *
 * provides common attributes for derived filter classes and defines basic
 * interface
 */
class CFilterBase {
protected:
	/**
	 * \brief intermediate states from last sample or circular buffer (optimized delay filters)
	 */
	double *m_z;
	/**
	 * \brief filter order
	 */
	int m_order;
	/**
	 * \brief number of channels of the signals to be filtered
	 */
	int m_channels;

public:
	/**
	 * \brief Constructor
	 *
	 * - initializes attributes
	 * - initializes buffer for intermediate states
	 * - throws exception if order or channels are zero
	 *
	 * \param order order of the filter
	 * \param channels no of channels of input signal
	 */
	CFilterBase(int order, int channels);
	/**
	 * Destructor
	 *
	 * deletes buffer for intermediate states
	 */
	virtual ~CFilterBase();
	/**
	 * \brief filters signal from input buffer x into output buffer y
	 *
	 * this pure virtual method defines the filter methods interface for the
	 * implementation in derived classes
	 *
	 * \param x pointer on block buffer of original signal
	 * \param y pointer on block buffer of filtered signal
	 * \param framesPerBuffer no of frames in the block buffers (original & filtered)
	 */
	virtual bool filter(float *x, float *y, int framesPerBuffer)=0;
	/**
	 * \brief resets filter
	 *
	 * Clears all intermediate values. May be used before filtering a new signal
	 * by the same filter object.
	 */
	void reset();
	/**
	 * \brief retrieves the order of the filter
	 */
	int getOrder();
};

/**
 * \brief General filter class to calculate digital filter output by direct form II transposed
 *
 * implements filter method, handles errors by exception
 */
class CFilter: public CFilterBase {
private:
	/**
	 * \brief filter coefficients of numerator
	 */
	float *m_b;
	/**
	 * \brief filter coefficients of denominator
	 */
	float *m_a;
	/*
	 * filter file path
	 */
	string m_filePath;
public:
	/**
	 * \brief Constructor
	 *
	 * - dynamic creation of arrays for filter coefficients
	 * - initialization of arrays
	 * - throws exception if order or channels are zero
	 *
	 * \param filePath path of the associated filter file
	 * \param ca pointer to array of denominator filter coefficients
	 * \param cb pointer to array of numerator filter coefficients
	 * \param order filter order
	 * \param channels number of channels of original signal
	 */
	CFilter(const string filePath, float *ca, float *cb, int order,
			int channels = 2);
	/**
	 * \brief deletes filter coefficient arrays
	 */
	virtual ~CFilter();
	/**
	 * \brief Filters a signal.
	 *
	 * \param x pointer on block buffer of original signal
	 * \param y pointer on block buffer of filtered signal
	 * \param framesPerBuffer no of frames in the block buffers (original & filtered)
	 * \return flag for successful execution (true) or error condition (false)
	 *
	 * The input/output buffer are float arrays, i.e. each element
	 * represents only a sample. For signals with multiple channels (e.g. stereo,
	 * surround sound etc.) the buffer size must be framesPerBuffer*channels
	 */
	bool filter(float *x, float *y, int framesPerBuffer); // straight forward difference equation

	/**
	 * \return path of filter file
	 */
	string getFilePath();
};

// todo Add your declaration of CDelayFilter here

#endif /* CFILTER_H_ */

