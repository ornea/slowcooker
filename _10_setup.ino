void setup()
{
#include "_init.h"  //
#include "http.h"
#include "OTA.h"
  server.on("/", handle_root);
  server.on("/pid_config", handle_pid_config);
  server.on("/pid_config_update", handle_pid_config_update);
  server.on("/pid_websockets", handle_pid_websockets);
  server.on("/pid_dataonly", handle_pid_data_only);
  server.on("/pid_status",  handle_pid_status);
  server.on("/pid_help", handle_pid_help);
  server.on("/config", handle_config);
  server.on("/config_update", handle_config_update);
  server.on("/build", handle_build);
  server.on("/IO_read",  handle_IO_read);
  server.on("/IO_write",  handle_IO_write);

  server.on("/draw", handle_draw);
  server.on("/restart", handle_restart);
  server.onNotFound(handle_not_found);
  server.begin();
  Serial.println(F("HTTP server started"));

  Serial.println();
  Serial.println();
  Serial.println(F("Starting"));

  HEATER_LOW;
  pinMode(HEATER, OUTPUT);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

#ifdef USE_DHT
  dht.begin();
#endif
}
