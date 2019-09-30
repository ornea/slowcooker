#include <WebSocketsServer.h>

WebSocketsServer webSocket(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  Serial.println();
  Serial.print(F("clientid="));
  Serial.println(num);
  Serial.print (F("Type="));
  Serial.println (type);
  Serial.print(F("length="));
  Serial.print(length);

  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        Serial.println();

        // send message to client
        webSocket.sendTXT(num, "Connected");
        sendPIDData();
        sendStatus();
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      Serial.println();
      String spayload = (char*)payload;
      Serial.println(spayload);
      if (payload[0] == BUTTON_PREAMPLE[0]) //we have a button
      {
        String id = spayload.substring(1, spayload.indexOf(',')) ;
        Serial.print("id :");
        Serial.print(id);
        if (id == "idMode")
        {
          AppConfig.Mode++;
          if (AppConfig.Mode == invalid) AppConfig.Mode = free_run;
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
              webSocket.broadcastTXT("heater_on");
              HEATER_HIGH;
            }
            break;
          case forced_low: //Normal State
            {
              webSocket.broadcastTXT(FORCED_LOW);
              webSocket.broadcastTXT("heater_off");
              HEATER_LOW;
            }
            break;
          case invalid: //Normal State
            {
              //Should never get here
              AppConfig.Mode = free_run;
            }
            break;
        }

        if (id == "idActive")
        {
          AppConfig.Flags.bIsOutputInverted = !AppConfig.Flags.bIsOutputInverted; //toggle inversion
          if (AppConfig.Flags.bIsOutputInverted)
          {
            webSocket.broadcastTXT(INVERTED);
          }
          else
          {
            webSocket.broadcastTXT(NONINVERTED);
          }
        }

        if (id == "idReset")
        {
          sm = standby;
          //sendPIDData();  //i want to do this but the loop still needs to update the variable
          //heating_stage = stage1;
        }

        if (id == "idUnits")
        {
          AppConfig.Flags.bIsCelsius = !AppConfig.Flags.bIsCelsius;
        }
        if (id == "idSave")
        {
          SaveAppConfig();
        }

      }
      if (payload[0] == STRING_PREAMBLE[0])//STRING_PREAMBLE_SQ )//'@')//STRING_PREAMBLE') //we have a string
      {
        float f = (spayload.substring(spayload.indexOf(',') + 1, spayload.length())).toFloat();
        String arrfieldsinput[] = {"sp1", "sp2", "sp1td", "sp2td", "kp", "ki", "kd", "mvmax", "mvmin", "cal"};
        //const byte io_pins [9] = {D0, D1, D2, D3, D4, D5, D6, D7, D8};//
        String id = spayload.substring(1, spayload.indexOf(',')) ;

        if (id == "sp1")
        {
          AppConfig.PID[sp1] =  f;
        }
        if (id == "sp2")
        {
          AppConfig.PID[sp2] =  f;
        }
        if (id == "sp1td")
        {
          AppConfig.PID[sp1td] =  f;
        }
        if (id == "sp2td")
        {
          AppConfig.PID[sp2td] =  f;
        }

        if (id == "kp")
        {
          AppConfig.PID[kp] =  f;
        }
        if (id == "ki")
        {
          AppConfig.PID[ki] =  f;
        }
        if (id == "kd")
        {
          AppConfig.PID[kd] =  f;
        }
        if (id == "mvmax")
        {
          AppConfig.PID[mvmax] =  f;
        }
        if (id == "mvmin")
        {
          AppConfig.PID[mvmin] =  f;
        }
        if (id == "cal")
        {
          AppConfig.PID[cal] =  f;
        }
        if (id == "tc")
        {
          AppConfig.PID[tc] =  f * 1000; //in ms
        }

        if (id == "es")
        {
          //call all stop
        }
      }
      break;
  }
}
