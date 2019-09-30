void handle_pid_data_only()
{
  START_HTML;
  START_PAGE(AppConfig.EpHostname, " PID Data Only");

  server.sendContent(F("<div id='main'>"
                       "easycook PID controller.&nbsp;<br>"
                       "&emsp;ticks&emsp;&emsp;,&emsp;&#176;C&emsp;&nbsp;, error&emsp;, pTerm&nbsp; , ITerm&nbsp;&nbsp; , dTerm&nbsp;&nbsp; , CV&nbsp;,&emsp;&nbsp;Millis&emsp;&nbsp;&nbsp; , %Hum,&nbsp;State"
                       "</div>"

                       "<div id='arrfieldoutputheadings'>"
                       "</div>"

                       "<div id='arrfieldoutputvalues'>"
                       "</div>"

                       "<script>"
                       "var i=0;var k=0;"
                       "var connection=new WebSocket('ws://'+location.hostname+':81/',['arduino']);"

                       "connection.onopen=function(){connection.send('Connect '+new Date());};"
                       "connection.onerror=function(error){console.log('WebSocket Error ',error);};"

                       "connection.onmessage=function(e){console.log('Server: ',e.data);"

                       "var msg=e.data;"
                       "if(msg.indexOf('"PID_DATA_PREAMBLE"') == 0){" //is the first char the preamble
                       "msg = msg.slice(1);"
                       "var arrmsg=msg.split(',');"
                       "var elements=document.getElementById('arrfieldoutputvalues');"
                       "var d = new Date();"
                       "elements.outerHTML += '<br>' + msg + d.toLocaleDateString() + ' ' + d.toLocaleTimeString();"
                       "}};"

                       "</script>"
                       "<br>"));
  END_PAGE_CUSTOM;
  END_PAGE;
}
