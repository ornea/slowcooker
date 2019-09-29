/* Notes
  server.send(204); //No Content - stops the source page from reloading
  server.sendHeader("Access-Control-Allow-Origin", "*"); //allows a page to auto load content
  server.send(200, "text/html", message);

*/
void handle_root()
{
  // Serial.println(client.connected());
  Serial.print(F("URI: "));
  Serial.println(server.uri());
  Serial.print(F("Method: "));
  Serial.println((server.method() == HTTP_GET) ? "GET" : "POST");
  Serial.print(F("Arguments: "));
  Serial.println(server.args());
  Serial.println(F("\n"));
  for (uint8_t i = 0; i < server.args(); i++)
  {
    Serial.println(server.argName(i));
    Serial.println(server.arg(i));
  }
  if (server.arg(0) == "open")
  {
    HEATER_HIGH;
    Serial.println(F("Opening ..."));
  }
  if (server.arg(0) == "close")
  {
    HEATER_LOW;
    Serial.println(F("Closing ..."));
  }
  if (server.argName(0) == "toggle") //?toggle=5
  {
    digitalWrite(server.arg(0).toInt(), !digitalRead(server.arg(0).toInt()));
  }
  START_HTML;
  START_PAGE(AppConfig.EpHostname, " Home Page");
  if (digitalRead(HEATER))
    server.sendContent(F("<b>Heater High</b>"));
  else
    server.sendContent(F("<b>Heater Low</b>"));
  END_PAGE_CUSTOM;
  END_PAGE;

  //server.send(204); //No Content - stops the source page from reloading
  //hmmmserver.sendHeader("Access-Control-Allow-Origin", "*");
  // server.send(200, "text/html", message);
}

void favicon_ico(void)
{
  server.sendContent (F("data:image/x-icon;base64,AAABAAEAEBAQAAEABAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAA/4QAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAERABAAEAEBAQABAQEBABAAEAEQARABAQABAQABAAAQARAAEQARAAAAAAAAAAAAAAAAAAAAAAEREAEQAQAAAQAAEAEBAAABAAAAAQEAAAERAAEQAREAAQAAEAABABABAAAQAQEAEAEREAEQAREAAAAAAAAAAAD//wAA2N0AAKuqAADdmQAArrsAANnMAAD//wAA//8AAIZvAAC9rwAAv68AAI5jAAC97QAAva0AAIZjAAD//wAA'"));
  //hmmm server.send(200, "image/x-icon", message );
}

//**************************** /build **********************************
void handle_build(void)
{
  Serial.println(F("accessing Build Info"));
  //hmmm server.sendHeader("Access-Control-Allow-Origin", "*");
  String fullpathname = __SKETCH__;
  fullpathname.replace("\\", "\\<br>");

  START_HTML;
  START_PAGE(AppConfig.EpHostname, " Build Info");

  server.sendContent(String(millis()));
  server.sendContent(F( "<br>Hostname: "));
  server.sendContent( AppConfig.EpHostname);
  server.sendContent(F("<br>Build Date: "));
  server.sendContent( __DATE__ );
  server.sendContent(F("<br>   Time: "));
  server.sendContent( __TIME__ );
  server.sendContent(F("<br>   Board: "));
  server.sendContent(ARDUINO_BOARD);
  server.sendContent(F("<br>   File: "));
  server.sendContent(fullpathname);//withbreaks);
  server.sendContent(strSummary) ;
  END_PAGE_CUSTOM;
  END_PAGE;
  //hmmm  server.send(200, "text/plain", message );
}

