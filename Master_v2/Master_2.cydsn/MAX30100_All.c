#include <MAX30100_All.h>

double min(double a, double b) {
    return a<b ? a : b;
}

void BeatDetector_Init(){ 
    pulseOximeter.beatDetector->state = BEATDETECTOR_STATE_INIT;// BEATDETECTOR_STATE_INIT;
    pulseOximeter.beatDetector->threshold = BEATDETECTOR_MIN_THRESHOLD;
    pulseOximeter.beatDetector->beatPeriod = 0;
    pulseOximeter.beatDetector->lastMaxValue = 0;
    pulseOximeter.beatDetector->tsLastBeat = 0;
}

char BeatDetector_addSample(float sample)
{
    return BeatDetector_checkForBeat(sample);
}

float BeatDetector_getRate(){
    if (pulseOximeter.beatDetector->beatPeriod != 0) {
        return 1 / pulseOximeter.beatDetector->beatPeriod * 1000 * 60;
    } else {
        return 0;
    }
}

float BeatDetector_getCurrentThreshold(){
    return pulseOximeter.beatDetector->threshold;
}

char BeatDetector_checkForBeat(float sample){
    char beatDetected = 0;

    switch (pulseOximeter.beatDetector->state) {
        case BEATDETECTOR_STATE_INIT:
            if (millis() > BEATDETECTOR_INIT_HOLDOFF) {
                pulseOximeter.beatDetector->state = BEATDETECTOR_STATE_WAITING;
            }
            break;

        case BEATDETECTOR_STATE_WAITING:
            if (sample > pulseOximeter.beatDetector->threshold) {
                pulseOximeter.beatDetector->threshold = min(sample, BEATDETECTOR_MAX_THRESHOLD);
                pulseOximeter.beatDetector->state = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
            }

            // Tracking lost, resetting
            if (millis() - pulseOximeter.beatDetector->tsLastBeat > BEATDETECTOR_INVALID_READOUT_DELAY) {
                pulseOximeter.beatDetector->beatPeriod = 0;
                pulseOximeter.beatDetector->lastMaxValue = 0;
            }

            BeatDetector_decreaseThreshold();
            break;

        case BEATDETECTOR_STATE_FOLLOWING_SLOPE:
            if (sample < pulseOximeter.beatDetector->threshold) {
                pulseOximeter.beatDetector->state = BEATDETECTOR_STATE_MAYBE_DETECTED;
            } else {
                pulseOximeter.beatDetector->threshold = min(sample, BEATDETECTOR_MAX_THRESHOLD);
            }
            break;

        case BEATDETECTOR_STATE_MAYBE_DETECTED:
            if (sample + BEATDETECTOR_STEP_RESILIENCY < pulseOximeter.beatDetector->threshold) {
                // Found a beat
                beatDetected = 1;
                pulseOximeter.beatDetector->lastMaxValue = sample;
                pulseOximeter.beatDetector->state = BEATDETECTOR_STATE_MASKING;
                float delta = millis() - pulseOximeter.beatDetector->tsLastBeat;
                
                if (delta) {
                    pulseOximeter.beatDetector->beatPeriod = BEATDETECTOR_BPFILTER_ALPHA * delta + (1 - BEATDETECTOR_BPFILTER_ALPHA) * pulseOximeter.beatDetector->beatPeriod;
                }

                pulseOximeter.beatDetector->tsLastBeat = millis();
            } else {
                pulseOximeter.beatDetector->state = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
            }
            break;

        case BEATDETECTOR_STATE_MASKING:
            if (millis() - pulseOximeter.beatDetector->tsLastBeat > BEATDETECTOR_MASKING_HOLDOFF) {
                pulseOximeter.beatDetector->state = BEATDETECTOR_STATE_WAITING;
            }
            BeatDetector_decreaseThreshold();
            break;
    }

    return beatDetected;
}

