/*
 * A library for the 4 digit display
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Frankie.Chu
 * Create Time: 9 April,2012
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <TimerOne.h>
#include "TM1637.h"
#define ON 1
#define OFF 0
const int buzzerPin = A0;
int lastState = 0;
int switchPin = 12;
int switchState = LOW;
int lastState2 = 0;
int switchPin2 = 11;
int switchState2 = LOW;
int lastState3 = 0;
int switchPin3 = 10;
int switchState3 = LOW;
int lastState4 = 0;
int switchPin4 = 9;
int switchState4 = LOW;
int switchStatedisplay = 0;
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char halfsecond = 0;
unsigned char second;
unsigned char minute = 0;
unsigned char hour = 12;
int clockswitch = 0;
int8_t TimeDispAlarm[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPointAlarm = 1;
unsigned char UpdateAlarm;
unsigned char halfsecondAlarm = 0;
unsigned char secondAlarm;
unsigned char minuteAlarm = 0;
unsigned char hourAlarm = 0;
int alarmOn = 0;

#define CLK 2//pins definitions for TM1637 and can be changed to other ports
#define DIO 3
TM1637 tm1637(CLK,DIO);

void setup()
{
  pinMode(buzzerPin, OUTPUT); 
  Serial.begin(9600);
  pinMode(switchPin, INPUT);
  pinMode(switchPin2, INPUT);


  tm1637.set();
  tm1637.init();
  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR
}
void loop()
{
  switchState = digitalRead(switchPin);
  switchState2 = digitalRead(switchPin2);
  Serial.print(switchState);
  Serial.print(" - ");
  Serial.println(switchState2);

  switchState3 = digitalRead(switchPin3);
  switchState4 = digitalRead(switchPin4);

  Serial.print(switchState3);
  Serial.print(" - ");
  Serial.println(switchState4);



//this is the code for the buttons to switch the clockswitch so that the display switch functions properly

if (lastState3 != switchState3)
{
  if (switchState3 == HIGH){
      clockswitch = 1;
  }
}
if (lastState4 != switchState4)
{
  if (switchState4 == HIGH){
      clockswitch = 0;
  }
}
//This is the display switcher between the alarm display and clock display

if(clockswitch == 1){
  tm1637.display(TimeDispAlarm);
  }else{
  tm1637.display(TimeDisp);
}


//this is the logic that increases minutes and hours. these minutes and hours are added to the right clock by checking the clockswitch condition

if(alarmOn == 0){
  if(clockswitch == 1){
  if (lastState != switchState){
    if (switchState == HIGH) {
      lastState = switchState;
      minuteAlarm+=1;
    }
  }
  lastState = switchState;

  if (lastState2 != switchState2){
    if (switchState2 == HIGH) {
      lastState2 = switchState2;
        hourAlarm+=1;
    }
  }
  lastState2 = switchState2;


}
if(clockswitch == 0){
  if (lastState2 != switchState2){
    if (switchState2 == HIGH) {
      lastState2 = switchState2;
        hour+=1;
    }
  }
  lastState2 = switchState2;

  if (lastState != switchState){
    if (switchState == HIGH) {
      lastState = switchState;
      minute+=1;
      if (minute == 60){
        minute = 0;
      }
    }
  }
lastState = switchState;
}
}else{
    if (lastState != switchState){
      if (switchState == HIGH) {
        lastState = switchState;
        alarmOn = 0;
    }
  }
lastState = switchState;
}



//if(TimeDisp[0] == TimeDispAlarm[0] && TimeDisp[1] == TimeDispAlarm[1] && TimeDisp[2] == TimeDispAlarm[2] && TimeDisp[3] == TimeDispAlarm[3])
//{
  //alarmOn = 1;
//}

//if(alarmOn == 1){
 // tone(buzzerPin, 1000); // Send 1KHz sound signal...
  //delay(1000);        // ...for 1 sec
  //noTone(buzzerPin);     // Stop sound...
  //delay(1000);        // ...for 1sec
//}
TimeUpdate();
TimeUpdateAlarm();
UpdateAlarm = ON;
}
void TimingISR()
{
  halfsecond ++;
  Update = ON;
  if(halfsecond == 2){
    second ++;
    if(second == 60)
    {
      minute ++;
      if(minute == 60)
      {
        hour ++;
        if(hour == 24)hour = 0;
        minute = 0;
      }
      second = 0;
    }
    halfsecond = 0;
  }
 // Serial.println(second);
  ClockPoint = (~ClockPoint) & 0x01;
  ClockPointAlarm = (~ClockPointAlarm) & 0x01;
}
void TimeUpdate(void)
{
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF);
  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;
  Update = OFF;
}
void TimeUpdateAlarm(void)
{
  TimeDispAlarm[0] = hourAlarm / 10;
  TimeDispAlarm[1] = hourAlarm % 10;
  TimeDispAlarm[2] = minuteAlarm / 10;
  TimeDispAlarm[3] = minuteAlarm % 10;
  UpdateAlarm = OFF;
}
