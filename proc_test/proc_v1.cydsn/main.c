/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <accelerometer.h>
#include <temp_bat_button.h>
#include <MAX30100_All.h>

#define REPORTING_PERIOD_MS     1000

int NOTIFY[] = {0, 0, 0, 0, 0, 0};

uint8 deviceConnected ; 
uint8 restartAdvertisement; 
uint8 busyStatus; 
uint8 whatToDo;
uint8 started;
int charCode;
CYBLE_CONN_HANDLE_T connectionHandle; 

uint32 tsLastReport = 0;
uint32 otherLastReport = 0; //last report time for all other sensors besides MAX30100

const int ACCELEROMETER = 0;
const int BATTERY = 1;
const int BUTTON = 2;
const int HEART = 3;
const int OXYGEN = 4;
const int TEMPERATURE = 5;

void initBLE();
void StackEventHandler( uint32 eventCode, void *eventParam );

CY_ISR(Minute_Handler){
    pulseOximeter.currentMinute++;
    Timer_minute_ClearInterrupt(Timer_minute_INTR_MASK_TC);
}

void initAll(){
    initBLE();                                                  //initialize bluetooth
    I2C_1_Start();                                              //start I2C
    initAccelControl();                                         //initialize and setup acclerometer
    initTempBatButton();                                        //useless
    
    PulseOximeter_Init();                                       //setup pulse oximeter
    PulseOximeter_begin(PULSEOXIMETER_DEBUGGINGMODE_NONE);      //start pulse oximeter

    Timer_minute_Start();                                       //start minute timer
    isr_minute_StartEx(Minute_Handler);                         //setup isr
    
    ADC_Start();                                                //start the ADC
    ADC_StartConvert();                                         //start converting values
}

//initializes the BLE module
void initBLE(){
    CyBle_Start( StackEventHandler );   //starts the event handler
    deviceConnected = 0; 
    restartAdvertisement = 0 ; 
    busyStatus = 0; 
    whatToDo = 0;
    started = 0;
}

void StackEventHandler( uint32 eventCode, void *eventParam )
{
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
    
    switch( eventCode ){
        case CYBLE_EVT_STACK_ON:       
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:            
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            deviceConnected = 0;
            break;

        case CYBLE_EVT_GATT_CONNECT_IND:
            connectionHandle = *(CYBLE_CONN_HANDLE_T*)eventParam;
            deviceConnected = 1; 
            break;

        case CYBLE_EVT_GATT_DISCONNECT_IND:
            deviceConnected = 0;
            uint32 i = 0;
            for(; i < sizeof(NOTIFY)/sizeof(int); i++)
                NOTIFY[i] = 0;
            break;
            
        case CYBLE_EVT_GATTS_WRITE_REQ:				
			/*When this event is triggered, the peripheral has received 
			* a write command on the custom characteristic */
			
			/* Extract the write value from the event parameter */
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
            
            charCode = -1;
            
            //Depending on the value of event parameter do different tasks
            switch(wrReqParam->handleValPair.attrHandle){
                case CYBLE_BIOSNAP_TEMPERATURE_TEMPCCC_DESC_HANDLE:
                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    NOTIFY[TEMPERATURE] = wrReqParam->handleValPair.value.val[0] & 0x01; 
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                break;
                    
                case CYBLE_BIOSNAP_ACCELEROMETER_ACCELCCC_DESC_HANDLE:
                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    NOTIFY[ACCELEROMETER] = wrReqParam->handleValPair.value.val[0] & 0x01; 
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                break;
                
                case CYBLE_BIOSNAP_OXYGEN_OXYGENCCC_DESC_HANDLE:
                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    NOTIFY[OXYGEN] = wrReqParam->handleValPair.value.val[0] & 0x01; 
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                break;
                    
                case CYBLE_BIOSNAP_HEART_HEARTCCC_DESC_HANDLE:
                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    NOTIFY[HEART] = wrReqParam->handleValPair.value.val[0] & 0x01; 
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                break;
                    
                case CYBLE_BIOSNAP_BATTERY_BATCCC_DESC_HANDLE:
                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    NOTIFY[BATTERY] = wrReqParam->handleValPair.value.val[0] & 0x01; 
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                break;
                    
                case CYBLE_BIOSNAP_BUTTON_BUTTONCCC_DESC_HANDLE:
                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    NOTIFY[BUTTON] = wrReqParam->handleValPair.value.val[0] & 0x01; 
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                break;
            }
            break; 
        
        /* default catch-all case */
        case CYBLE_EVT_STACK_BUSY_STATUS:
            busyStatus = *(uint8*) eventParam;
            
        default:
        break;
        
    }
}

void updateCharacteristic(int charCode, int charHandle, uint32 value){
    if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
   
    CYBLE_GATT_HANDLE_VALUE_PAIR_T CCCDHandle; 

    //update characteristic handle with the correct information
    CCCDHandle.attrHandle = charHandle;
    CCCDHandle.value.val = (uint8 *)&value;
    CCCDHandle.value.len = 4;
    
    //send out new characteristic value
    CyBle_GattsWriteAttributeValue(&CCCDHandle, 0, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
    CyBle_GattsReadAttributeValue(&CCCDHandle, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
   
    if(NOTIFY[charCode])
        CyBle_GattsNotification(cyBle_connHandle, &CCCDHandle);
}

//puts data into little endian for CySmart app
uint32 reverseUint32(uint32 num){
    return ((0xFF000000 & num) >> 24) | ((0x00FF0000 & num) >> 8) | ((0x0000FF00 & num) << 8) | ((0x000000FF & num) << 24);
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    initAll();
    
    //initialize all sensor calues to 0
    uint32 accel = 0;
    uint32 heart = 0;
    uint32 oxygen = 0;
    uint32 temp = 0;
    uint32 bat = 0;
    uint32 button = 0;
    
    long elapsed = 0;
    
    for(;;){
        
        CyBle_ProcessEvents();
        
        if(deviceConnected && busyStatus == CYBLE_STACK_STATE_FREE){ //check to make sure device is connected
            PulseOximeter_update();
            elapsed = millis() - tsLastReport;
            
            //every 100ms send out temperature, accelerometer, battery, and button data
            if(millis() - otherLastReport > 100){
                temp = getTemperature();
                updateCharacteristic(TEMPERATURE, CYBLE_BIOSNAP_TEMPERATURE_CHAR_HANDLE, temp);
            
                accel = getAccelXYZ();
                updateCharacteristic(ACCELEROMETER, CYBLE_BIOSNAP_ACCELEROMETER_CHAR_HANDLE, accel);
                
                bat = getBattery();
                updateCharacteristic(BATTERY, CYBLE_BIOSNAP_BATTERY_CHAR_HANDLE, bat);
                
                button = getButton();
                updateCharacteristic(BUTTON, CYBLE_BIOSNAP_BUTTON_CHAR_HANDLE, button);
                
                otherLastReport = millis();
            }
            //every 1000ms send out pulse oximeter data
            if (elapsed > REPORTING_PERIOD_MS) {
                oxygen = (uint32)(PulseOximeter_getSpO2());
                heart = (uint32)(PulseOximeter_getHeartRate());
                updateCharacteristic(HEART, CYBLE_BIOSNAP_HEART_CHAR_HANDLE, heart);
                updateCharacteristic(OXYGEN, CYBLE_BIOSNAP_OXYGEN_CHAR_HANDLE, oxygen);
                
                tsLastReport = millis();
            }

            
        }
//        CyDelay(10);
    }
}

/* [] END OF FILE */
