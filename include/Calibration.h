//
// Created by yauta on 26.03.2020.
//

#ifndef SIM_PEDALS_CALIBRATION_H
#define SIM_PEDALS_CALIBRATION_H

void readEEPROM(bool debug,int16_t leftBrakeMax,int16_t rightBrakeMax,int16_t yawMin,int16_t yawMax,int16_t leftBrakeMin,int16_t rightBrakeMin);
void calibration(bool debug,Joystick_ joystick,int16_t leftBrakeMax,int16_t rightBrakeMax,int16_t yawMin,int16_t yawMax,int16_t leftBrakeMin,int16_t rightBrakeMin);

#endif //SIM_PEDALS_CALIBRATION_H
