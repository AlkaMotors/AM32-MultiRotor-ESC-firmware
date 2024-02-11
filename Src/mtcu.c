///*
// * crsf.c
// *
// *  Created on: Jun 27, 2023
// *      Author: alkaz
// */
// crc code from https://github.com/stepinside/Arduino-CRSF/tree/main
//
#include "mtcu.h"
#include "targets.h"
#include "common.h"
#include "functions.h"
#include "signal.h"

uint8_t mtcu_buffer[100] = {0};
/*struct DataSentence
{                      // Serial commands should have five bytes (plus termination).
  uint8_t Address = 0; // We use a struct for convenience
  uint8_t Command = 0;
  uint8_t Value = 0;
  uint8_t Modifier = 0; // Unlike the Sabertooth and Scout protocols, the sentence for the sound card includes an extra byte we call Modifier.
  uint8_t Checksum = 0;
};*/
// uint16_t crsf_channels[16] = {0};
// uint32_t invalid_crc = 0;
//static DataSentence Sentence; // A struct to store incoming commands
#define AddressA 131          // Device has two possible addresses, set by dipswitch
#define AddressB 132          // These are within the range of addresses also used by Dimension Engineering Sabertooth devices (128-135)
uint8_t MyAddress = AddressA; // Address of device. Set by dipswitch.