void BeatDetector_decreaseThreshold(){
    // When a valid beat rate readout is present, target the
    if (pulseOximeter.beatDetector->lastMaxValue > 0 && pulseOximeter.beatDetector->beatPeriod > 0) {
        pulseOximeter.beatDetector->threshold -= pulseOximeter.beatDetector->lastMaxValue * (1 - BEATDETECTOR_THRESHOLD_FALLOFF_TARGET) /
                (pulseOximeter.beatDetector->beatPeriod / BEATDETECTOR_SAMPLES_PERIOD);
    } else {
        // Asymptotic decay
        pulseOximeter.beatDetector->threshold *= BEATDETECTOR_THRESHOLD_DECAY_FACTOR;
    }

    if (pulseOximeter.beatDetector->threshold < BEATDETECTOR_MIN_THRESHOLD) {
        pulseOximeter.beatDetector->threshold = BEATDETECTOR_MIN_THRESHOLD;
    }
}

void MAX30100_begin(){
	//Start I2C
	//Set I2C bus speed
	
	MAX30100_setMode(DEFAULT_MODE);
	MAX30100_setMode(DEFAULT_MODE);
    MAX30100_setLedsPulseWidth(DEFAULT_PULSE_WIDTH);
    MAX30100_setSamplingRate(DEFAULT_SAMPLING_RATE);
    MAX30100_setLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);
    MAX30100_setHighresModeEnabled(1);
	
}

void MAX30100_setMode(Mode mode){
    MAX30100_writeRegister(MAX30100_REG_MODE_CONFIGURATION, mode);
}

void MAX30100_setLedsPulseWidth(LEDPulseWidth ledPulseWidth){
    uint8_t previous = MAX30100_readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    MAX30100_writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);
}

void MAX30100_setSamplingRate(SamplingRate samplingRate){
    uint8_t previous = MAX30100_readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    MAX30100_writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));
}

void MAX30100_setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent){
    MAX30100_writeRegister(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);
}

void MAX30100_setHighresModeEnabled(char enabled){
    uint8_t previous = MAX30100_readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    if (enabled) {
        MAX30100_writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
    } else {
        MAX30100_writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
    }
}

void MAX30100_update(){
    MAX30100_readFifoData();
}

uint8_t MAX30100_readRegister(uint8_t address){
    /*Wire.beginTransmission(MAX30100_I2C_ADDRESS);
    Wire.write(address);
    Wire.endTransmission(false);
    Wire.requestFrom(MAX30100_I2C_ADDRESS, 1);

    return Wire.read();*/
    
    uint8 Write_Buf[1] = {0}; //Buffer that will contain the register that will be read from
    Write_Buf[0] = address;
    
    uint8 Read_Buf[1]={0};  //Buffer that will store the value read from the register
    
    //Step 1
    I2C_1_I2CMasterWriteBuf(MAX30100_I2C_ADDRESS, (uint8 *)Write_Buf, 1, I2C_1_I2C_MODE_NO_STOP);
    while((I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_WR_CMPLT)==0){}
    
    //Step 2
    I2C_1_I2CMasterReadBuf(MAX30100_I2C_ADDRESS, (uint8 *)Read_Buf, 1, I2C_1_I2C_MODE_REPEAT_START);
    while((I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_RD_CMPLT)==0){}
    
    return Read_Buf[0];
}

void MAX30100_writeRegister(uint8_t address, uint8_t data){
    /*Wire.beginTransmission(MAX30100_I2C_ADDRESS);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();*/
    
    uint8 Write_Buf[2]={0};
    Write_Buf[0] = address;          //Assign the first element to be the register you want to write to (Parameter 1)
    Write_Buf[1] = data;             //Assign the second elemnt to be the value you wish to write to the register (Parameter 2)
    
    
    I2C_1_I2CMasterWriteBuf(MAX30100_I2C_ADDRESS, (uint8 *)Write_Buf, 2, I2C_1_I2C_MODE_COMPLETE_XFER);
    while((I2C_1_I2CMasterStatus()&I2C_1_I2C_MSTAT_WR_CMPLT)==0);
        
    return;
}

