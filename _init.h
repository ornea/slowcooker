/*This code included in setup initialises Serial, checks AppConfig struct < 511 bytes,
  initialises AppConfig struct with values from EEPROM
  Attempts to connect Wifi
  Starts MDNS
*/

int WiFiTimeout = 0;

Serial.begin(SerialSpeed);
for (uint8_t t = 4; t > 0; t--)
{
  Serial.println(F("[SETUP] WAIT ..\n"));
  Serial.flush();
  delay(100);
}

Serial.println(F("SizeOf AppConfig:"));
Serial.println(sizeof(AppConfig));
if (sizeof(AppConfig) > 511)
{
  while (1);
}
OutAppConfig();//Print Pre EEPROM Config

// Initialize application related NV variables
InitAppConfig();
OutAppConfig();
Serial.println();
Serial.print(F("Hostname: "));

if (AppConfig.EpHostname == "notset")
{
  sprintf(AppConfig.EpHostname, "ESP_%06X", ESP.getChipId());
}
Serial.println(AppConfig.EpHostname);
Serial.print(F("Build Date: "));
Serial.print(__DATE__);
Serial.print(F("   Time: "));
Serial.print(__TIME__);
Serial.print(F("   Board: "));
Serial.println(ARDUINO_BOARD);

WiFi.disconnect();
//WiFi.setAutoConnect(false);
WiFi.persistent(false);
WiFi.mode(WIFI_STA);    // As End Point Only (ie wifi Station/Client)
WiFi.hostname(AppConfig.EpHostname);
if (AppConfig.Flags.bUseStaticIP)
{
  WiFi.config(AppConfig.EpIP, AppConfig.EpDNS, AppConfig.EpGW, AppConfig.EpMASK);
}

for (uint8_t ap_list_index = 0; (ap_list_index < numAccessPoints); ap_list_index++)
{
  //Serial.println(sizeof(AppConfig.access_point));
  Serial.println();
  Serial.print(F("Trying WAP:"));
  Serial.print(AppConfig.access_point[ap_list_index][0]);

  WiFi.begin(AppConfig.access_point[ap_list_index][indx_SSID], AppConfig.access_point[ap_list_index][indx_KEY]);
  WiFiTimeout = 0;
  while ((WiFi.status() != WL_CONNECTED) && (++WiFiTimeout != 15) )
  {
    delay(500);
    Serial.print(F("."));
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println(F("Connected OK"));
    break;
  }
}

if (WiFi.status() != WL_CONNECTED) //WiFiTimeout == 10)
{
  yield();
  Serial.println(F("Huston we have a problem. Trying to create Access Point"));
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);     // As Access Point Only

  //bool softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
  WiFi.softAPConfig(AppConfig.ApIP, AppConfig.ApGW, AppConfig.ApMASK);
  WiFi.softAP(AppConfig.ApSSID);
  delay(3500);
}

Serial.println();
Serial.print(F("IP Address: "));
Serial.println(WiFi.localIP());
if (MDNS.begin(AppConfig.EpHostname))
{
  Serial.println(F("MDNS responder started"));
  MDNS.addService("esp", "tcp", 8080);
}
