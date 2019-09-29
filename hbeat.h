//#define URL_Heartbeat "http://www.pickledeye.com:8181/cgi-bin/meltdown.pl?hn="
//#define URL_Heartbeat "http://172.16.0.20/cgi-bin/jotomon.pl?"
void sendHeartBeat(void)
{
  IPAddress ipaddr;
  char buffers [200];
  //char ipaddr[50];
  ipaddr = WiFi.localIP();//.toString();
  //Serial.println(F("Sending HeartBeat ... " );

  sprintf(buffers, "%shn=%s&ip=%03d.%03d.%03d.%03d&ticks=%010d&heat=%06.2f&Humidity=%06.2f&pid=%03d&err=%06.2f&iterm=%06.2f&dTerm=%06.2f&output=%1d&ms=%010d",
          URL_Heartbeat, AppConfig.EpHostname, ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3], ticks, currentHeat, currentHumidity, CV, error, iTerm, dTerm, getPinMode(HEATER), millis());

  // http.begin(URL_Heartbeat + String(AppConfig.EpHostname) + "&ip=" + WiFi.localIP().toString() + buffers);//"&millis=" + millis() + "&joto=" +  currentHeat + "&hum=" + currentHumidity);
  http.begin(buffers);//"&millis=" + millis() + "&joto=" +  currentHeat + "&hum=" + currentHumidity);
  Serial.print(buffers);
  int httpCode = http.GET();
  Serial.print(F(" Http Resp: "));
  Serial.println(httpCode);
  /*  if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);
    }
  */
  //sprintf(buffers,"%s,%03d.%03d.%03d.%03d",AppConfig.EpHostname,ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3]);
  //Serial.println(buffers);
  http.end();
}
