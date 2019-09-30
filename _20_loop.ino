//ESP8266 crashes unless these are called regularly
void doservers(void)
{
  server.handleClient();
  ArduinoOTA.handle();
  webSocket.loop();
  yield();
}
char buffers [200];
String strstate = "Starting"; //String that tracks that state. Used in Web page
static unsigned long countdowntimer = 0; //countdown timer for current heating profile

void loop()
{


  unsigned long currentMillis = millis();

  static int PWM_state = 0; //tracks software pwm output state
  static int on_time = 0;   //Software PWM variable
  static int off_time = 0;  //Softare PWM variable
  float deadzone = 0.3;     //change profile when PV is +/- deadzone within current SP.

  char t[] = "sss""sss";

  doservers(); //do housekeeping

  if ((currentMillis - previous_PID_Millis) > AppConfig.PID[tc])
  {
    previous_PID_Millis = currentMillis;
    ticks += 1; //increment PID loop execution counter

    switch (sm) //Switch on statemachine variable
    {
      case standby: //Initial state, zero all PID variables
        {
          Serial.print(F("Standby"));
          strstate = "Starting";
          CV = 0;
          pTerm = 0;
          iTerm = 0;
          dTerm = 0;
          sm++;
        }
        break;
      case stage1vectoring:   //Target SP1 then transition state when PV within +/- deadzone of SP1
        {
          Serial.print(F("Stage1Vectoring"));
          strstate = "SP1» ≅";
          CV = computePID(AppConfig.PID[sp1]);
          if ( (AppConfig.PID[sp1] < (currentHeat + deadzone)) && (AppConfig.PID[sp1] > (currentHeat - deadzone)))
          {
            smMillis = currentMillis;
            sm++;
          }
        }
        break;
      case stage1sustain: //Maintain SP1 for duration set by sp1td
        { countdowntimer = AppConfig.PID[sp1td] * INTERVAL_1_MIN;

          Serial.print(F("Stage1Sustain"));
          strstate = "SP1» ≡";
          CV = computePID(AppConfig.PID[sp1]);
          if (AppConfig.PID[sp1td] != 0) //if 0 stay here for ever
          {
            countdowntimer = (AppConfig.PID[sp1td] * INTERVAL_1_MIN) - (currentMillis - smMillis);
            if ((currentMillis - smMillis) > AppConfig.PID[sp1td]*INTERVAL_1_MIN)
            {
              sm++;
            }
          }
        }
        break;
      case stage2vectoring: //Target SP2 then transition state when PV within +/- deadzone of SP2
        {
          Serial.print(F("Stage2Vectoring"));
          strstate = "SP2» ≅";
          CV = computePID(AppConfig.PID[sp2]);
          countdowntimer = AppConfig.PID[sp2td] * INTERVAL_1_MIN;
          if ( (AppConfig.PID[sp2] < (currentHeat + deadzone)) && (AppConfig.PID[sp2] > (currentHeat - deadzone)))
          {
            smMillis = currentMillis;
            sm++;
          }
        }
        break;
      case stage2sustain: //Maintain SP1 for duration set by sp2td
        {
          Serial.print(F("Stage2Sustain"));
          strstate = "SP2» ≡";
          CV = computePID(AppConfig.PID[sp2]);
          if (AppConfig.PID[sp2td] != 0) //if 0 stay here for ever
          {
            countdowntimer = (AppConfig.PID[sp2td] * INTERVAL_1_MIN) - (currentMillis - smMillis);
            if ((currentMillis - smMillis) > AppConfig.PID[sp2td]*INTERVAL_1_MIN)
            {
              sm++;
            }
          }
        }
        break;
      case cyclecomplete: //turn off output and wait until someone presses Reset/Restart
        {
          strstate = "~Comp~";
          Serial.print(F("Cycle Completed"));
          CV = computePID(AppConfig.PID[0]); //will still want to do readings
          pTerm = 0;
          iTerm = 0;
          dTerm = 0;
          CV = 0;
        }
        break;
      default: //should never be here
        {
          strstate = "Invalid";
          Serial.print(F("Invalid StateMachine State"));
        }
        break;
    }

    if (isnan(currentHeat))
    {
      Serial.println("Setting CV = 0 as reading is NaN");
      CV = 0;
    }


    if (AppConfig.Flags.bIsOutputInverted) //invert the on/off times if the user has configured it so
    {
      off_time = CV;
      on_time = 255 - off_time;
    }
    else
    {
      on_time = CV;
      off_time = 255 - on_time;
    }

    sendPIDData();
    if (AppConfig.Flags.bIsCelsius == TRUE)
    {
      sprintf(buffers, " ticks:%09d Heat:%06.2f Error:%06.2f pTerm:%06.2f iTerm:%06.2f dTerm:%06.2f CV:%03d millis:%09d Humidty:%06.2f State:%s Timer:%9d", ticks, currentHeat, error, pTerm, iTerm, dTerm, CV, millis(), currentHumidity, strstate.c_str(), countdowntimer / INTERVAL_1_SEC );
      Serial.println(buffers);
    }
    else
    {
      sprintf(buffers, " ticks:%09d Heat:%06.2f Error:%06.2f pTerm:%06.2f iTerm:%06.2f dTerm:%06.2f CV:%03d millis:%09d Humidty:%06.2f State:%s Timer:%09d", ticks, degf(currentHeat), degf(error), degf(pTerm), degf(iTerm), degf(dTerm), degf(CV), millis(), currentHumidity, strstate.c_str(), countdowntimer / INTERVAL_1_SEC );
      Serial.println(buffers);
    }

  }
  if (AppConfig.Mode == free_run) //AppConfig.Flags.bIsModeAuto) //if we are in run mode then do the software PWM
  {
    // multiples of 20mS which is 1 period of 50Hz.
    if (((currentMillis - previous_ON_Millis) > (on_time * 20)) && (PWM_state == 1) && off_time)
    {
      HEATER_LOW;
      previous_OFF_Millis = currentMillis;
      PWM_state = 0;
      webSocket.broadcastTXT(HEATER_OFF);
      Serial.println(HEATER_OFF);

    }
    if (((currentMillis - previous_OFF_Millis) > (off_time * 20)) && (PWM_state == 0) && on_time)
    {
      HEATER_HIGH;
      previous_ON_Millis = currentMillis;
      PWM_state = 1;
      webSocket.broadcastTXT(HEATER_ON);
      Serial.println(HEATER_ON);
    }
  }
  else
  {
    if (AppConfig.Mode == forced_high)//AppConfig.Flags.bIsOutForcedHigh)
    {
      HEATER_HIGH;
      //webSocket.broadcastTXT("heater_on");
    }
    else
    {
      HEATER_LOW;
      //webSocket.broadcastTXT("heater_off");
    }
  }
  if (currentMillis > next_heartbeat_Millis) //INTERVAL_1_HOUR)
  {
    next_heartbeat_Millis = currentMillis + 25000;
    sendStatus();
    /*
        if (AppConfig.Flags.bIsCelsius)
        {
          webSocket.broadcastTXT(CELSIUS);
        }
        else
        {
          webSocket.broadcastTXT(FARENHEIT);
        }
    */

  }
}
void sendPIDData(void)
{
  char buffers [200];
  if (AppConfig.Flags.bIsCelsius == TRUE)
  {
    sprintf(buffers, PID_DATA_PREAMBLE" % 09d , % 06.2f , % 06.2f , % 06.2f  , % 06.2f , % 06.2f , % 03d , % 09d , % 06.2f, % s,% 9d", ticks, currentHeat, error, pTerm, iTerm, dTerm, CV, millis(), currentHumidity, strstate.c_str(), countdowntimer / INTERVAL_1_SEC );
  }
  else
  {
    sprintf(buffers, PID_DATA_PREAMBLE" % 09d , % 06.2f , % 06.2f , % 06.2f  , % 06.2f , % 06.2f , % 03d , % 09d , % 06.2f, % s,% 9d", ticks, degf(currentHeat), degf(error), degf(pTerm), degf(iTerm), degf(dTerm), degf(CV), millis(), currentHumidity, strstate.c_str(), countdowntimer / INTERVAL_1_SEC );
  }
  webSocket.broadcastTXT(buffers);
}

void sendStatus(void)
{
  if (ISHEATERHIGH)
  {
    webSocket.broadcastTXT(HEATER_ON);
  }
  else
  {
    webSocket.broadcastTXT(HEATER_OFF);
  }


  if (AppConfig.Flags.bIsOutputInverted)
  {
    webSocket.broadcastTXT(INVERTED);
  }
  else
  {
    webSocket.broadcastTXT(NONINVERTED);
  }
  switch (AppConfig.Mode) //Switch on statemachine variable
  {
    case free_run: //Normal State
      {
        webSocket.broadcastTXT(MODE_AUTO);
      }
      break;
    case forced_high: //Normal State
      {
        webSocket.broadcastTXT(FORCED_HIGH);
        webSocket.broadcastTXT(HEATER_ON);
      }
      break;
    case forced_low: //Normal State
      {
        webSocket.broadcastTXT(FORCED_LOW);
        webSocket.broadcastTXT(HEATER_OFF);
      }
      break;
    case invalid: //Normal State
      {
        //Should never get here
        AppConfig.Mode = free_run;
      }
      break;
  }
}
