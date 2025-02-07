/* ASK Shield - Sound Level ELECTREC CAPSULE */

//SMOTHING FILTER CONSTANT
#define alphaSound 0.98

//LIMITS FOR CALIBRATION SENSOR
#define minSound  32
#define maxSound  300
int soundSilence = 512;

//SERIAL DEBUGING
#define debugSound 0

int dbValue;
double smoothValue;
double lastValue;
double newValue;


void calSound()
{
  soundSilence = getAverage(micpin, 1000, 2);
  Serial.print("sound calibration: ");
  Serial.println(soundSilence); 
}


int getSound(byte micPin)
{

  for(int i = 0; i<500 ; i++) // to ensure the filter is stable
  {   
    newValue = analogRead(micpin); // take measurement here  
    //newValue = getAverage(micpin, 100, 25); // take measurement here every 25us (40khz)

    if (newValue > soundSilence) newValue = (newValue-soundSilence)*2;
    else newValue = (soundSilence-newValue)*2;

    smoothValue = alphaSound * lastValue + (1 - alphaSound) * newValue;
    if(smoothValue < 0)  smoothValue = 0;
    lastValue = smoothValue;

    dbValue = computeFscale( minSound, maxSound, 40, 95, smoothValue, 6.5); // logarithm response mapping - 5.5!!!

    if(debugSound)
    {
      Serial.print("sensor RAW: ");
      Serial.print(newValue);
      Serial.print("  sensor smooth: ");
      Serial.print((int)smoothValue);
      Serial.print("  DB ");
      Serial.println(dbValue);
    }
  }

  return dbValue;

}



