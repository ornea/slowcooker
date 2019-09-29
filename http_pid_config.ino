void handle_pid_config()
{
  Serial.println(F("/pid_config Accessed"));
  START_HTML;
  START_PAGE(AppConfig.EpHostname, "PID Configuration Page");
  START_FORM("pid_config_update");

  START_TABLE("AP: PID Configuration.");

  ADD_ROW_STRING ("kpPID: ");
  ADD_INPUT_CELL (AppConfig.PID[kp], kpPID);
  END_ROW;

  ADD_ROW_STRING ("kiPID: ");
  ADD_INPUT_CELL (AppConfig.PID[ki], kiPID);
  END_ROW;

  ADD_ROW_STRING ("kdPID: ");
  ADD_INPUT_CELL (AppConfig.PID[kd], kdPID);
  END_ROW;

  ADD_ROW_STRING ("maxPID: ");
  ADD_INPUT_CELL (AppConfig.PID[mvmax], maxPID);
  END_ROW;

  ADD_ROW_STRING ("minPID: ");
  ADD_INPUT_CELL (AppConfig.PID[mvmin], minPID);
  END_ROW;

  ADD_ROW_STRING ("SP1: ");
  ADD_INPUT_CELL (AppConfig.PID[sp1], sp1PID);
  END_ROW;

  ADD_ROW_STRING ("SP2: ");
  ADD_INPUT_CELL (AppConfig.PID[sp2], sp2PID);
  END_ROW;

  ADD_ROW_STRING ("SP1TD: ");
  ADD_INPUT_CELL (AppConfig.PID[sp1td], sp1tdPID);
  END_ROW;

  ADD_ROW_STRING ("SP2TD: ");
  ADD_INPUT_CELL (AppConfig.PID[sp2td], sp2tdPID);
  END_ROW;

  ADD_ROW_STRING ("CAL: ");
  ADD_INPUT_CELL (AppConfig.PID[cal], CAL);
  END_ROW;

  END_TABLE;

  END_FORM;
  END_PAGE_CUSTOM;

  END_PAGE;
  //server.sendHeader("Access-Control-Allow-Origin", "*");
}


void handle_pid_config_update()
{
  Serial.println(F("/pid_config_update Accessed"));

  Serial.println(F("Writing to EEPROM with new kpPID: "));
  Serial.println(server.arg ("kpPID" ));
  AppConfig.PID[kp] = server.arg ("kpPID" ).toFloat();

  Serial.println(F("Writing to EEPROM with new kiPID: "));
  Serial.println(server.arg ("kiPID" ));
  AppConfig.PID[ki] = server.arg ("kiPID" ).toFloat();

  Serial.println(F("Writing to EEPROM with new kdPID: "));
  Serial.println(server.arg ("kdPID" ));
  AppConfig.PID[kd] = server.arg ("kdPID" ).toFloat();

  Serial.println(F("Writing to EEPROM with new maxPID: "));
  Serial.println(server.arg ("maxPID" ));
  AppConfig.PID[mvmax] = server.arg ("maxPID" ).toInt();

  Serial.println(F("Writing to EEPROM with new minPID: "));
  Serial.println(server.arg ("minPID" ));
  AppConfig.PID[mvmin] = server.arg ("minPID" ).toInt();

  Serial.println(F("Writing to EEPROM with new sp1PID: "));
  Serial.println(server.arg ("sp1PID" ));
  AppConfig.PID[sp1] = server.arg ("sp1PID" ).toInt();

  Serial.println(F("Writing to EEPROM with new sp2PID: "));
  Serial.println(server.arg ("sp2PID" ));
  AppConfig.PID[sp2] = server.arg ("sp2PID" ).toInt();

  Serial.println(F("Writing to EEPROM with new sp1tdPID: "));
  Serial.println(server.arg ("sp1tdPID" ));
  AppConfig.PID[sp1td] = server.arg ("sp1tdPID" ).toInt();

  Serial.println(F("Writing to EEPROM with new sp2tdPID: "));
  Serial.println(server.arg ("sp2tdPID" ));
  AppConfig.PID[sp2td] = server.arg ("sp2tdPID" ).toInt();

  Serial.println(F("Writing to EEPROM with new CAL: "));
  Serial.println(server.arg ("CAL" ));
  AppConfig.PID[cal] = server.arg ("CAL" ).toInt();

  START_HTML;
  START_PAGE(AppConfig.EpHostname, "Updating PID Configuration");
  server.sendContent(F("PID saved to eeprom..."));
  END_PAGE_CUSTOM;
  END_PAGE;
  Serial.println(F("PID saved to eeprom... "));
  SaveAppConfig();
}
