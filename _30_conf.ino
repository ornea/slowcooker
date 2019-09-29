void InitAppConfig()
{
  char s[16] = "";
  Serial.println(F("Reading EEPROM signature"));
  EEPROM.begin(512); delay(10);
  for (int i = 0; i < sizeof(AppConfig.E_sign); ++i)
  {
    s[i] += char(EEPROM.read(i));
  }
  EEPROM.end();
  Serial.print(F("Signature read: "));
  Serial.println(s);

  if (strcmp(AppConfig.E_sign, s)  == 0)  //if(AppConfig.E_sign == s)
  {
    EEPROM.begin(512); delay(10);
    Serial.println(F("Signature verified. Initialising Config From EEPROM"));
    EEPROM.get(0, AppConfig);
    EEPROM.end();
  }
  else
  {
    Serial.println(F("Signature not verified. Initialising EEPROM "));
    SaveAppConfig();
  }
}

void SaveAppConfig()
{
  EEPROM.begin(512);
  delay(10);

  EEPROM.put(0, AppConfig);
  EEPROM.end();
  Serial.println("Saving EEPROM");
}

void OutAppConfig()
{
  Serial.println(AppConfig.E_sign);

  Serial.print(F("End Point Hostname:"));
  Serial.println(AppConfig.EpHostname);

  Serial.print(F("End Point Control Hostname:"));
  Serial.println(AppConfig.EpControlHostname);

  Serial.print(F("Ep SSID0: "));
  Serial.println(AppConfig.access_point[0][indx_SSID]);

  Serial.print(F("Ep Key0: "));
  Serial.println(AppConfig.access_point[0][indx_KEY]);


  Serial.print(F("Ep SSID1: "));
  Serial.println(AppConfig.access_point[1][indx_SSID]);

  Serial.print(F("Ep Key1: "));
  Serial.println(AppConfig.access_point[1][indx_KEY]);


  Serial.print(F("Ep SSID2: "));
  Serial.println(AppConfig.access_point[2][indx_SSID]);

  Serial.print(F("Ep Key2: "));
  Serial.println(AppConfig.access_point[2][indx_KEY]);


  Serial.print(F("Ep SSID3: "));
  Serial.println(AppConfig.access_point[3][indx_SSID]);

  Serial.print(F("Ep Key3: "));
  Serial.println(AppConfig.access_point[3][indx_KEY]);


  Serial.print(F("Ep SSID4: "));
  Serial.println(AppConfig.access_point[4][indx_SSID]);

  Serial.print(F("Ep Key4: "));
  Serial.println(AppConfig.access_point[4][indx_KEY]);



  Serial.print(F("End Point IP:"));
  Serial.println(IPAddress(AppConfig.EpIP));
  Serial.print(F("End Point GW:"));
  Serial.println(IPAddress(AppConfig.EpGW));
  Serial.print(F("End Point Mask:"));
  Serial.println(IPAddress(AppConfig.EpMASK));

  Serial.print(F("Access Point SSID:"));
  Serial.println(AppConfig.ApSSID);
  Serial.print(F("Access Point Key:"));
  Serial.println(AppConfig.ApKey);
  Serial.print(F("Access Point IP:"));
  Serial.println(IPAddress(AppConfig.ApIP));
  Serial.print(F("Access Point GW:"));
  Serial.println(IPAddress(AppConfig.ApGW));
  Serial.print(F("Access Point Mask:"));
  Serial.println(IPAddress(AppConfig.ApMASK));

  Serial.print(F("DHCP Enabled:(not Used)"));
  Serial.println(AppConfig.Flags.bIsDHCPEnabled);

}

void OutChAppConfig()
{
  unsigned char *ptr = (unsigned char *)&AppConfig;
  unsigned char k;
  Serial.println(F("About to do lots of bytes out"));
  for (int i = 0; i < sizeof(AppConfig); ++i)
  {
    k = *(ptr + i);
    Serial.print(F("Index: ")); Serial.print(i); Serial.print(F("Char: ")); Serial.print(k); Serial.println(char(k));
  }
}
