#ifndef MAX30100_ALL_H
#define MAX30100_ALL_H

#include <stdint.h>
#include "MAX30100_Registers.h"
//#include "MAX30100_Filters.h"
#include <math.h>
#include <project.h>

//BeatDetector--------------------------------------------------------------------------------------------
#define BEATDETECTOR_INIT_HOLDOFF                2000    // in ms, how long to wait before counting
#define BEATDETECTOR_MASKING_HOLDOFF             200     // in ms, non-retriggerable window after beat detection
#define BEATDETECTOR_BPFILTER_ALPHA              0.6     // EMA factor for the beat period value
#define BEATDETECTOR_MIN_THRESHOLD               20      // minimum threshold (filtered) value
#define BEATDETECTOR_MAX_THRESHOLD               800     // maximum threshold (filtered) value
#define BEATDETECTOR_STEP_RESILIENCY             30      // maximum negative jump that triggers the beat edge
#define BEATDETECTOR_THRESHOLD_FALLOFF_TARGET    0.3     // thr chasing factor of the max value when beat
#define BEATDETECTOR_THRESHOLD_DECAY_FACTOR      0.99    // thr chasing factor when no beat
#define BEATDETECTOR_INVALID_READOUT_DELAY       2000    // in ms, no-beat time to cause a reset
#define BEATDETECTOR_SAMPLES_PERIOD              10      // in ms, 1/Fs

typedef enum BeatDetectorState {
    BEATDETECTOR_STATE_INIT ,
    BEATDETECTOR_STATE_WAITING,
    BEATDETECTOR_STATE_FOLLOWING_SLOPE,
    BEATDETECTOR_STATE_MAYBE_DETECTED,
    BEATDETECTOR_STATE_MASKING
} BeatDetectorState;

typedef struct BeatDetector{
	BeatDetectorState state;
	float threshold;
	float beatPeriod;
	float lastMaxValue;
	uint32_t tsLastBeat;
} BeatDetector;

void BeatDetector_Init();
char BeatDetector_addSample(float sample); //bool
float BeatDetector_getRate();
float BeatDetector_getCurrentThreshold();

char BeatDetector_checkForBeat(float value);//bool
void BeatDetector_decreaseThreshold();
//BeatDetector--------------------------------------------------------------------------------------------

//MAX30100------------------------------------------------------------------------------------------------
#define DEFAULT_MODE                MAX30100_MODE_HRONLY
#define DEFAULT_SAMPLING_RATE       MAX30100_SAMPRATE_100HZ
#define DEFAULT_PULSE_WIDTH         MAX30100_SPC_PW_1600US_16BITS
#define DEFAULT_RED_LED_CURRENT     MAX30100_LED_CURR_50MA
#define DEFAULT_IR_LED_CURRENT      MAX30100_LED_CURR_50MA

#define THREE_FIFTHS_RED_LED_CURRENT        MAX30100_LED_CURR_30_6MA
#define THREE_FIFTHS_IR_LED_CURRENT         MAX30100_LED_CURR_30_6MA

#define I2C_BUS_SPEED               400000UL

typedef struct MAX30100{
	uint16_t rawIRValue;
	uint16_t rawRedValue;
}MAX30100;

void MAX30100_begin();
void MAX30100_setMode(Mode mode);
void MAX30100_setLedsPulseWidth(LEDPulseWidth ledPulseWidth);
void MAX30100_setSamplingRate(SamplingRate samplingRate);
void MAX30100_setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent);
void MAX30100_setHighresModeEnabled(char/*bool*/ enabled);
void MAX30100_update();

uint8_t MAX30100_readRegister(uint8_t address);
void MAX30100_writeRegister(uint8_t address, uint8_t data);
void MAX30100_burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length);
void MAX30100_readFifoData();
//MAX30100------------------------------------------------------------------------------------------------
	
//SpO2Calculator------------------------------------------------------------------------------------------
#define CALCULATE_EVERY_N_BEATS         3

