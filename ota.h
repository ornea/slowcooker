//**************
//************** Init Over The Air Updates
//**************
ArduinoOTA.setHostname(AppConfig.EpHostname);
ArduinoOTA.onStart([]() {
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH)
    type = "sketch";
  else // U_SPIFFS
    type = "filesystem";

  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  HEATER_LOW; //lets turn off the HEATER while we update
  Serial.print(F("Start updating "));
  Serial.println(type);
});
ArduinoOTA.onEnd([]() {
  Serial.println(F("\nEnd"));
});
ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
});
ArduinoOTA.onError([](ota_error_t error) {
  Serial.printf("Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR) Serial.println(F("Auth Failed"));
  else if (error == OTA_BEGIN_ERROR) Serial.println(F("Begin Failed"));
  else if (error == OTA_CONNECT_ERROR) Serial.println(F("Connect Failed"));
  else if (error == OTA_RECEIVE_ERROR) Serial.println(F("Receive Failed"));
  else if (error == OTA_END_ERROR) Serial.println(F("End Failed"));
});
ArduinoOTA.begin();
Serial.println(F("Ready"));
Serial.print(F("IP address: "));
Serial.println(WiFi.localIP());
//**************
