#include "PCI_UG_T1.h"


//----------------------------------------------------------------------
PCI_UG_T1::PCI_UG_T1(uint8_t t0, uint8_t t1)
{
  xtipostep[0]=t0;
  xtipostep[1]=t1;

  if (t0==0){
    ledcAttachPin(14, 0);ledcWrite(0, 0);              //se motor DC em CN1, define channel 0 pino 14 (motor DC 0)
    ledcSetup(0, 1000, 8);                             //PWM sempre a 1KHz
    ledcAttachPin(15, 1);ledcWrite(1, 0);              //define channel 1 pino 15 (motor DC 1)
    ledcSetup(1, 1000, 8);                             //PWM sempre a 1KHz  
  }

  if (t1==0){
    ledcAttachPin(25, 2);ledcWrite(2, 0);              //se motor DC em CN1, define channel 2 pino 25 (motor DC 2)
    ledcSetup(2, 1000, 8);                             //PWM sempre a 1KHz
    ledcAttachPin(26, 3);ledcWrite(3, 0);              //define channel 3 pino 26 (motor DC 3)
    ledcSetup(3, 1000, 8);                             //PWM sempre a 1KHz
  }
}


//----------------------------------------------------------------------
void PCI_UG_T1::begin() {
  pinMode(16, OUTPUT); pinMode(17, OUTPUT); pinMode(18, OUTPUT); pinMode(19, OUTPUT); //motores CN1
  pinMode(13, OUTPUT); pinMode(27, OUTPUT); pinMode(33, OUTPUT); pinMode(23, OUTPUT); //motores CN2
  pinMode(14, OUTPUT); pinMode(15, OUTPUT); //enable/PWM motores CN1
  pinMode(25, OUTPUT); pinMode(26, OUTPUT); //enable/PWM motores CN2
  pinMode(5, OUTPUT); //beep
  pinMode(4, OUTPUT); //led

  digitalWrite(16,0); digitalWrite(17,0); digitalWrite(18,0); digitalWrite(19,0);
  digitalWrite(13,0); digitalWrite(27,0); digitalWrite(33,0); digitalWrite(23,0);
  digitalWrite(14,0); digitalWrite(15,0); digitalWrite(25,0); digitalWrite(26,0);
  digitalWrite(5,0); digitalWrite(4,0);

  ledcAttachPin(5, 5);ledcWrite(5, 0);               //define channel 5 pino 5 (beep) e grava 0 nele (silencia)
  ledcSetup(5, 1000, 8);                             //PWM sempre a 1KHz


  const uint8_t timerNumber = 0;
  hw_timer_t *timer1ms = NULL;
  timer1ms = timerBegin(timerNumber, 80, true);
  isrTable[timerNumber] = this;
  auto isr = getIsr(timerNumber);
  timerAttachInterrupt(timer1ms, isr, false);
  timerAlarmWrite(timer1ms, 1000, true);
  timerAlarmEnable(timer1ms);
}


//----------------------------------------------------------------------
void PCI_UG_T1::runStep(uint8_t n, uint32_t steps, uint8_t velstep, boolean cwstep)
{

  xsteps[n]=steps;
  xvelstep[n]=velstep;
  xvelnow[n]=velstep;
  xcwstep[n]=cwstep;
  if (xcwstep[n]){xfase[n]=-1;}
  if (!xcwstep[n]){xfase[n]=4; if (xtipostep[n]==3){xfase[n]=8;}}
  if (n==0){digitalWrite(14,1);digitalWrite(15,1);}
  if (n==1){digitalWrite(25,1);digitalWrite(26,1);}
}


//----------------------------------------------------------------------
void PCI_UG_T1::runDC(uint8_t n, uint32_t time, uint8_t veldc, boolean cwdc)
{

  xtime[n]=time;
  xveldc[n]=veldc;
  xcwdc[n]=cwdc;
  if ((n==0)||(n==1)){digitalWrite(14,1);digitalWrite(15,1);}
  if ((n==2)||(n==3)){digitalWrite(25,1);digitalWrite(26,1);}
}