typedef struct SpO2Calculator{
	uint8_t spO2LUT[43];

	float irACValueSqSum;
	float redACValueSqSum;
	uint8_t beatsDetectedNum;
	uint32_t samplesRecorded;
	uint8_t spO2;
}SpO2Calculator;

void SpO2Calculator_Init();

void SpO2Calculator_update(float irACValue, float redACValue, char/*bool*/ beatDetected);
void SpO2Calculator_reset();
uint8_t SpO2Calculator_getSpO2();
//SpO2Calculator------------------------------------------------------------------------------------------

//DCRemover-----------------------------------------------------------------------------------------------
typedef struct DCRemover{
    float alpha;
	float dcw;
}DCRemover;

void DCRemover_Init(char type, float alpha);
float DCRemover_step(char type, float x);
float DCRemover_getDCW(char type);
//DCRemover-----------------------------------------------------------------------------------------------

//FilterBuLp1---------------------------------------------------------------------------------------------
typedef struct FilterBuLp1{
    float v[2];
}FilterBuLp1;

void FilterBuLp1_Init();
float FilterBuLp1_step(float x);
//FilterBuLp1---------------------------------------------------------------------------------------------

//PulseOximeter-------------------------------------------------------------------------------------------
#define SAMPLING_FREQUENCY                  100
#define CURRENT_ADJUSTMENT_PERIOD_MS        500
#define IR_LED_CURRENT                      MAX30100_LED_CURR_50MA
#define RED_LED_CURRENT_START               MAX30100_LED_CURR_27_1MA
#define DC_REMOVER_ALPHA                    0.95
#define TEMPERATURE_SAMPLING_PERIOD_MS      2000

typedef enum PulseOximeterState {
    PULSEOXIMETER_STATE_INIT,
    PULSEOXIMETER_STATE_IDLE,
    PULSEOXIMETER_STATE_DETECTING
} PulseOximeterState;

typedef enum PulseOximeterDebuggingMode {
    PULSEOXIMETER_DEBUGGINGMODE_NONE,
    PULSEOXIMETER_DEBUGGINGMODE_RAW_VALUES,
    PULSEOXIMETER_DEBUGGINGMODE_AC_VALUES,
    PULSEOXIMETER_DEBUGGINGMODE_PULSEDETECT
} PulseOximeterDebuggingMode;

typedef struct PulseOximeter{
    PulseOximeterState state;
    PulseOximeterDebuggingMode debuggingMode;
    uint32_t tsFirstBeatDetected;
    uint32_t tsLastBeatDetected;
    uint32_t tsLastSample;
    uint32_t tsLastBiasCheck;
    uint32_t tsLastCurrentAdjustment;
    uint32_t tsLastTemperaturePoll;
    uint8_t redLedPower;
    
    BeatDetector *beatDetector;
    DCRemover *irDCRemover;
    DCRemover *redDCRemover;
    FilterBuLp1 *lpf;
    SpO2Calculator *spO2calculator;
    MAX30100 *hrm;

    //void (*onBeatDetected)();
    
    uint64 currentMinute;
} PulseOximeter;

PulseOximeter pulseOximeter;
BeatDetector globalBeatDetector;
DCRemover globalIrDCRemover;
DCRemover globalRedDCRemover;
FilterBuLp1 globalLpf;
SpO2Calculator globalSpO2calculator;
MAX30100 globalHrm;

void PulseOximeter_Init();
void PulseOximeter_begin(PulseOximeterDebuggingMode debuggingMode);//_=PULSEOXIMETER_DEBUGGINGMODE_NONE);
void PulseOximeter_update();
float PulseOximeter_getHeartRate();
uint8_t PulseOximeter_getSpO2();
uint8_t PulseOximeter_getRedLedCurrentBias();
void PulseOximeter_setOnBeatDetectedCallback(void (*cb)());

void PulseOximeter_checkSample();
void PulseOximeter_checkCurrentBias();
//void PulseOximeter_checkTemperature();

long millis();
//void (*onBeatDetected)();

//PulseOximeter-------------------------------------------------------------------------------------------

int numBeats;// = 0;

#endif