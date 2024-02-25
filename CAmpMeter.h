/**
 * \file CAmpMeter.h
 * \brief interface of amplitude meter class CAmpMeter
 *
 * \date 29.11.2022
 * \author A. Wirth <antje.wirth@h-da.de
 * \author H. Frank <holger.frank@h-da.de
 * \author Parth Sharma
 */


#ifndef CAMPMETER_H_
#define CAMPMETER_H_

class CPlayerCVDevice;
class CAmpMeter {
public:
	enum SCALING_MODE {
		SCALING_MODE_LIN, SCALING_MODE_LOG
	};
	enum AMP_ERROR {
		AMP_E_NOBUFFER, AMP_E_NOVISUALIZER
	};

private:
	/**
	 * logarithmic or linear scaling
	 */
	SCALING_MODE m_scmode;
	/**
	 * maximum input value for this instance of CAmpMeter
	 *
	 * This value is the highest absolute value of the value range of the input signal.
	 * For a WAV file containing float samples in the range of -1.0 ... 1.0 this is 1.0 for example.
	 *
	 * In the linear scaling mode, this value is used for calculating the thresholds of
	 * the intervals and in the logarithmic scaling mode, it is used  as the reference value
	 * for the peak normalization. see also _getBarPattern().
	 */
	float m_inValMax;
	/**
	 * array of thresholds
	 *
	 * The threshold values are compared with the linear input values (absolute values)
	 * to calculate the bit pattern for the displayed bar.
	 *
	 * the content is calculated in init()
	 */
	float m_thresholds[16];
	/**
	 * pointer to an instance of the IOWarrior extension board class that shows the bar patterns on
	 * a line of 16 LEDs
	 */
	CPlayerCVDevice *m_IoDev;

public:
	/**
	 * Constructor
	 * initializes the attributes with initial values (see UML class diagram)
	 */
	CAmpMeter();

	/**
	 * \brief connects the amplitude meter with the IoDevice and calculates the scaling thresholds for visualization
	 *
	 * \param pIoDev [in] address of an IOWarrior extension board object
	 * \param scmode [in] scaling of the amplitude meter (linear/logarithmic)
	 * \param inValMin [in] minimum value in the value range of the input signal
	 * \param inValMax [in] maximum value in the value range of the input signal
	 * \param outLogValMin [in] minimum value of the logarithmic scale in dB (only relevant for logarithmic scaling)
	 *
	 *
	 * The absolute values of the samples of the input signal should be visualized since they are relevant for the perception
	 * of the volume. Therefore, the linear scale starts with 0 and ends with the maximum of the absolute values of inValMin and
	 * inValMax.
	 *
	 * outLogValMin is used as the minimum value for the logarithmic scale and may be
	 * set to 0 for linear scaling. The logarithmic scale starts with outLogValMin and ends with 0 [dB].
	 *
	 *
	 * thresholds for linear scaling
	 * ============================
	 * calculated threshold values are leftbound to their interval
	 *
	 * example:
	 * a total number of 10 intervals and a maximum absolute input value of 1.0
	 * and a minimum absolute value of 0
	 *
	 * |   0   |   1   |   2   | ... |   9   |   	interval index
	 * ^       ^       ^       ^     ^       ^
	 * 0.0     0.1     0.2     0.3   0.9     1.0	threshold values
	 *
	 *
	 *
	 * thresholds for logarithmic scaling
	 * ==================================
	 * calculated threshold values are leftbound to their interval
	 *
	 * example:
	 * a total number of 10 intervals and a maximum input value of 1.0
	 * and a outLogValMin value of -100 [dB]
	 * |   0   |  ...  |   7   |   8   |   9   |   	interval index
	 * ^       ^       ^       ^       ^       ^
	 * -100    -90     -30     -20     -10     0	logarithmic threshold values
	 * 10e-10  10e-9   0.001   0.01    0.1     1.0	equivalent linear threshold values
	 */
	void init(CPlayerCVDevice *pIoDev, SCALING_MODE scmode, float inValMin,
			float inValMax, int8_t outLogValMin = 0);

	/**
	 * \brief visualizes the amplitude of a data buffer on the connected IODevice as a bar pattern
	 *
	 * \param databuf [in] The address of the first data buffer element.
	 * \param databufsize [in] The number of elements of the data buffer.
	 *
	 * the displayed value is an amplitude value representative for the data buffer
	 *
	 * the bar is displayed from left to right
	 */
	void write(float *databuf, unsigned long databufsize);

	/**
	 * \brief Visualizes the amplitude of one single data value on the connected IODevice as a bar pattern
	 * \param data [in] The data value.
	 *
	 * the bar is displayed from left to right
	 */
	void writeLEDs(float data);

	/**
	 * \brief visualizes the amplitude of a single sample on the console as a binary pattern
	 * \param data [in] the sample value
	 *
	 * this is only for testing the AmpMeter without an IoW connected
	 *
	 * the bar is displayed from left to right, 1 represents an active element,
	 * 0 represents an inactive element
	 */
	void writeConsole(float data);

	void write(double data);
private:
	/**
	 * \brief Returns an appropriate bar pattern for the data value scaled according to
	 * the current settings of the amplitude meter
	 *
	 * \param data [in] linear data value.
	 */
	uint16_t _getBarPattern(float data);

	/**
	 * \brief Returns an amplitude value representative for the data buffer.
	 * This value is taken for the visualization of the buffer.
	 *
	 * \param databuf [in] The address of the first data buffer element.
	 * \param databufsize [in] The number of elements of the data buffer.
	 */
	float _getValueFromBuffer(float *databuf, unsigned long databufsize);
};
#endif /* CAMPMETER_H_ */