//----------------------------------------------------------------------
uint32_t PCI_UG_T1::where(uint8_t n)
{
  return xsteps[n];
}


//----------------------------------------------------------------------
void PCI_UG_T1::beep(int xbnum, int xbdur, int xbfreq, int xbinter)
{
  bnum=xbnum; bdur=xbdur; bfreq=xbfreq; binter=xbinter;
}


//----------------------------------------------------------------------
void PCI_UG_T1::led(int xlnum, int xldur, int xlinter)
{
  lnum=xlnum; ldur=xldur; linter=xlinter;
}


//----------------------------------------------------------------------
void PCI_UG_T1::setms(uint32_t yms)
{
  xms=yms;
}


//----------------------------------------------------------------------
uint32_t PCI_UG_T1::getms()
{
  return xms;
}


//----------------------------------------------------------------------
void PCI_UG_T1::stopStep(uint8_t n)
{
  xsteps[n]=0;
}


//----------------------------------------------------------------------
void PCI_UG_T1::stopDC(uint8_t n)
{
  xtime[n]=0;
}


//----------------------------------------------------------------------
void PCI_UG_T1::stopBeep()
{
  bnum=0;
}


//----------------------------------------------------------------------
void PCI_UG_T1::stopLed()
{
  lnum=0;
}


//----------------------------------------------------------------------
void IRAM_ATTR PCI_UG_T1::onTimer1ms()
{
  if (xms>0){xms--;}

  //processa os steps---------------------------------------------------------------------------------
  if ((xsteps[0]>0)||(xsteps[1]>0)){
    for (k=0; k<2; k++){
      if (xsteps[k]>0){
        xvelnow[k]--;
        if (xvelnow[k]==0){
          xvelnow[k]=xvelstep[k];
          int nf=3;if (xtipostep[k]==3){nf=7;}
          if (xcwstep[k]){xfase[k]++;if (xfase[k]>nf){xfase[k]=0;}}else{xfase[k]--;if (xfase[k]<0){xfase[k]=nf;}}
          PCI_UG_T1::go();
          xsteps[k]--;
          if (xsteps[k]==0){
            if (k==0){digitalWrite(16, 0);digitalWrite(17, 0);digitalWrite(18, 0);digitalWrite(19, 0); digitalWrite(14,0);digitalWrite(15,0);}
            if (k==1){digitalWrite(13, 0);digitalWrite(27, 0);digitalWrite(33, 0);digitalWrite(23, 0); digitalWrite(25,0);digitalWrite(26,0);}
          }
        }
      }
    }
  }


  //processa os DCs------------------------------------------------------------------------------------
  for (k=0; k<4; k++){
    if (xtime[k]>0){
      if ( xcwdc[k]){digitalWrite(pinosdc[k][0], 0);digitalWrite(pinosdc[k][1], 1);}
      if (!xcwdc[k]){digitalWrite(pinosdc[k][1], 0);digitalWrite(pinosdc[k][0], 1);}
      ledcWrite(k, int(float(xveldc[k])/100.0*255.0));
      xtime[k]--;
      if (xtime[k]==0){digitalWrite(pinosdc[k][0], 0);digitalWrite(pinosdc[k][1], 0);digitalWrite(pinosdc[k][2], 0);}
    }
  }


  if (bnum>0){
    if (bxpri){                           //if is the beginning of cycle to beep,
      bxinter=binter+1; bxdur=bdur;       //init the time variables
      bxpausa=false; bxpri=false;         //with default values or user values
      ledcSetup(5, bfreq, 8);             //
    }                                     // 
    if (!bxpausa && (bxdur>0)) {          //if it is beeping 
      ledcWrite(5, 127);bxdur--;          //keep the beep beeping for bxdur ms
      if(bxdur==0){                       //at end,
        ledcWrite(5, 0);                  //stop the beep and advise 
        bxpausa=true;                     //that pause fase is to be initiated
      }
    }
    if (bxpausa && (bxinter>0)){          //if it is in pause
      ledcWrite(5, 0);bxinter--;          //keep the beep stoped for bxinter ms
      if(bxinter==0){                     //at end, exit from pause, subtract 1 from quantity of desired 
        bxpausa=false;bnum--;bxpri=true;  //beeps and advise to reload the variables for a new cycle
      }
    }
  }


  if (lnum>0){
    if (lxpri){                           //if is the beginning of cycle to blink led,
      lxinter=linter+1; lxdur=ldur;       //init the time variables
      lxpausa=false; lxpri=false;         //with default values or user valuess
    }                                     // 
    if (!lxpausa && (lxdur>0)) {          //if the led is on (out of pause fase)
      digitalWrite(4, 1);lxdur--;        //keep the led on for lxdur ms
      if(lxdur==0){                       //at end,
        digitalWrite(4, 0);              //turn off the led and advise
        lxpausa=true;                     //that pause fase is to be initiated
      }
    }
    if (lxpausa && (lxinter>0)){          //if the led is off (pause fase)
      digitalWrite(4, 0);lxinter--;      //keep the led off for lxinter ms
      if(lxinter==0){                     //at end, exit from pause, subtract 1 from quantity of desired
        lxpausa=false;lnum--;lxpri=true;  //blinks and advise to reload the variables for a new cycle
      }
    }
  }

}