void MAX30100_burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length){
    /*Wire.beginTransmission(MAX30100_I2C_ADDRESS);
    Wire.write(baseAddress);
    Wire.endTransmission(0);
    Wire.requestFrom((uint8_t)MAX30100_I2C_ADDRESS, length);

    uint8_t idx = 0;
    while (Wire.available()) {
        buffer[idx++] = Wire.read();
    }*/
    
    /*int i = 0;
    for(; i < length; i++){
        buffer[i++] = MAX30100_readRegister(baseAddress);
    }*/
    
    uint8 Write_Buf[1] = {0}; //Buffer that will contain the register that will be read from
    Write_Buf[0] = baseAddress;
    
    //uint8 Read_Buf[1] ={0};  //Buffer that will store the value read from the register
    
    //Step 1
    I2C_1_I2CMasterWriteBuf(MAX30100_I2C_ADDRESS, (uint8 *)Write_Buf, 1, I2C_1_I2C_MODE_NO_STOP);
    while((I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_WR_CMPLT)==0){}
    
    //Step 2
    I2C_1_I2CMasterReadBuf(MAX30100_I2C_ADDRESS, buffer, length, I2C_1_I2C_MODE_REPEAT_START);
    while((I2C_1_I2CMasterStatus() & I2C_1_I2C_MSTAT_RD_CMPLT)==0){}
}

void MAX30100_readFifoData(){
    uint8_t buffer[4];

    MAX30100_burstRead(MAX30100_REG_FIFO_DATA, buffer, 4);

    // Warning: the values are always left-aligned
    pulseOximeter.hrm->rawIRValue = (buffer[0] << 8) | buffer[1];
    pulseOximeter.hrm->rawRedValue = (buffer[2] << 8) | buffer[3];
}

// SaO2 Look-up Table
// http://www.ti.com/lit/an/slaa274b/slaa274b.pdf


void SpO2Calculator_Init(){
    /*spO2calculator.spO2LUT[43] = {100,100,100,100,99,99,99,99,99,99,98,98,98,98,
                                             98,97,97,97,97,97,97,96,96,96,96,96,96,95,95,
                                             95,95,95,95,94,94,94,94,94,93,93,93,93,93};*/
    int i = 0;
    for(; i < 4; i++)
        pulseOximeter.spO2calculator->spO2LUT[i] = 100;
    for(; i < 10; i++)
        pulseOximeter.spO2calculator->spO2LUT[i] = 99;
    for(; i < 15; i++)
        pulseOximeter.spO2calculator->spO2LUT[i] = 98;
    for(; i < 21; i++)
        pulseOximeter.spO2calculator->spO2LUT[i] = 97;
    for(; i < 27; i++)
        pulseOximeter.spO2calculator->spO2LUT[i] = 96;
    for(; i < 33; i++)
        pulseOximeter.spO2calculator->spO2LUT[i] = 95;
    for(; i < 38; i++)
        pulseOximeter.spO2calculator->spO2LUT[i] = 94;
    for(; i < 43; i++)
        pulseOximeter.spO2calculator->spO2LUT[i] = 93;
    
    pulseOximeter.spO2calculator->irACValueSqSum = 0;
    pulseOximeter.spO2calculator->redACValueSqSum = 0;
    pulseOximeter.spO2calculator->beatsDetectedNum = 0;
    pulseOximeter.spO2calculator->samplesRecorded = 0;
    pulseOximeter.spO2calculator->spO2 = 0;
}

void SpO2Calculator_update(float irACValue, float redACValue, char beatDetected){
    pulseOximeter.spO2calculator->irACValueSqSum += irACValue * irACValue;
    pulseOximeter.spO2calculator->redACValueSqSum += redACValue * redACValue;
    ++(pulseOximeter.spO2calculator->samplesRecorded);

    if (beatDetected) {
        ++(pulseOximeter.spO2calculator->beatsDetectedNum);
        if (pulseOximeter.spO2calculator->beatsDetectedNum == CALCULATE_EVERY_N_BEATS) {
            float acSqRatio = 
                100.0 * 
                log(pulseOximeter.spO2calculator->redACValueSqSum/pulseOximeter.spO2calculator->samplesRecorded) / 
                log(pulseOximeter.spO2calculator->irACValueSqSum/pulseOximeter.spO2calculator->samplesRecorded);
            //float acSqRatio = 60;
            uint8_t index = 0;

            if (acSqRatio > 66) {
                index = (uint8_t)acSqRatio - 66;
            } else if (acSqRatio > 50) {
                index = (uint8_t)acSqRatio - 50;
            }
            SpO2Calculator_reset(pulseOximeter.spO2calculator);

            pulseOximeter.spO2calculator->spO2 = pulseOximeter.spO2calculator->spO2LUT[index];
        }
    }
}

