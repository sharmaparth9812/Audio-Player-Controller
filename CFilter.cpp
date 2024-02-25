/**
 * \file CFilter.cpp
 * \brief implementation CFilterBase, CFilter, CDelayFilter
 *
 * \date 11.09.2019
 * \author A. Wirth <antje.wirth@h-da.de
 * \author Parth Sharma
 */
#include <math.h>
#include <SKSLib.h>
#include "CFilter.h"

CFilterBase::CFilterBase(int order, int channels) {
	m_order = abs(order);
	m_channels = abs(channels);
	if ((m_order != 0) && (m_channels != 0)) {
		// intermediate buffer: for the intermediate filter states from last sample
		m_z = new double[m_channels * (m_order + 1)];
		for (int i = 0; i < m_channels * (m_order + 1); i++) {
			m_z[i] = 0.;
		}
	} else
		throw CException(CException::SRC_Filter, -1,
				"Filter order and channels must not be zero!");
}

CFilterBase::~CFilterBase() {
	if (m_z != NULL)
		delete[] m_z;
}

void CFilterBase::reset() {
	for (int i = 0; i < m_channels * (m_order + 1); i++) {
		m_z[i] = 0.;
	}
}

int CFilterBase::getOrder() {
	return m_order;
}

CFilter::CFilter(const string filePath, float *ca, float *cb, int order, int channels) :
		CFilterBase(order, channels) {
	m_filePath=filePath;
	if ((ca != NULL) && (cb != NULL)) {
		m_a = new float[m_order + 1];
		m_b = new float[m_order + 1];
		for (int i = 0; i <= m_order; i++) {
			m_a[i] = ca[i] / ca[0];
			m_b[i] = cb[i] / ca[0];
		}
	} else
		throw CException(CException::SRC_Filter, -1,
				"Filter coefficients not available!");
}

CFilter::~CFilter() {
	if (m_a != NULL)
		delete[] m_a;
	if (m_b != NULL)
		delete[] m_b;
}

bool CFilter::filter(float *x, float *y, int framesPerBuffer) {
	// m_order frames have to be filtered at least, buffers for original samples (x)
	// and filtered samples (y) must be provided
	if ((framesPerBuffer < m_order) || (x == NULL) || (y == NULL))
		return false;

	// total buffer size with respect to interleaved channels
	int bufsize = framesPerBuffer * m_channels;
	// for all samples x of one block
	for (int k = 0; k < bufsize; k += m_channels) {
		// calculate the output y for all samples of one frame
		// y(k)=b0*x(k)+z0(k-1) - m_z contains all samples from the previous time step (k-1)
		for (int c = 0; c < m_channels; c++) {
			y[k + c] = m_b[0] * x[k + c] + m_z[0 + c];
		}
		// calculate the new state values z0(k) ..... zn-1(k) from the
		//          previous state values z1(k-1) ... zn(k-1)
		for (int n = 1; n <= m_order; n++) {
			for (int c = 0; c < m_channels; c++)
			{
				int cur_zpos= m_channels * (n - 1); // new state value position
				int next_zpos= m_channels * n;		// previous state value position
				m_z[cur_zpos + c] = 					/*   zn-1(k) = */
						            m_b[n] * x[k + c]   /*   bn*x(k) */
								  - m_a[n] * y[k + c]   /* - an*y(k) */
								  + m_z[next_zpos + c]; /* + zn(k-1) */			   ;
			}
		}
	}
	return true;
}

string CFilter::getFilePath() {
	return m_filePath;
}

// todo: Copy the implementation of your CDelayFilter class here