/*
uint8_t crsf_crc8(const uint8_t *ptr, uint8_t len){
static const uint8_t crsf_crc8tab[256] = {
    0x00, 0xD5, 0x7F, 0xAA, 0xFE, 0x2B, 0x81, 0x54, 0x29, 0xFC, 0x56, 0x83, 0xD7, 0x02, 0xA8, 0x7D,
    0x52, 0x87, 0x2D, 0xF8, 0xAC, 0x79, 0xD3, 0x06, 0x7B, 0xAE, 0x04, 0xD1, 0x85, 0x50, 0xFA, 0x2F,
    0xA4, 0x71, 0xDB, 0x0E, 0x5A, 0x8F, 0x25, 0xF0, 0x8D, 0x58, 0xF2, 0x27, 0x73, 0xA6, 0x0C, 0xD9,
    0xF6, 0x23, 0x89, 0x5C, 0x08, 0xDD, 0x77, 0xA2, 0xDF, 0x0A, 0xA0, 0x75, 0x21, 0xF4, 0x5E, 0x8B,
    0x9D, 0x48, 0xE2, 0x37, 0x63, 0xB6, 0x1C, 0xC9, 0xB4, 0x61, 0xCB, 0x1E, 0x4A, 0x9F, 0x35, 0xE0,
    0xCF, 0x1A, 0xB0, 0x65, 0x31, 0xE4, 0x4E, 0x9B, 0xE6, 0x33, 0x99, 0x4C, 0x18, 0xCD, 0x67, 0xB2,
    0x39, 0xEC, 0x46, 0x93, 0xC7, 0x12, 0xB8, 0x6D, 0x10, 0xC5, 0x6F, 0xBA, 0xEE, 0x3B, 0x91, 0x44,
    0x6B, 0xBE, 0x14, 0xC1, 0x95, 0x40, 0xEA, 0x3F, 0x42, 0x97, 0x3D, 0xE8, 0xBC, 0x69, 0xC3, 0x16,
    0xEF, 0x3A, 0x90, 0x45, 0x11, 0xC4, 0x6E, 0xBB, 0xC6, 0x13, 0xB9, 0x6C, 0x38, 0xED, 0x47, 0x92,
    0xBD, 0x68, 0xC2, 0x17, 0x43, 0x96, 0x3C, 0xE9, 0x94, 0x41, 0xEB, 0x3E, 0x6A, 0xBF, 0x15, 0xC0,
    0x4B, 0x9E, 0x34, 0xE1, 0xB5, 0x60, 0xCA, 0x1F, 0x62, 0xB7, 0x1D, 0xC8, 0x9C, 0x49, 0xE3, 0x36,
    0x19, 0xCC, 0x66, 0xB3, 0xE7, 0x32, 0x98, 0x4D, 0x30, 0xE5, 0x4F, 0x9A, 0xCE, 0x1B, 0xB1, 0x64,
    0x72, 0xA7, 0x0D, 0xD8, 0x8C, 0x59, 0xF3, 0x26, 0x5B, 0x8E, 0x24, 0xF1, 0xA5, 0x70, 0xDA, 0x0F,
    0x20, 0xF5, 0x5F, 0x8A, 0xDE, 0x0B, 0xA1, 0x74, 0x09, 0xDC, 0x76, 0xA3, 0xF7, 0x22, 0x88, 0x5D,
    0xD6, 0x03, 0xA9, 0x7C, 0x28, 0xFD, 0x57, 0x82, 0xFF, 0x2A, 0x80, 0x55, 0x01, 0xD4, 0x7E, 0xAB,
    0x84, 0x51, 0xFB, 0x2E, 0x7A, 0xAF, 0x05, 0xD0, 0xAD, 0x78, 0xD2, 0x07, 0x53, 0x86, 0x2C, 0xF9};

uint8_t crc = 0;
for (uint8_t i = 0; i < len; i++)
{
    crc = crsf_crc8tab[crc ^ *ptr++];
}
return crc;
}*/
/*if (ReadData(&Sentence))
{
    ProcessCommand(&Sentence); // Do whatever we're told
    //TimeLastSerial = millis(); // Save the time
    // SendTelemetry(TotalCurrent_mA);
}

// -------------------------------------------------------------------------------------------------------------------------------------------------->
// SERIAL COMMANDS - Check the serial port, do whatever we're told
// -------------------------------------------------------------------------------------------------------------------------------------------------->
uint8_t ReadData(DataSentence *sentence)
{
  //byte ByteIn;
  uint8_t addressReceived = 0; // Have we received a byte that matches our address
  //bool longAddressReceived = false;
  static char input_line[SENTENCE_BYTES];
  //static char input_line_long[LONG_SENTENCE_BYTES];
  static uint8_t numBytes = 0; // Start off with no data received
  //static uint8_t longNumBytes = 0;

  uint8_t SentenceReceived = 0; // Start off false, will get set to true if a valid sentence was received

  // Read all the bytes that are available, starting with the first byte that matches our address
  while (Serial1.available())
  {
    ByteIn = Serial1.read();
#ifdef debug
    Serial.println(ByteIn);
#endif
    if (ByteIn == MyAddress)
    {
      addressReceived = true; // Matching address
      input_line[0] = ByteIn; // Save it in our array
      numBytes = 1;           // Subsequent bytes will be added to the array until we have enough to compare against INIT_STRING
    }
    else if (addressReceived)
    {
      input_line[numBytes++] = ByteIn;
      if (numBytes >= SENTENCE_BYTES)
        break; // We have enough bytes for a full sentence, so evaluate it
    }
    else if (ByteIn == 0xDA)
      {
       longAddressReceived = true;         // Matching address
       input_line_long[0] = ByteIn;         // Save it in our array
       longNumBytes = 1;                   // Subsequent bytes will be added to the array until we have enough to compare against INIT_STRING
      }
      else if (longAddressReceived)
      {
       input_line_long[longNumBytes++] = ByteIn;
       if (longNumBytes >= LONG_SENTENCE_BYTES) break;  // We have enough bytes for a full sentence, so evaluate it
      }
  }
  if (mtcu_buffer[0] == MyAddress)
  {
    numBytes = 1;
    for(int i = 0; i<= SENTENCE_BYTES;i++)
    {
        numBytes++;
        input_line[i] = mtcu_buffer[i];
    }
  }


  // If we have enough bytes for a full sentence, save it
  if (numBytes >= SENTENCE_BYTES)
  // if (input_line[0] == MyAddress)
  { // We have enough bytes for a full sentence
    // Serial1.println("S");
    sentence->Address = input_line[0];
    sentence->Command = input_line[1];
    mtcu_buffer[2] = input_line[2];
    sentence->Checksum = input_line[3];

    // Now verify the checksum
    if (ChecksumValid(sentence))
    {
      SentenceReceived = 1; // Yes, a valid sentence has been received!
    }

    // Start everything over
    input_line[0] = '\0';
    addressReceived = 0;
    numBytes = 0;
  }
  if (longNumBytes >= LONG_SENTENCE_BYTES)
    //if (input_line_long[0] == 0xDA)
    { // We have enough bytes for a full sentence
    //Serial1.println("L");
    sentence->Address  = input_line_long[0];
    sentence->Command  = input_line_long[1];
    mtcu_buffer[2]    = input_line_long[2];
    sentence->Modifier = input_line_long[3];
    sentence->Checksum = input_line_long[4];

    // Now verify the checksum
    if (longChecksumValid(sentence))
    {
      SentenceReceived = true;        // Yes, a valid sentence has been received!
    }

    // Start everything over
    input_line_long[0] = '\0';
    longAddressReceived = false;
    longNumBytes = 0;
    }
  return SentenceReceived;
}

uint8_t ChecksumValid(DataSentence *sentence)
{
  uint8_t check = (sentence->Address + sentence->Command + mtcu_buffer[2]) & B01111111;

  if (check == sentence->Checksum)
    return 1;
  else
    return 0;
}

bool longChecksumValid(DataSentence *sentence)
{
  uint8_t check = (sentence->Address + sentence->Command + mtcu_buffer[2] + sentence->Modifier) & B01111111;

  if (check == sentence->Checksum)
    return true;
  else
    return false;
}

void ProcessCommand(DataSentence *sentence)
{
*/
void setChannelsMTCU()
{
    uint8_t validData = 0;
    if (mtcu_buffer[0] == MyAddress)
    {
        uint8_t check = (mtcu_buffer[0] + mtcu_buffer[1] + mtcu_buffer[2]) & 0x7F;
        if (mtcu_buffer[3] == check)
        {
            validData = 1;
        }
        /*for(int i = 0; i<= SENTENCE_BYTES;i++)

        {
            input_line[i] = mtcu_buffer[i];
        }*/
    }
    //uint8_t val;
    /*Serial1.print(sentence->Address);
      Serial1.print(" ");
      Serial1.print(sentence->Command);
      Serial1.print(" ");
      Serial1.print(mtcu_buffer[2]);
      Serial1.print(" ");
      Serial1.println(sentence->Modifier);*/
    // Serial1.println(val);
    //  Address bytes have values greater than 127
    if (validData == 1)
    {
        switch (mtcu_buffer[1])
        {
        case 0:
            // Motor 1 Forward
            newinput = map(mtcu_buffer[2], 0, 127, 1000, 2000);
            // setSpeed(1, getSpeedCommand_fromSerial(mtcu_buffer[2]));
            break;

        case 1:
            // Motor 1 Reverse
            newinput = map(mtcu_buffer[2], 0, 127, 1000, 0);
            // setSpeed(1, -getSpeedCommand_fromSerial(mtcu_buffer[2]));
            break;

        case 2:
            // Set minimum voltage
            // Used to set a custom minimum voltage for the battery supplying power (essentially LVC level). If battery voltage drops below this value,
            // motors will be turned off. This value is cleared at startup, so must be set each run. The value is sent in .2 volt
            // increments with a command of zero corresponding to 6v, which is the minimum. Valid data is from 0 to 50 (6-16 volts).
            // The function for converting volts to command data is Value = (desired volts-6) x 5

            // If valid value sent, update MinVoltage
            if (mtcu_buffer[2] <= 50)
            {
                // MinVoltage = 6.0 + ((float)mtcu_buffer[2] * 0.2);
            }
            break;

        case 3:
            // If voltage exceeds the value set here, the motors will be turned off, however the valute set here can also not exceed AbsMaxVoltage which is
            // determined by the hardware on the board. MaxVoltage is cleared at startup (and set to default of AbsMaxVoltage) so must be re-set by serial
            // at each boot. The value is sent in 0.2 volt increments with valid values being:
            // 30 to 140 (6 volts to 28 volts)
            // However again this is subject to AbsMaxVoltage which for all curent board revisios is actually capped at 16 volts.
            // The function for converting volts to command data is Value = (desired volts / 0.2)

            // If valid value is sent, update MaxVoltage
            /*val = mtcu_buffer[2];
              if (val >= 30 && val <= 140)        // Allowable range
              {
                if (val <= (AbsMaxVoltage * 5)) // But constrain also to our board-specific AbsMaxVoltage
                {
                    MaxVoltage = ((float)val * 0.2);
                }
              }*/
            break;

        case 4:
            // Motor 2 Forward
            // setSpeed(2, getSpeedCommand_fromSerial(mtcu_buffer[2]));
            break;

        case 5:
            // Motor 2 Reverse
            // setSpeed(2, -getSpeedCommand_fromSerial(mtcu_buffer[2]));
            break;

            // cases  6-13 reserved for future compatibility with the equivalent Sabertooth commands

        case 14:
            // Serial1 Watchdog (disabled by default on each boot)
            // Values greater than 0 will enabled the watchdog. The value specifies what length of time the controller will wait for a new serial command, after which if it does not receive one it will
            // stop the motors as a safety precaution. This can help guard against for example the communication cable becoming disconnected.
            // The the value passed is 0 it will disable the watchdog, however the feature is disabled by default on each restart so you don't need to do anything if you don't want it.
            // Note also the serial watchdog has no effect when the Scout is running in RC mode.

            if (mtcu_buffer[2] == 0)
            {
                // SerialWatchdog = false;
            }
            else
            {
                // The length of time for the watchdog to wait is set in 100mS increments, so for example a value of 10 would equate to a watchdog timeout of 1000mS (1 second).
                // Valid data is a number from 0 to 255 which corresponds to watchdog timeouts of 100ms (1/10 second) to 25500mS (25.5 seconds)
                // The function for converting watchdog time to command data is Value = desired time in mS / 100
                // SerialWatchdogTimeout_mS = mtcu_buffer[2] * 100;
                // SerialWatchdog = true;
            }
            break;

        case 15:
            // Change baud rate. If valid value passed, re-start the hardware serial port at the selected baud rate
            // Serial1.println(6);
            /*Serial1.end();
            delay(1);
            switch (mtcu_buffer[2])
            {
            case BAUD_CODE_2400:
              Serial1.begin(2400);
              break;
            case BAUD_CODE_9600:
              Serial1.begin(9600);
              break;
            case BAUD_CODE_19200:
              Serial1.begin(19200);
              break;
            case BAUD_CODE_38400:
              Serial1.begin(38400);
              break;
            case BAUD_CODE_115200:
              Serial1.begin(115200);
              break;
            case BAUD_CODE_57600:
              Serial1.begin(57600);
              break;
            }*/
            break;

        // cases 16-17 reserved for future compatibility with Sabertooth commands (ramping and deadband)
        // case  19 presently un-assigned
        case 19:
            // Throttle position
            if (mtcu_buffer[2])
            {
                // Throttle = mtcu_buffer[2];
            }
            break;
        case 20:
            // Direct fan control
            // Set fan speed to value 0-255
            /*setFanSpeed(mtcu_buffer[2]);
              // If user sets a fan speed, we switch to ManualFanControl. They can revert to automatic control by issuing command 21
              ManualFanControl = true;*/
            break;

        case 21:
            // Set fan control to "Automatic"
            // Fan control is automatic by default, so this command doesn't need to be issued unless the user initiated manual fan control (command 20) and now wants to revert to automatic.
            // Value is ignored.
            // ManualFanControl = false;
            break;

        case 22:
            // Set maximum current
            // Used to set a maximum current PER MOTOR (not total device current). If current on either motor exceeds the maximum level, BOTH motors will be stopped.
            // Value is in amps and can be any value from 1 to 30. Default is 12 amps, if the user chooses to set a higher level it is highly recommended to use a cooling fan.
            // One may wonder why we don't permit milliamp-level adjustment the way we do with voltage. The reason is that current sensing on this device is crude and
            // setting a precision current limit isn't possible anyway.

            // If valid value sent, update MaxWatt

#if defined REV1_3 || defined REV1_2
#if defined HIGH_CURRENT
            if (mtcu_buffer[2] > 0 && mtcu_buffer[2] <= 60)
            {
                MaxCurrent = mtcu_buffer[2];
            }
            else
            {
                MaxCurrent = 60;
            }
#elif defined LOW_CURRENT
            if (mtcu_buffer[2] > 0 && mtcu_buffer[2] <= 6)
            {
                MaxCurrent = mtcu_buffer[2];
            }
            else
            {
                MaxCurrent = 6;
            }
#else
            if (mtcu_buffer[2] > 0 && mtcu_buffer[2] <= 20)
            {
                MaxCurrent = mtcu_buffer[2];
            }
            else
            {
                MaxCurrent = 20;
            }
#endif
#elif defined REV1_1
            if (mtcu_buffer[2] > 0 && mtcu_buffer[2] <= 30)
            {
                MaxCurrent = mtcu_buffer[2];
            }
            else
            {
                MaxCurrent = 30;
            }
#endif

            break;

        case 23:
            // Brake motors at stop
            // The default method of stopping the motors is simply to quit powering them. However this does nothing to keep them from free-wheeling in response to an external
            // force. If we actually want them to remain stopped, we need to "brake" them. This is done by shorting both motor leads together which creates resistance inside
            // the motor that can help keep it stationary. The motor of course can still be turned as the brake is not very strong. But it can make a difference in some applications.
            // For tracked vehicles we will typically want this enabled since turning at slow speed is accomplished by keeping one track stationary.
            // The BrakeAtStop setting defaults to false at bootup but can be set by the user via this serial command. Pass a value of 1 (true) in the data byte to enable, or 0 (false) to disable.
            /*if (mtcu_buffer[2])
            {
              BrakeAtStop = true;
              // If a motor is already stopped go ahead and apply the brakes now
              if (M1_Speed == 0)
                BrakeMotor1();
              if (M2_Speed == 0)
                BrakeMotor2();
            }
            else
            {
              BrakeAtStop = false;
            }*/
            break;

        case 24:
            // Drag Inner Track
            // This function was created to compensate for certain gearboxes such as the Taigen V2 Steel 3:1 and 4:1 gearboxes that have inadequate
            // internal friction to prevent freewheeling with any sort of external force. When used in a tank or similar vehicle that requires
            // differential motor speed in order to turn, the model may become difficult or impossible to steer.
            //
            // The effect is most pronounced on heavy wide-tracked models with metal upgrades, such as the King Tiger, Jagdtiger, Panther, Jagdpanther, etc....
            // In these cases reducing voltage to the inner track to steer does nothing, the outer (faster) track has enough traction with the ground to
            // keep driving the model straight forward and the inner track just freewheels to keep up, rather than "dragging" the model into a turn as it should.
            //
            // The DragInnerTrack setting defaults to false at bootup but can be set by the user via this serial command. Pass a value of 1 (true) in the data byte
            // to enable, or 0 (false) to disable. When enabled, the Scout will determine which motor is supposed to be turning slower than the other, and attempt
            // to prevent it from freewheeling beyond the desired speed by "dragging" it with brief, pulsed brake commands.
            // See the DragMotor() function on the MOTOR tab of this sketch for further comments.
            /*if (mtcu_buffer[2])
              DragInnerTrack = true;
            else
              DragInnerTrack = false;*/
            break;
        case 25:
            // Max speed forward set by user in op config
            if (mtcu_buffer[2])
            {
                // MaxSpeedForward = mtcu_buffer[2];
            }
            break;
        case 26:
            // Max speed reverse set by user in op config
            if (mtcu_buffer[2])
            {
                // MaxSpeedReverse = mtcu_buffer[2];
            }
            break;
        case 27:
            // Torque modifier for the current selected gear 0-255 = 0 - 100%
            if (mtcu_buffer[2])
            {
                // GearTorque = mtcu_buffer[2];
            }
            break;
        case 46:
            // Throttle position
            if (mtcu_buffer[2])
            {
                // Throttle = mtcu_buffer[2];
#ifdef uart_debug
                Serial.print(" t ");
                Serial.println(Throttle);
#endif
            }
            break;
        case 75:
            // Vehicle speed scaled by max speed values
            if (mtcu_buffer[2])
            {
                // vehicleSpeed = mtcu_buffer[2];
#ifdef uart_debug
                Serial.print(" vs ");
                Serial.println(vehicleSpeed);
#endif
            }
            break;
        default:
            break;
        }
    }
}