void SpO2Calculator_reset(){
    pulseOximeter.spO2calculator->samplesRecorded = 0;
    pulseOximeter.spO2calculator->redACValueSqSum = 0;
    pulseOximeter.spO2calculator->irACValueSqSum = 0;
    pulseOximeter.spO2calculator->beatsDetectedNum = 0;
    pulseOximeter.spO2calculator->spO2 = 0;
}

uint8_t SpO2Calculator_getSpO2(){
    return pulseOximeter.spO2calculator->spO2;
}

void DCRemover_Init(char type, float alpha){
    if(type == 'i'){
        pulseOximeter.irDCRemover->alpha = alpha;
        pulseOximeter.irDCRemover->dcw = 0;
    }
    else if(type =='r'){
        pulseOximeter.redDCRemover->alpha = alpha;
        pulseOximeter.redDCRemover->dcw = 0;
    }
}
float DCRemover_step(char type, float x){
    if(type == 'i'){
        float olddcw = pulseOximeter.irDCRemover->dcw;
	    pulseOximeter.irDCRemover->dcw = (float)x + pulseOximeter.irDCRemover->alpha * pulseOximeter.irDCRemover->dcw;
        return pulseOximeter.irDCRemover->dcw - olddcw;
    }
    else if(type == 'r'){
        float olddcw = pulseOximeter.redDCRemover->dcw;
	    pulseOximeter.redDCRemover->dcw = (float)x + pulseOximeter.redDCRemover->alpha * pulseOximeter.redDCRemover->dcw;
        return pulseOximeter.redDCRemover->dcw - olddcw;
    }

    return 0.0;
}

float DCRemover_getDCW(char type){
    if(type == 'i')
        return pulseOximeter.irDCRemover->dcw;
    else if(type == 'r')
        return pulseOximeter.redDCRemover->dcw;
    
    return 0.0;
}

void FilterBuLp1_Init(){
    pulseOximeter.lpf->v[0] = 0.0;
}

float FilterBuLp1_step(float x){
    pulseOximeter.lpf->v[0] = pulseOximeter.lpf->v[1];
	pulseOximeter.lpf->v[1] = (2.452372752527856026e-1 * x) + (0.50952544949442879485 * pulseOximeter.lpf->v[0]);
	
    return (pulseOximeter.lpf->v[0] + pulseOximeter.lpf->v[1]);
}

void PulseOximeter_Init(){
    pulseOximeter.state = PULSEOXIMETER_STATE_INIT;
    pulseOximeter.tsFirstBeatDetected = 0;
    pulseOximeter.tsLastBeatDetected = 0;
    pulseOximeter.tsLastSample = 0;
    pulseOximeter.tsLastBiasCheck = 0;
    pulseOximeter.tsLastCurrentAdjustment = 0;
    pulseOximeter.tsLastTemperaturePoll = 0;
    pulseOximeter.redLedPower = (uint8_t)RED_LED_CURRENT_START,
    //onBeatDetected(NULL);
    
    pulseOximeter.beatDetector = &globalBeatDetector;
    pulseOximeter.irDCRemover = &globalIrDCRemover;
    pulseOximeter.redDCRemover = &globalRedDCRemover;
    pulseOximeter.lpf = &globalLpf;
    pulseOximeter.spO2calculator = &globalSpO2calculator;
    pulseOximeter.hrm = &globalHrm;
    
    BeatDetector_Init();
    FilterBuLp1_Init();
    SpO2Calculator_Init();
    
    pulseOximeter.currentMinute = 0;
    numBeats = 0;
}

long millis(){
    return (pulseOximeter.currentMinute * 60000) + (60000 - Timer_minute_ReadCounter());
}