//**************************** /IO_read **********************************
void handle_IO_read()
{
  Serial.println(F("accessing I/O Status"));
  //hmmm server.sendHeader("Access-Control-Allow-Origin", "*");
  START_HTML;
  START_PAGE(AppConfig.EpHostname, " IO Read");
  server.sendContent("ms: " + String(millis()));
  server.sendContent(F("<br>D0 GPIO16: "));
  server.sendContent(String( digitalRead(16)));
  server.sendContent(F("<br>D1 GPIO05: "));
  server.sendContent(String(digitalRead(5)));
  server.sendContent(F("<br>D2 GPIO04: "));
  server.sendContent(String(digitalRead(4)));
  server.sendContent(F("<br>D3 GPIO00: "));
  server.sendContent(String(digitalRead(0)));
  server.sendContent(F("<br>D4 GPIO02: "));
  server.sendContent(String(digitalRead(2)));
  server.sendContent(F("<br>D5 GPIO14: "));
  server.sendContent(String(digitalRead(14)));
  server.sendContent(F("<br>D6 GPIO12: "));
  server.sendContent(String(digitalRead(12)));
  server.sendContent(F("<br>D7 GPIO13: "));
  server.sendContent(String(digitalRead(13)));
  server.sendContent(F("<br>D8 GPIO15: "));
  server.sendContent(String(digitalRead(15)));
  //hmmm server.send(200, "text/plain", message );
  END_PAGE_CUSTOM;
  END_PAGE;
}

//**************************** /pid_status **********************************
void handle_pid_status()
{
  Serial.println(F("accessing PID Status"));
  START_HTML;
  START_PAGE(AppConfig.EpHostname, " IO Read");
  //hmmm server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendContent(F("ms: "));
  server.sendContent(String(millis()));
  server.sendContent(F("<br>"));
  server.sendContent(F("Heat °C: "));
  server.sendContent(String(currentHeat));
  server.sendContent(F("<br>"));
  server.sendContent(F("Humidity: "));
  server.sendContent(String(currentHumidity));
  server.sendContent(F("<br>"));
  server.sendContent(F("error: "));
  server.sendContent(String(error));
  server.sendContent(F("<br>"));
  server.sendContent(F("pTerm: "));
  server.sendContent(String(pTerm));
  server.sendContent(F("<br>"));
  server.sendContent(F("iTerm: "));
  server.sendContent(String(iTerm));
  server.sendContent(F("<br>"));
  server.sendContent(F("dTerm: "));
  server.sendContent(String(dTerm));
  server.sendContent(F("<br>"));
  server.sendContent(F("PID: "));
  server.sendContent(String(CV));
  server.sendContent(F("<br>"));
  END_PAGE_CUSTOM;
  END_PAGE;
};

//**************************** /IO_write **********************************
void handle_IO_write()
{

  uint8_t bit; // = digitalPinToBitMask(D4);
  uint8_t port; // = digitalPinToPort(D4);
  volatile uint32_t *reg ;// = portModeRegister(port);
  int i;
  Serial.println(F("accessing I/O Write "));
  //server.sendHeader("Access-Control-Allow-Origin", "*");
  Serial.print(F("URI: "));
  Serial.println(server.uri());
  Serial.print(F("Method: "));
  Serial.println((server.method() == HTTP_GET) ? "GET" : "POST");
  Serial.print(F("Arguments: "));
  Serial.println(server.args());
  Serial.println(F("\n"));
  for (uint8_t i = 0; i < server.args(); i++)
  {
    Serial.println(server.argName(i));
    Serial.println(server.arg(i));
  }

  if (server.argName(0) == "toggle") //?toggle=5
  {
    digitalWrite(io_pins[server.arg(0).toInt()], !digitalRead(io_pins[server.arg(0).toInt()]));
  }
  if (server.argName(0) == "toggle_mode") //?toggle=5
  {
    pinMode(io_pins[server.arg(0).toInt()], !getPinMode(io_pins[server.arg(0).toInt()]));
  }

  if (server.argName(0) == "high") //?high=5
  {
    digitalWrite(io_pins[server.arg(0).toInt()], HIGH);
  }

  if (server.argName(0) == "low") //?low=5
  {
    digitalWrite(io_pins[server.arg(0).toInt()], LOW);
  }

  START_HTML;
  START_PAGE(AppConfig.EpHostname, " I/O Write Page");

  for (i = 0; i <= 8; i++)
  {
    server.sendContent(F( "<br><button onclick=\"location.href = '?high="));
    server.sendContent(String(i));
    server.sendContent(F( " ';\" id='D"));
    server.sendContent(String(i));
    server.sendContent(F("' class='float-left submit-button' >HIGH D"));
    server.sendContent(String(i));
    server.sendContent(F("</button>"));

    server.sendContent(F("<button onclick=\"location.href = '?low="));
    server.sendContent(String(i));
    server.sendContent(F(" ';\" id='D"));
    server.sendContent(String(i));
    server.sendContent(F("' class='float-left submit-button' >LOW D"));
    server.sendContent(String(i));
    server.sendContent(F("</button>"));

    server.sendContent(F("<button onclick=\"location.href = '?toggle="));
    server.sendContent(String(i));
    server.sendContent(F(" ';\" id='D"));
    server.sendContent(String(i));
    server.sendContent(F("' class='float-left submit-button' >TOGGLE D"));
    server.sendContent(String(i));
    server.sendContent(F("</button>  "));

    server.sendContent(F("<button onclick=\"location.href = '?toggle_mode="));
    server.sendContent(String(i));
    server.sendContent(F(" ';\" id='D"));
    server.sendContent(String(i));
    server.sendContent(F("' class='float-left submit-button' >TOGGLE MODE D"));
    server.sendContent(String(i));
    server.sendContent(F("</button>  "));

    server.sendContent(F("  State:"));
    server.sendContent(String(digitalRead(io_pins[i])));
    server.sendContent(F("  "));
    //    bit = digitalPinToBitMask(io_pins[i]);
    //    uint8_t port = digitalPinToPort(io_pins[i]);
    //    volatile uint32_t *reg = portModeRegister(port);
    if (getPinMode(io_pins[i]))
    {
      // It's an output
      server.sendContent(F("  PinMode:OUTPUT"));
    }
    else
    {
      // It's an input
      server.sendContent(F("  PinMode:INPUT"));
    }
  }
  END_PAGE_CUSTOM;
  END_PAGE;
  //server.sendHeader("Access-Control-Allow-Origin", "*");
  //server.send(200, "text/html", message);
}

