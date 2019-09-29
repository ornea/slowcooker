/*
   PID.ino

*/

// resistance at 25 degrees C
#define THERMISTORNOMINAL 5000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 4216 //3950
// the value of the 'other' resistor
#define SERIESRESISTOR 5000

// What pin to connect the sensor to
#define THERMISTORPIN A0


#ifdef USE_DHT
#define DHTPIN D5
#include "DHT.h"
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
#endif

int totalfails;

#ifdef USE_ADC
int sensorPin = A0;
#endif

#ifdef USE_MAX6675
#define ktcSO  D6  //12// 2
#define ktcCS  D7  //13// 12
#define ktcCLK D5  //14//  13 WAS D5
#include <max6675.h>
MAX6675 ktc(ktcCLK, ktcCS, ktcSO);
#endif

//This function computes the new value for the ON time of the system
//This is the return value from this function
int computePID(float spPID)
{
  static float lastError = 0;
  //float lastReading = 0;
  float output = 0;

  int NaNCount = 0;
  currentHeat = 0.0 / 0.0; //make it NaN
  while (isnan(currentHeat) && (NaNCount++ < 4))
  {
    doservers();
#ifdef USE_DHT
    currentHeat = dht.readTemperature() + AppConfig.PID[cal];
    //Serial.print("DHT Temp:");
    //Serial.println(dht.readTemperature());
    currentHumidity = dht.readHumidity();
#endif

#ifdef USE_ADC
    sensorValue = analogRead(sensorPin);
    //currentHeat = (sensorValue * 0.0148) //volts conversion

    float reading;// = analogRead(THERMISTORPIN);

    //Serial.print("Analog reading ");
    //Serial.println(sensorValue);

    //reading = ((5076 * 1023) / reading) - 4923;
    reading = ((5153 * 1000) / sensorValue) - 4951;

    //Serial.print("Thermistor resistance ");
    //Serial.println(reading);

    float steinhart;
    steinhart = reading / THERMISTORNOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;                         // convert to C

    currentHeat = steinhart;
#endif

#ifdef USE_MAX6675
    currentHeat = ktc.readCelsius() + AppConfig.PID[cal]; //readIronTemp(0, 1,spPID); //get the current temp of the iron
#endif
    if (isnan(currentHeat))
    {
      totalfails++;
      previousyieldMillis = millis();
      Serial.println(NaNCount);
      Serial.print(F("Read Attempt No:"));

      while ((millis() - previousyieldMillis) < 1000)
      {
        doservers();
      }
    }
  }

  if (isnan(currentHeat))// || isnan(currentHumidity) )
  {
    Serial.println(F("Suspect prope is disconnected - all set to zero"));
    iTerm = 0;
    //lastReading = 0;
    lastError = 0;
    return 0;
  }
  else
  {
    //add cal offset
    currentHeat += AppConfig.PID[cal];

    //Calc the error
    error =  spPID - currentHeat; //calculate the error term
    pTerm = AppConfig.PID[kp] * error;

    //Calc Intergration
    iTerm += (error * AppConfig.PID[ki]);

    //Windup limiting
    if (iTerm > AppConfig.PID[mvmax])
      iTerm = AppConfig.PID[mvmax];
    else if (iTerm < 0)
      iTerm = 0; //cap at 0 since we cant force the iron to cool itself :)

    //Calc Differential
    //float dTerm = (currentHeat - lastReading); //compute the input to the D term
    dTerm = (error - lastError) * AppConfig.PID[kd];

    //Summation
    output = pTerm  + iTerm + dTerm;

    if (output > AppConfig.PID[mvmax])
    {
      output = AppConfig.PID[mvmax];
      //I do the following other wise dTerm has no effect when iTerm has woundup or pTerm is maxed out and I think it should.
      //it causes the CV to back off a little as PV approaches the SP partically after a system upset i.e startup
      if (dTerm < 0)
        output += dTerm;
    }
    else if (output < AppConfig.PID[mvmin])
      output = AppConfig.PID[mvmin];
    /*if(currentHeat > spPID)
      {
      output = 0;
      }
    */

    //lastReading = currentHeat; //storing values for next iteration of the loop
    lastError = error;
    if (error < 0)
    {
      // output = 0;
    }
    /*
      For Debug below
      Serial.println();
      Serial.print(F("Heat °C: "));
      Serial.print(currentHeat);
      Serial.print(F(" \t"));

      Serial.print(F("Humidity: "));
      Serial.print(currentHumidity);
      Serial.print(F(" %\t"));

      Serial.print(F("ERROR: "));
      Serial.print(error);
      Serial.print(F(" \t"));

      Serial.print(F("pTerm: "));
      Serial.print(pTerm);
      Serial.print(F(" \t"));

      Serial.print(F("ITerm: "));
      Serial.print(iTerm);
      Serial.print(F(" \t"));

      Serial.print(F("dTerm: "));
      Serial.print(dTerm);
      Serial.print(F(" \t"));

      Serial.print(F("PID: "));
      Serial.print(output);
      Serial.print(F(" \t"));
      Serial.print(F("Millis: "));
      Serial.print(millis());
    */
    return output;
  }
}