PCI_UG_T1 *PCI_UG_T1::isrTable[SOC_TIMER_GROUP_TOTAL_TIMERS];


//----------------------------------------------------------------------
void PCI_UG_T1::go()
{
  switch (xtipostep[k]) {
    case 1: PCI_UG_T1::move1(); break;   //28byj-48, 2048 steps, full step, low torque, low consumption
    case 2: PCI_UG_T1::move2(); break;   //28byj-48, 2048 steps, full step, high torque, high consumption
    case 3: PCI_UG_T1::move3(); break;   //28byj-48, 4096 steps, half step, high torque, high consumption
  }
}


//----------------------------------------------------------------------
void PCI_UG_T1::move1(){   //28byj-48, 2048 steps, full step, low torque, low consumption
  switch (xfase[k]) {
    case 0: PCI_UG_T1::writ(0,0,0,1); break;   //0x01
    case 1: PCI_UG_T1::writ(0,0,1,0); break;   //0x02
    case 2: PCI_UG_T1::writ(0,1,0,0); break;   //0x04
    case 3: PCI_UG_T1::writ(1,0,0,0); break;   //0x08
  }
}

void PCI_UG_T1::move2(){   //28byj-48, 2048 steps, full step, high torque, high consumption
  switch (xfase[k]) {
    case 0: PCI_UG_T1::writ(1,0,0,1); break;   //0x09
    case 1: PCI_UG_T1::writ(0,0,1,1); break;   //0x03
    case 2: PCI_UG_T1::writ(0,1,1,0); break;   //0x06
    case 3: PCI_UG_T1::writ(1,1,0,0); break;   //0x0C    
  }
}

void PCI_UG_T1::move3(){   //28byj-48, 4096 steps, half step, high torque, high consumption
  switch (xfase[k]) {
    case 0: PCI_UG_T1::writ(1,0,0,1); break;   //0x09
    case 1: PCI_UG_T1::writ(0,0,0,1); break;   //0x01
    case 2: PCI_UG_T1::writ(0,0,1,1); break;   //0x03
    case 3: PCI_UG_T1::writ(0,0,1,0); break;   //0x02
    case 4: PCI_UG_T1::writ(0,1,1,0); break;   //0x06
    case 5: PCI_UG_T1::writ(0,1,0,0); break;   //0x04
    case 6: PCI_UG_T1::writ(1,1,0,0); break;   //0x0C
    case 7: PCI_UG_T1::writ(1,0,0,0); break;   //0x08
  } 
}


//----------------------------------------------------------------------
void PCI_UG_T1::writ(uint8_t px1, uint8_t px2, uint8_t px3, uint8_t px4)
{
  if (k==0){digitalWrite(16, px1);digitalWrite(17, px2);digitalWrite(18, px3);digitalWrite(19, px4);}
  if (k==1){digitalWrite(13, px1);digitalWrite(27, px2);digitalWrite(33, px3);digitalWrite(23, px4);}
}
//----------------------------------------------------------------------