//**************************** /config **********************************
void handle_config() //sends form to /submit_to_page
{
  Serial.println(F("/config Accessed"));
  START_HTML;
  START_PAGE(AppConfig.EpHostname, " Configuration Page");

  START_FORM("config_update");

  START_TABLE("EP: End Point Configuration.");

  ADD_ROW_STRING ("EP HOSTNAME: ");
  ADD_INPUT_CELL (AppConfig.EpHostname, EpHostname);
  END_ROW;

  ADD_ROW_STRING ("EP CONTROL HOSTNAME: ");
  ADD_INPUT_CELL (AppConfig.EpControlHostname, EpControlHostname);
  END_ROW;

  ADD_ROW_STRING ("EP SSID0: ");
  ADD_INPUT_CELL (AppConfig.access_point[0][indx_SSID], EpSSID0);
  ADD_INPUT_CELL (AppConfig.access_point[0][indx_KEY], EpKey0);
  END_ROW;

  ADD_ROW_STRING ("EP SSID1: ");
  ADD_INPUT_CELL (AppConfig.access_point[1][indx_SSID], EpSSID1);
  ADD_INPUT_CELL (AppConfig.access_point[1][indx_KEY], EpKey1);
  END_ROW;

  ADD_ROW_STRING ("EP SSID2: ");
  ADD_INPUT_CELL (AppConfig.access_point[2][indx_SSID], EpSSID2);
  ADD_INPUT_CELL (AppConfig.access_point[2][indx_KEY], EpKey2);
  END_ROW;

  ADD_ROW_STRING ("EP SSID3: ");
  ADD_INPUT_CELL (AppConfig.access_point[3][indx_SSID], EpSSID3);
  ADD_INPUT_CELL (AppConfig.access_point[3][indx_KEY], EpKey3);
  END_ROW;

  ADD_ROW_STRING ("EP SSID4: ");
  ADD_INPUT_CELL (AppConfig.access_point[4][indx_SSID], EpSSID4);
  ADD_INPUT_CELL (AppConfig.access_point[4][indx_KEY], EpKey4);
  END_ROW;

  ADD_ROW_STRING ("EP IP: ");
  ADD_INPUT_CELL (AppConfig.EpIP[0], EpIP_0);
  ADD_INPUT_CELL (AppConfig.EpIP[1], EpIP_1);
  ADD_INPUT_CELL (AppConfig.EpIP[2], EpIP_2);
  ADD_INPUT_CELL (AppConfig.EpIP[3], EpIP_3);
  END_ROW;

  ADD_ROW_STRING ("EP GW: ");
  ADD_INPUT_CELL (AppConfig.EpGW[0], EpGW_0);
  ADD_INPUT_CELL (AppConfig.EpGW[1], EpGW_1);
  ADD_INPUT_CELL (AppConfig.EpGW[2], EpGW_2);
  ADD_INPUT_CELL (AppConfig.EpGW[3], EpGW_3);
  END_ROW;

  ADD_ROW_STRING ("EP MASK: ");
  ADD_INPUT_CELL (AppConfig.EpMASK[0], EpMASK_0);
  ADD_INPUT_CELL (AppConfig.EpMASK[1], EpMASK_1);
  ADD_INPUT_CELL (AppConfig.EpMASK[2], EpMASK_2);
  ADD_INPUT_CELL (AppConfig.EpMASK[3], EpMASK_3);
  END_ROW;

  END_TABLE;

  START_TABLE("AP: Access Point Configuration.");

  ADD_ROW_STRING ("AP SSID: ");
  ADD_INPUT_CELL (AppConfig.ApSSID, ApSSID);
  END_ROW;

  ADD_ROW_STRING ("AP Key: ");
  ADD_INPUT_CELL (AppConfig.ApKey, ApKey);
  END_ROW;

  ADD_ROW_STRING ("AP IP: ");
  ADD_INPUT_CELL (AppConfig.ApIP[0], ApIP_0);
  ADD_INPUT_CELL (AppConfig.ApIP[1], ApIP_1);
  ADD_INPUT_CELL (AppConfig.ApIP[2], ApIP_2);
  ADD_INPUT_CELL (AppConfig.ApIP[3], ApIP_3);
  END_ROW;

  ADD_ROW_STRING ("AP GW: ");
  ADD_INPUT_CELL (AppConfig.ApGW[0], ApGW_0);
  ADD_INPUT_CELL (AppConfig.ApGW[1], ApGW_1);
  ADD_INPUT_CELL (AppConfig.ApGW[2], ApGW_2);
  ADD_INPUT_CELL (AppConfig.ApGW[3], ApGW_3);
  END_ROW;

  ADD_ROW_STRING ("AP MASK: ");
  ADD_INPUT_CELL (AppConfig.ApMASK[0], ApMASK_0);
  ADD_INPUT_CELL (AppConfig.ApMASK[1], ApMASK_1);
  ADD_INPUT_CELL (AppConfig.ApMASK[2], ApMASK_2);
  ADD_INPUT_CELL (AppConfig.ApMASK[3], ApMASK_3);
  END_ROW;

  END_TABLE;
  END_FORM;
  END_PAGE_CUSTOM;
  END_PAGE;
  //  server.sendHeader("Access-Control-Allow-Origin", "*");
}