/*int16_t getSpeedCommand_fromSerial(uint8_t val)
{
  // Serial speed commands should be 0 to 127.
  //val = constrain(val, 0, 127);

  // Now multiply by 3 to scale our speed value (from 0 to 127) to our PWM duty cycle range (0 to 381)
  return val * 2;
}*/

/*void setChannelsMTCU(){
    uint8_t crc = crsf_crc8(&crsf_buffer[2], crsf_buffer[1] - 1);

    if(crc == crsf_buffer[25]){

    if (crsf_buffer[1] == 24)
    {
        crsf_channels[0] = ((crsf_buffer[3] | crsf_buffer[4] << 8) & 0x07FF);
        crsf_channels[1] = ((crsf_buffer[4] >> 3 | crsf_buffer[5] << 5) & 0x07FF);
        crsf_channels[2] = ((crsf_buffer[5] >> 6 | crsf_buffer[6] << 2 | crsf_buffer[7] << 10) & 0x07FF);
        crsf_channels[3] = ((crsf_buffer[7] >> 1 | crsf_buffer[8] << 7) & 0x07FF);
        crsf_channels[4] = ((crsf_buffer[8] >> 4 | crsf_buffer[9] << 4) & 0x07FF);
        crsf_channels[5] = ((crsf_buffer[9] >> 7 | crsf_buffer[10] << 1 | crsf_buffer[11] << 9) & 0x07FF);
        crsf_channels[6] = ((crsf_buffer[11] >> 2 | crsf_buffer[12] << 6) & 0x07FF);
        crsf_channels[7] = ((crsf_buffer[12] >> 5 | crsf_buffer[13] << 3) & 0x07FF);
        crsf_channels[8] = ((crsf_buffer[14] | crsf_buffer[15] << 8) & 0x07FF);
        crsf_channels[9] = ((crsf_buffer[15] >> 3 | crsf_buffer[16] << 5) & 0x07FF);
        crsf_channels[10] = ((crsf_buffer[16] >> 6 | crsf_buffer[17] << 2 | crsf_buffer[18] << 10) & 0x07FF);
        crsf_channels[11] = ((crsf_buffer[18] >> 1 | crsf_buffer[19] << 7) & 0x07FF);
        crsf_channels[12] = ((crsf_buffer[19] >> 4 | crsf_buffer[20] << 4) & 0x07FF);
        crsf_channels[13] = ((crsf_buffer[20] >> 7 | crsf_buffer[21] << 1 | crsf_buffer[22] << 9) & 0x07FF);
        crsf_channels[14] = ((crsf_buffer[22] >> 2 | crsf_buffer[23] << 6) & 0x07FF);
        crsf_channels[15] = ((crsf_buffer[23] >> 5 | crsf_buffer[24] << 3) & 0x07FF);
    }
//
//    if(crsf_channels[4] >= 1000){
//    	RC_CAR_REVERSE = 1;
//    	comp_pwm = 0;
//    	use_sin_start = 0;
//    	brake_on_stop = 0;
//    }else{
//    	RC_CAR_REVERSE = 0;
//    	comp_pwm = 1;
//    	use_sin_start = 1;
//    	brake_on_stop = 1;
//    }
if(bi_direction){
    if((crsf_channels[crsf_input_channel] > 980) && (crsf_channels[crsf_input_channel] <996)){
    newinput = 1000;
    }else{
    newinput = map(crsf_channels[crsf_input_channel], 174, 1810, 0, 2000);
    }
}else{
    if(crsf_channels[crsf_input_channel]<= 174){
        newinput = 0;
    }else{
    newinput = map(crsf_channels[crsf_input_channel], 174, 1810, 47, 2047);
    }
}
    signaltimeout = 0;
    if((adjusted_input == 0) && !armed){
        zero_input_count++;
    }else{
        zero_input_count = 0;
    }
    }else{
        invalid_crc++;
    }
}*/
