//
// Created by yauta on 26.03.2020.
//

#ifndef SIM_PEDALS_CALIBRATION_H
#define SIM_PEDALS_CALIBRATION_H

void readEEPROM(unsigned int leftBrakeMax,unsigned int rightBrakeMax,long yawMin,long yawMax,long temp,unsigned int leftBrakeMin,unsigned int rightBrakeMin);
void calibration(Joystick_ joystick,unsigned int leftBrakeMax,unsigned int rightBrakeMax,long yawMin,long yawMax,long temp,unsigned int leftBrakeMin,unsigned int rightBrakeMin);
long filter(int channel);

#endif //SIM_PEDALS_CALIBRATION_H
