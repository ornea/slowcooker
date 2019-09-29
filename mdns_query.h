IPAddress remote_ip;

void get_controller_ip(void)
{
  remote_ip[0] = 0;
  remote_ip[1] = 0;
  remote_ip[2] = 0;
  remote_ip[3] = 0;

  Serial.println(F("Sending mDNS query"));
  MDNS.update();
  int n = MDNS.queryService("esp", "tcp"); // Send out query for esp tcp services
  Serial.println(F("mDNS query done"));
  if (n == 0) {
    Serial.println(F("no services found"));
  }
  else
  {
    Serial.print(n);
    Serial.println(F(" service(s) found"));
    for (int i = 0; i < n; ++i)
    {
      // Print details for each service found
      Serial.print(i + 1);
      Serial.print(F(": "));
      Serial.print(MDNS.hostname(i));
      Serial.print(F(":"));
      Serial.print(MDNS.IP(i));
      Serial.print(F(":"));
      Serial.print(MDNS.port(i));
      //		  if (MDNS.hostname(i) == _control_hostname)
      if (MDNS.hostname(i) == AppConfig.EpControlHostname)
      {
        Serial.print(F(" Controller Match"));
        remote_ip = MDNS.IP(i);
      }
      Serial.println();
    }
  }
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") + \
         String(ipAddress[1]) + String(".") + \
         String(ipAddress[2]) + String(".") + \
         String(ipAddress[3])  ;
}