//**************************** /update_config **********************************
void handle_config_update()  //gets form from /config
{
  Serial.println(F("/config_update Accessed"));
  // String message;
  //  String c;

  if (1) //host.length()>1)
  {
    //EEPROM.begin(512);

    Serial.println(F("Writing to EEPROM with new Ep HOSTNAME: "));
    Serial.println(server.arg ("EpHostname" ));
    strcpy(AppConfig.EpHostname, server.arg ("EpHostname" ).c_str());

    Serial.println(F("Writing to EEPROM with new Ep CONTROL HOSTNAME: "));
    Serial.println(server.arg ("EpControlHostname" ));
    strcpy(AppConfig.EpControlHostname, server.arg ("EpControlHostname" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep SSID0: "));
    Serial.println(server.arg ("EpSSID0" ));
    strcpy(AppConfig.access_point[0][indx_SSID], server.arg ("EpSSID0" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key0: "));
    Serial.println(server.arg ("EpKey0" ));
    strcpy(AppConfig.access_point[0][indx_KEY], server.arg ("EpKey0" ).c_str());


    Serial.print(F("Writing to EEPROM with new Ep SSID1: "));
    Serial.println(server.arg ("EpSSID1" ));
    strcpy(AppConfig.access_point[1][indx_SSID], server.arg ("EpSSID1" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key1: "));
    Serial.println(server.arg ("EpKey1" ));
    strcpy(AppConfig.access_point[1][indx_KEY], server.arg ("EpKey1" ).c_str());


    Serial.print(F("Writing to EEPROM with new Ep SSID2: "));
    Serial.println(server.arg ("EpSSID2" ));
    strcpy(AppConfig.access_point[2][indx_SSID], server.arg ("EpSSID2" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key2: "));
    Serial.println(server.arg ("EpKey2" ));
    strcpy(AppConfig.access_point[2][indx_KEY], server.arg ("EpKey2" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep SSID3: "));
    Serial.println(server.arg ("EpSSID3" ));
    strcpy(AppConfig.access_point[3][indx_SSID], server.arg ("EpSSID3" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key3: "));
    Serial.println(server.arg ("EpKey3" ));
    strcpy(AppConfig.access_point[3][indx_KEY], server.arg ("EpKey3" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep SSID4: "));
    Serial.println(server.arg ("EpSSID4" ));
    strcpy(AppConfig.access_point[4][indx_SSID], server.arg ("EpSSID4" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key4: "));
    Serial.println(server.arg ("EpKey4" ));
    strcpy(AppConfig.access_point[4][indx_KEY], server.arg ("EpKey4" ).c_str());


    Serial.println(F("Writing to EEPROM with new Ep IP: "));
    Serial.print(server.arg ("EpIP_0" )); Serial.print(F(".")); Serial.print(server.arg ("EpIP_1" )); Serial.print(F(".")); Serial.print(server.arg ("EpIP_2" )); Serial.print(F(".")); Serial.println(server.arg ("EpIP_3" ));
    AppConfig.EpIP[0] = server.arg ("EpIP_0" ).toInt(); AppConfig.EpIP[1] = server.arg ("EpIP_1" ).toInt(); AppConfig.EpIP[2] = server.arg ("EpIP_2" ).toInt(); AppConfig.EpIP[3] = server.arg ("EpIP_3" ).toInt();

    Serial.println(F("Writing to EEPROM with new Ep GW: "));
    Serial.print(server.arg ("EpGW_0" )); Serial.print(F(".")); Serial.print(server.arg ("EpGW_1" )); Serial.print(F(".")); Serial.print(server.arg ("EpGW_2" )); Serial.print(F(".")); Serial.println(server.arg ("EpGW_3" ));
    AppConfig.EpGW[0] = server.arg ("EpGW_0" ).toInt(); AppConfig.EpGW[1] = server.arg ("EpGW_1" ).toInt(); AppConfig.EpGW[2] = server.arg ("EpGW_2" ).toInt(); AppConfig.EpGW[3] = server.arg ("EpGW_3" ).toInt();

    Serial.println(F("Writing to EEPROM with new Ep Mask: "));
    Serial.print(server.arg ("EpMASK_0" )); Serial.print(F(".")); Serial.print(server.arg ("EpMASK_1" )); Serial.print(F(".")); Serial.print(server.arg ("EpMASK_2" )); Serial.print(F(".")); Serial.println(server.arg ("EpMASK_3" ));
    AppConfig.EpMASK[0] = server.arg ("EpMASK_0" ).toInt(); AppConfig.EpMASK[1] = server.arg ("EpMASK_1" ).toInt(); AppConfig.EpMASK[2] = server.arg ("EpMASK_2" ).toInt(); AppConfig.EpMASK[3] = server.arg ("EpMASK_3" ).toInt();

    Serial.print(F("Writing to EEPROM with new Ep SSID0: "));
    Serial.println(server.arg ("EpSSID0" ));
    strcpy(AppConfig.access_point[0][indx_SSID], server.arg ("EpSSID0" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key0: "));
    Serial.println(server.arg ("EpKey0" ));
    strcpy(AppConfig.access_point[0][indx_KEY], server.arg ("EpKey0" ).c_str());


    Serial.print(F("Writing to EEPROM with new Ep SSID1: "));
    Serial.println(server.arg ("EpSSID1" ));
    strcpy(AppConfig.access_point[1][indx_SSID], server.arg ("EpSSID1" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key1: "));
    Serial.println(server.arg ("EpKey1" ));
    strcpy(AppConfig.access_point[1][indx_KEY], server.arg ("EpKey1" ).c_str());


    Serial.print(F("Writing to EEPROM with new Ep SSID2: "));
    Serial.println(server.arg ("EpSSID2" ));
    strcpy(AppConfig.access_point[2][indx_SSID], server.arg ("EpSSID2" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key2: "));
    Serial.println(server.arg ("EpKey2" ));
    strcpy(AppConfig.access_point[2][indx_KEY], server.arg ("EpKey2" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep SSID3: "));
    Serial.println(server.arg ("EpSSID3" ));
    strcpy(AppConfig.access_point[3][indx_SSID], server.arg ("EpSSID3" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key3: "));
    Serial.println(server.arg ("EpKey3" ));
    strcpy(AppConfig.access_point[3][indx_KEY], server.arg ("EpKey3" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep SSID4: "));
    Serial.println(server.arg ("EpSSID4" ));
    strcpy(AppConfig.access_point[4][indx_SSID], server.arg ("EpSSID4" ).c_str());

    Serial.print(F("Writing to EEPROM with new Ep Key4: "));
    Serial.println(server.arg ("EpKey4" ));
    strcpy(AppConfig.access_point[4][indx_KEY], server.arg ("EpKey4" ).c_str());


    Serial.print(F("Writing to EEPROM with new AP SSID: "));
    Serial.println(server.arg ("ApSSID" ));
    strcpy(AppConfig.ApSSID, server.arg ("ApSSID" ).c_str());

    Serial.print(F("Writing to EEPROM with new AP Key: "));
    Serial.println(server.arg ("ApKey" ));
    strcpy(AppConfig.ApKey, server.arg ("ApKey" ).c_str());


    Serial.println(F("Writing to EEPROM with new Ap IP: "));
    Serial.print(server.arg ("ApIP_0" )); Serial.print(F(".")); Serial.print(server.arg ("ApIP_1" )); Serial.print(F(".")); Serial.print(server.arg ("ApIP_2" )); Serial.print(F(".")); Serial.println(server.arg ("ApIP_3" ));
    AppConfig.ApIP[0] = server.arg ("ApIP_0" ).toInt(); AppConfig.ApIP[1] = server.arg ("ApIP_1" ).toInt(); AppConfig.ApIP[2] = server.arg ("ApIP_2" ).toInt(); AppConfig.ApIP[3] = server.arg ("ApIP_3" ).toInt();

    Serial.println(F("Writing to EEPROM with new Ap GW: "));
    Serial.print(server.arg ("ApGW_0" )); Serial.print(F(".")); Serial.print(server.arg ("ApGW_1" )); Serial.print(F(".")); Serial.print(server.arg ("ApGW_2" )); Serial.print(F(".")); Serial.println(server.arg ("ApGW_3" ));
    AppConfig.ApGW[0] = server.arg ("ApGW_0" ).toInt(); AppConfig.ApGW[1] = server.arg ("ApGW_1" ).toInt(); AppConfig.ApGW[2] = server.arg ("ApGW_2" ).toInt(); AppConfig.ApGW[3] = server.arg ("ApGW_3" ).toInt();

    Serial.println(F("Writing to EEPROM with new Ap Mask: "));
    Serial.print(server.arg ("ApMASK_0" )); Serial.print(F(".")); Serial.print(server.arg ("ApMASK_1" )); Serial.print(F(".")); Serial.print(server.arg ("ApMASK_2" )); Serial.print(F(".")); Serial.println(server.arg ("ApMASK_3" ));
    AppConfig.ApMASK[0] = server.arg ("ApMASK_0" ).toInt(); AppConfig.ApMASK[1] = server.arg ("ApMASK_1" ).toInt(); AppConfig.ApMASK[2] = server.arg ("ApMASK_2" ).toInt(); AppConfig.ApMASK[3] = server.arg ("ApMASK_3" ).toInt();

    START_HTML;

    START_PAGE(AppConfig.EpHostname, "Updating Configuration Page");
    server.sendContent(F("saved to eeprom... reset to boot into new wifi"));
    END_PAGE_CUSTOM;
    END_PAGE;
    //server.sendHeader("Access-Control-Allow-Origin", "*");
    //server.send(200, "text/html", message);
    Serial.println(F("saved to eeprom... reset to boot into new wifi"));
    SaveAppConfig();
  }
  else
  {
    Serial.println(F("Hostname appears invalid. Not saving Hostname: "));
    Serial.println(server.arg ("ApSSID" ));
    START_PAGE(AppConfig.EpHostname, "NOT Updating Configuration Page");
    server.sendContent(F("<p> Hostname invalid. No changes saved</html>\r\n\r\n"));
    END_PAGE_CUSTOM;
    END_PAGE;
    //server.sendHeader("Access-Control-Allow-Origin", "*");
  }
  OutAppConfig();
}


//**************************** /help **********************************
void handle_pid_help()
{
  Serial.println(F("/pid_help accessed"));
  START_HTML;
  START_PAGE(AppConfig.EpHostname, " PID Help Page");
  server.sendContent(F("<STYLE TYPE='text/css';>  <!--    @page { margin: 2cm }   P { margin-bottom: 0.21cm}    A:link { so-language: zxx } --> </STYLE><div align='left';><BODY align='left' LANG='en-AU' DIR='LTR'><P><B>Overview.</B></P><P>The easycook PID controller was originally designed to accurately control a Halogen oven (aka AU-Easycook or US-Flavorwave Turbo Oven) to make beef jerky accurately and quickly.</P><P>The USDA Meat and Poultry Hotline's states '... After heating to 160 &deg;F or 165 &deg;F, maintaining a constant dehydrator temperature of 130 to 140 &deg;F during the drying process is important ...' and as such wanted to design a controller that would meet this requirement.</P><P>The controller can be configured with two Setpoints (sp1 &amp; sp2) with corresponding durations (sp1td &amp; sp2td).</P><P>Once running the controller will initially drive (vectoring) the output to obtain a temperature as set by sp1, once achieved will maintain (sustain) this temperature for the duration set by sp1td (in mins), then drive the output to obtain a temperature as set by sp2 and maintain for a duration set by dp1td.</P><P>A Time Delay setting of '0' will configure the controller to maintain the related Setpoint indefinately. (Not sure what happens after 49 days)</P><P><BR></P><P><B>1. The Buttons :-</B></P><P>'Jerky Dryer', 'Yogurt Maker', 'EggIncubator' , 'Egg SousVide', 'Black Garlic' and 'Solder Reflow' preset buttons will configured the controller with the relevant PID settings to perform these tasks.</P><P>'Toggle Units' will toggle the displayed values between &#176;C and &#176;F. This only affects the displayed values.  The PID calculations are always done in Celsius. As such values like 'err' may appear misleading </P><P>'Output High' and 'Output Low' allows the user to over ride the PID controller and force the output either high or low.</P><P>'Auto Run' returns control to the PID controller in the event that it has been over ridden with the 'Output High' and 'Output Low' buttons.</P><P>'Active High' and 'Active Low' determines how the output will be driven when the PID is controller demands power. Is usefull if the output logic needs to be reveresed. For example a Zero Crossing Optocoupler (MOC3041) driving a TRIAC (BTA16) (As the author uses in the Easycooker) will require a Active High setting where a An NPN (BC548) driving a N-MOSFET (WFP848) will require Active Low. Note: The changes are applied next time the loop is executed.</P><P>'Reset Restart' resets all the PID calculated values to zero and resets the Statemachine back to the start where it will begin transitioning thru the various states as determined by PID settings.</P><P>'Save State' saves the currently configured PID settings that will be used next time the controller is power cycled.</P><P><BR></P><P><B>2. Output Table.</B></P><P>PID calculated or derived values. Cannot be directly modified.</P><P>'ticks' - the number of times the PID control loop has been executed. The rate of execution is set by the 'tc' slider.</P><P>'PV' Process Variable is the value measured. In this case it will be the temperature as measured by the temperature sensor.</P><P>'err' The current error. (PV - SPx)(The currently active setpoint either SP1, SP2 or 0)</P><P>'pterm' Proportional Term (err * kp)</P><P>'iterm' Intergral Term += (err * ki)</P><P>'dterm' Differential Term = (err -lasterr) * kd (I have seen some implementation that calculate this as (currentReading - previousReading) * kd which is not the same.</P><P>'CV' Control Variable also know as MV or Manipulated Variable. How much the PID controller is demanding from the HEATER. In this controller it can range from 0 (no demand) to 255(full demand) and implements 'On-Off A/C time control' as apposed to 'Phase Angle Control'. Time is chopped into 5 second chunks. The PID controller drive the Output On for a proportion of this 5 secs. For example if CV = 20 then the on duration will be (20/255 * 5sec)or 392ms.</P><P>'millis' Milliseconds since the controller was power cycled or rebooted.</P><P>'hum' Humidity of your sensor supportshumidity.</P><P>'State' can be:-</P><P> 'Starting' -The system is starting,</P><P> ' SP1&raquo; &cong;' The PID controller is vectoring towards SP1, the first setpoint,</P><P> ' SP1&raquo; &equiv;' SP1 has beenreached and the controller is now attempting to maintain that setpoint,</P><P> ' SP2&raquo; &cong;' The duration set by sp1td has expired and the controller is now vectoring towards SP2,</P><P> ' SP2&raquo; &equiv;' SP2 has been reached and is now maintaining SP2 and</P><P> ' ~Comp~ ' sp2td has timed out and the PID is no longer driving the output. The process can be restarted by pressing 'Reset Restart'</P><P>'Timer' The remaining time for the current Setpoint in seconds<P><br></P><P><B>3. Input Table.</B> </P><P>These values can be set by the user via the slider controls below or selecting a preset button.</P><P> 'SP1' The first Setpoint the controller will try to drive the output to.</P><P> 'SP2' The second Setpoint.</P><P> 'sp1td' The duration that thecontroller will attempt to maintain temperature set by SP1. A setting of '0' is infinite.</P><P> 'sp2td' The duration that the controller will attempt to maintain temperature set by SP2. A setting of '0' is infinite.</P><P> 'kp' gain Factor K for the proportional error. See PID Controller Wiki article for effects of these parameters</P><P> 'ki' gain factor for intergral error. If set to zero output may reach the setpoint but will not stabilise with a residual error</P><P> 'kd' gain factor for derivative error. Helps react to sudden system upsets but can cause instability.</P><P> 'mvmax' limits the maximum duration the controller can be active. ie It limits CV, the control variable aka Demand.</P><P> 'mvmin' limits the minimum active duration.</P><P> 'cal' provides a calibration offset for temperature sensor offset errors</P><P> 'tc' Time Constant. The delay between susequent PID control loop executions. For systems that are slow to react set high</P><P> Chart pv &amp; hum. Charts the measured values from the sensor. It also displays a green outlinedwindow that represents the relative values set bu SP1 &amp; SP2</P><P> Chart CV pterm iterm &amp; dterm plot these values as they are calculated.</P><P><br></P><P><B>4. pid tuning levers</B></P><P>The sliders used to vary the various user configurable inputs</P><P><BR></P><P></P><P><B>5. Issues</B> Driving a TRIAC with less than 20V A/C. During testing, it was discovered that there is a minimum VAC as per the Datasheet: Zero Crossing Inhibit Voltage, Max: 20V</P><P>Library for DHT - Needed to change DHT timmings in the library when using ESP8266</P><P><BR></P>"));
  END_PAGE_CUSTOM;
  //hmmm not sure why this is here server.sendContent ("<a href='/pid_config_update'>PID Config Update</a>&nbsp&nbsp");
  END_PAGE;
}

//**************************** /restart **********************************
void handle_restart()
{

  delay(100);
  ESP.restart();
}

//**************************** /notfound **********************************
void handle_not_found()
{
  Serial.println(F("handlenotfound"));
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  Serial.println(message);
  server.send(404, "text/plain", message);
}

//**************************** /draw **********************************
void handle_draw()
{
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send ( 200, "image/svg+xml", out);
}