void PulseOximeter_begin(PulseOximeterDebuggingMode debuggingMode_)
{
    pulseOximeter.debuggingMode = debuggingMode_;

    MAX30100_begin();
    MAX30100_setMode(MAX30100_MODE_SPO2_HR);
    MAX30100_setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT_START);
    
    DCRemover_Init('i', DC_REMOVER_ALPHA);
    DCRemover_Init('r', DC_REMOVER_ALPHA);

    pulseOximeter.state = PULSEOXIMETER_STATE_IDLE;
}

void PulseOximeter_update()
{
    PulseOximeter_checkSample(pulseOximeter);
    PulseOximeter_checkCurrentBias(pulseOximeter);
}

float PulseOximeter_getHeartRate()
{
    return BeatDetector_getRate();
}

uint8_t PulseOximeter_getSpO2()
{
    return SpO2Calculator_getSpO2();
}

uint8_t PulseOximeter_getRedLedCurrentBias()
{
    return pulseOximeter.redLedPower;
}

void PulseOximeter_setOnBeatDetectedCallback(void (*cb)())
{
    //onBeatDetected = cb;
}

void PulseOximeter_checkSample(){
    if (millis() - pulseOximeter.tsLastSample > 1.0 / SAMPLING_FREQUENCY * 1000.0) {
        pulseOximeter.tsLastSample = millis();
        MAX30100_update();
        float irACValue = DCRemover_step('i', pulseOximeter.hrm->rawIRValue);
        float redACValue = DCRemover_step('r', pulseOximeter.hrm->rawRedValue);

        // The signal fed to the beat detector is mirrored since the cleanest monotonic spike is below zero
        float filteredPulseValue = FilterBuLp1_step(-irACValue);
        char beatDetected = BeatDetector_addSample(filteredPulseValue);

        if (BeatDetector_getRate() > 0) {
            pulseOximeter.state = PULSEOXIMETER_STATE_DETECTING;
            SpO2Calculator_update(irACValue, redACValue, beatDetected);
        } else if (pulseOximeter.state == PULSEOXIMETER_STATE_DETECTING) {
            pulseOximeter.state = PULSEOXIMETER_STATE_IDLE;
            SpO2Calculator_reset();
        }

/*        switch (pulseOximeter.debuggingMode) {
            case PULSEOXIMETER_DEBUGGINGMODE_RAW_VALUES:
                Serial.print("R:");
                Serial.print(hrm.rawIRValue);
                Serial.print(",");
                Serial.println(hrm.rawRedValue);
                break;

            case PULSEOXIMETER_DEBUGGINGMODE_AC_VALUES:
                Serial.print("R:");
                Serial.print(irACValue);
                Serial.print(",");
                Serial.println(redACValue);
                break;

            case PULSEOXIMETER_DEBUGGINGMODE_PULSEDETECT:
                Serial.print("R:");
                Serial.print(filteredPulseValue);
                Serial.print(",");
                Serial.println(BeatDetector_getCurrentThreshold());
                break;

            default:
                break;
        }*/

        if (beatDetected){// && onBeatDetected) {
            //onBeatDetected();
            numBeats++;
        }
    }
}

void PulseOximeter_checkCurrentBias()
{
    // Follower that adjusts the red led current in order to have comparable DC baselines between
    // red and IR leds. The numbers are really magic: the less possible to avoid oscillations
    if (millis() - pulseOximeter.tsLastBiasCheck > CURRENT_ADJUSTMENT_PERIOD_MS) {
        char changed = 0;
        if (DCRemover_getDCW('i') - DCRemover_getDCW('r') > 70000 && pulseOximeter.redLedPower < MAX30100_LED_CURR_50MA) {
            ++(pulseOximeter.redLedPower);
            changed = 1;
        } else if (DCRemover_getDCW('r') - DCRemover_getDCW('i') > 70000 && pulseOximeter.redLedPower > 0) {
            --(pulseOximeter.redLedPower);
            changed = 1;
        }

        if (changed) {
            MAX30100_setLedsCurrent(IR_LED_CURRENT, (LEDCurrent)pulseOximeter.redLedPower);
            pulseOximeter.tsLastCurrentAdjustment = millis();
        }

        pulseOximeter.tsLastBiasCheck = millis();
    }
}