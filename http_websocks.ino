// handle index

//https://en.wikipedia.org/wiki/List_of_Unicode_characters
//https://en.wikipedia.org/wiki/Numeric_character_reference
//https://en.wikipedia.org/wiki/List_of_XML_and_HTML_character_entity_references#Character_entity_references_in_HTML
// variable names were heavily abbreviated as there was a limit to the size of the page (variable string) that could be sent
// This has now been resolved breaking the page up and using many "server.sendContent(F("
// tf=textfield
// idx=arrindex
// cp=arrfieldsinput or control parameters

void handle_pid_websockets() {


  START_HTML;  //Macro
  START_PAGE(AppConfig.EpHostname, " PID Controller"
             "<svg id='svg1' style='width: 55px; height: 26px'>"
             //Holder for the red circle that flashes in time with PWM
             "<circle id = 'circle1' r = '10' style = 'fill: red; stroke: black; stroke-width: 1' cx = '22' cy = '15'> </circle>"
             "</svg>");


  END_PAGE_CUSTOM;
  END_PAGE;
  server.sendContent(F(
                       "<div align='center' id='outerdiv' style='width: 885px;border:5px solid #000000;margin: auto;'>"
                       "<style>.b{white-space:normal;width: 63px;height: 40px;}</style>"
                       "<div id='butts'>"
                       //"<svg id='svg1' style='float:right;width: 35px; height: 40px'>"
                       "<input class='b'type='button'value='Jerky Dryer' onclick=setpr('sp1=72.5&sp2=57.2&sp1td=5&sp2td=360&kp=5&ki=0.4&kd=6&mvmax=120&mvmin=0');>"
                       "<input class='b'type='button'value='Yogurt Maker' onclick=setpr('sp1=82&sp2=44.5&sp1td=10&sp2td=240&kp=5&ki=0.4&kd=6&mvmax=100&mvmin=0');>"
                       "<input class='b'type='button'value='Egg Incubate' onclick=setpr('sp1=37.5&sp2=37.5&sp1td=0&sp2td=0&kp=1&ki=0.4&kd=1&mvmax=80&mvmin=0');>"
                       "<input class='b'type='button'value='Egg SousVide' onclick=setpr('sp1=64&sp2=63&sp1td=5&sp2td=120&kp=20&ki=0.4&kd=10&mvmax=140&mvmin=0');>"
                       "<input class='b'type='button'value='Black Garlic' onclick=setpr('sp1=54&sp2=76&sp1td=600&sp2td=0&kp=20&ki=0.4&kd=10&mvmax=140&mvmin=0');>"
                       "<input class='b'type='button'value='Solder Reflow' onclick=setpr('sp1=150&sp2=248&sp1td=60&sp2td=30&kp=2&ki=0.4&kd=1&mvmax=180&mvmin=0');>"
                       "<input class='b'type='button'value='Toggle Units' id='idUnits' onclick=toggleunits();>"
                       "</div>"
                       "<br>"

                       //afoh=arrfieldoutputheadings
                       "<div id='afoh'></div>"

                       //afov=arrfieldoutputvalues
                       "<div id='afov'></div>"
                       "<br>"

                       //afih=arrfieldinputheadings
                       "<div id='afih'></div>"

                       //afic=arrfieldinputvalues
                       "<div id='afiv'></div>"

                       // https://en.wikipedia.org/wiki/List_of_XML_and_HTML_character_entity_references
                       "<div id='idpvtxt'><br><font color='red'>pv (temperature &#176;C)</font><font color='blue'> hum (Humidity %)</font></div>"

                       //This canvas contains the Temperature and Humidy
                       "<canvas onclick='showCoords(event)'  id='mypvCanvas' width='825' height='150' style='border:5px solid #FFAD21;'>Browser no support HTML5 canvas tag.</canvas>"

                       "<div  id='idmvtxt'><br><font color='red'> CV (aka MV)</font><font color='green'> pterm </font><font color='blue'> iterm </font><font color='black'> dterm </font> </div>"

                       //This canvas contains the mv & PID terms
                       "<canvas  onclick='showCoords(event)' id='mymvCanvas' width='825' height='150' style='border:5px solid #FFAD21;'>Browser no support HTML5 canvas tag.</canvas>"

                       "<div><br>pid tuning levers</div>"
                       "<div style=' border:5px solid #FFAD21;' id='arrrangeinput'></div>"
                       "</div>"
                       "<script>"

                       "const sCX=825;" //X axis Size of Canvas
                       "const sCY=150;" //Y axis size of Canvas
                       "var mPV=10;" //Maximum Value of the PV or temperature
                       "var minPV=5;" //min Valie of the PV
                       "const maxYpwm=255;"
                       "const minYpwm=0;"
                       "var PVS=sCY/(mPV-minPV);" //PVS=scaler for the pr Process Varaiable Scaler
                       "const CVS=sCY/(maxYpwm-minYpwm);")); //CVS Control Variable Scaler

  /*
    server.sendContent(String(AppConfig.PID[sp1]));
    server.sendContent(F(";"));
    server.sendContent(String(AppConfig.PID[sp2]));
    server.sendContent(F(";"));
  */
  server.sendContent(F("var arrpv=[];"
                       "var arrhum=[];"
                       "var arrmv=[];"
                       "var arrpterm=[];"
                       "var arriterm=[];"
                       "var arrdterm=[];"
                       "var units='cels';"

                       //"Number.prototype.map = function (in_min, in_max, out_min, out_max) {return (this - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;}"
                       "Number.prototype.map = function (in_min, in_max, out_min, out_max) {return ((this - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);};"  //Need the last semicolon not sure why

                       "function showCoords(event) {var canvas=document.getElementById(event.currentTarget.id);var ctx = canvas.getContext('2d');var rect = event.currentTarget.getBoundingClientRect();var x = event.clientX-rect.left;var y = event.clientY-rect.top;if(event.currentTarget.id == 'mypvCanvas'){var Val = y.map(sCY,0,minPV,mPV)}else{var Val = y.map(sCY,0,0,255)};if(units == 'F'){Val = Val * 1.8 + 32;} ctx.strokeText(Val.toFixed(2), x, y);}"

                       "function toggleunits() {"
                       "if(units == 'F'){"
                       "units = 'C';"
                       "document.getElementById('idUnits').value='Units'+' "CELSIUS"';"

                       "document.getElementById('idpvtxt').innerHTML = `<br><font color='red'>pv (temperature &#176;C)</font><font color='blue'> hum (Humidity %)</font>`;"
                       "document.getElementById('idinputsp1').value = document.getElementById('idrangesp1').value;"
                       "document.getElementById('idinputsp2').value = document.getElementById('idrangesp2').value;"
                       "}else{"
                       "units = 'F';"
                       "document.getElementById('idUnits').value='Units'+' "FARENHEIT"';"
                       "document.getElementById('idpvtxt').innerHTML = `<br><font color='red'>pv (temperature &#176;F)</font><font color='blue'> hum (Humidity %)</font>`;"
                       "document.getElementById('idinputsp1').value = Math.round(document.getElementById('idrangesp1').value * 1.8 + 32);"
                       "document.getElementById('idinputsp2').value = Math.round(document.getElementById('idrangesp2').value * 1.8 + 32);"
                       "}"
                       "}"

                       //updatehtmltext processes changes in the sliders to make changes to the page and send updates to the ESP8266
                       "function updatehtmltext(ID) {"
                       "var posVal = document.getElementById('idrange'+ID).value;"

                       "if(ID == 'tc'){"
                       //"var position = Math.round(posVal);"
                       "var ptr = cp.indexOf(ID);"
                       "var minv = Math.log(cpmin[ptr]);"
                       "var maxv = Math.log(cpmax[ptr]);"
                       "var scale = (maxv-minv) / (cpmax[ptr]-cpmin[ptr]);"
                       "var posVal = (Math.exp(minv + scale*(posVal-cpmin[ptr])));"
                       "posVal = Math.round(posVal);"
                       "console.log(posVal);" //The Logrtithic version
                       "}"

                       "if(units == 'F' && (ID == 'sp1' || ID == 'sp2')){"
                       "document.getElementById('idinput'+ID).value=Math.round(posVal * 1.8 + 32);"
                       "}else{"
                       "document.getElementById('idinput'+ID).value=posVal;"
                       "}"
                       "var pidupdate='"  STRING_PREAMBLE "'+ID+','+posVal;console.log('pidupdate:'+pidupdate);"
                       "connection.send(pidupdate);"
                       "}"

                       //Send the button presses based on ID
                       "function sendbuttons(ID) {"
                       "var pidupdate='"BUTTON_PREAMPLE"'+ID+',button';" //,'+document.getElementById('idrange'+ID).value;console.log('pidupdate:'+pidupdate);"
                       "connection.send(pidupdate);"
                       "}"

                       //process key PRE-SET profiles. The key_value pair embedded in the button and update the web page and send values to the ESP8266
                       "function setpr(kvs) {"
                       //kvs key_values
                       "var pairs=kvs.split('&');"
                       "var i;"
                       "for(i=0;i<pairs.length;++i)"
                       "{"
                       "var kv=pairs[i].split('=');"
                       "if(units == 'F'){"
                       "document.getElementById('idrange'+kv[0]).value = kv[1] * 1.8 + 32;"
                       "document.getElementById('idinput'+kv[0]).value = kv[1] * 1.8 + 32;"
                       "}else{"
                       "document.getElementById('idrange'+kv[0]).value = kv[1];"
                       "document.getElementById('idinput'+kv[0]).value = kv[1];"
                       "}"
                       "console.log('pidupdate:'+'"STRING_PREAMBLE"'+kv[0]+','+kv[1]);"
                       "connection.send('"STRING_PREAMBLE"'+kv[0]+','+kv[1]);"
                       "}"
                       "}"


                       //many of the page elements are generated as the page is rendered.  This reduced the size of the page to send.  This is not so important now
                       //cp Control Parameter
                       //aFO=arrfieldsoutput
                       "var aFO=['ticks','PV','err','pterm','iterm','dterm','CV','millis','hum','state','timer'];"
                       "var cp=['sp1','sp2','sp1td','sp2td','kp','ki','kd','mvmax','mvmin','cal','tc'];"
                       "var cpdata=["));
  server.sendContent(String(AppConfig.PID[sp1]));
  server.sendContent(F(","));
  server.sendContent(String(AppConfig.PID[sp2]));
  server.sendContent(F(","));
  server.sendContent(String(AppConfig.PID[sp1td]));
  server.sendContent(F(","));
  server.sendContent(String(AppConfig.PID[sp2td]));
  server.sendContent(F(","));
  server.sendContent(String(AppConfig.PID[kp]));
  server.sendContent(F(","));
  server.sendContent(String(AppConfig.PID[ki]));
  server.sendContent(F(","));
  server.sendContent(String(AppConfig.PID[kd]));

  server.sendContent(F(","));
  server.sendContent(String(AppConfig.PID[mvmax]));
  server.sendContent(F(","));
  server.sendContent(String(AppConfig.PID[mvmin]));
  server.sendContent(F(","));
  server.sendContent(String(AppConfig.PID[cal]));
  server.sendContent(F(","));
  //server.sendContent(String(pow((AppConfig.PID[tc]/1000),3)));
  server.sendContent(String(AppConfig.PID[tc] / 1000));
  server.sendContent(F("];"));

  server.sendContent(F("var cpmax=[255,255,2000,2000,30,30,30,255,255,20,86400];"
                       "var cpmin=[0.1,0,0,0,0,0,0,0,0,-20,1];"
                       "var cpstep=[0.1,0.1,1,1,0.01,0.01,0.01,1,1,0.1,1];"

                       //add buttons
                       "var arrbutts=['Mode','Active','Reset','Save'];"
                       "var arrbuttsval=['Mode Run','Active High','Reset Restart','Save State'];"
                       "var element=document.getElementById('butts');"
                       "for (idx=0;idx<arrbutts.length;idx++)"
                       "{"
                       "tf=document.createElement('input');"
                       "element.appendChild(tf);"
                       "tf.type='button';"
                       "tf.id='id'+arrbutts[idx];"
                       "tf.value=arrbuttsval[idx];"
                       "tf.setAttribute('onclick','sendbuttons(this.id)');"
                       //"tf.style.width='60px';"
                       "tf.setAttribute('class','b');"
                       "}"

                       // add output fields ***************"
                       "var element=document.getElementById('afoh');"
                       "for (idx=0;idx<aFO.length;idx++)"
                       "{"
                       "tf=document.createElement('input');"
                       "element.appendChild(tf);"
                       "tf.value=aFO[idx];"
                       "tf.disabled = true;"
                       "tf.style.width='75px';"
                       "tf.style.textAlign='center';"
                       "}"
                       "var element=document.getElementById('afov');"
                       "for (idx=0;idx<aFO.length;idx++)"
                       "{"
                       "tf=document.createElement('input');"
                       "element.appendChild(tf);"
                       "tf.id='id'+aFO[idx];"
                       "tf.disabled = true;"
                       "tf.style.width='75px';"
                       //"tf.setAttribute('text-align','center');"
                       "tf.style.textAlign='center';"
                       "}"

                       // add input fields ***************"
                       "var element=document.getElementById('afih');"
                       "for (idx=0;idx<cp.length;idx++)"
                       "{"
                       "tf=document.createElement('input');"
                       "element.appendChild(tf);"
                       "tf.value=cp[idx];"
                       "tf.disabled = true;"
                       "tf.style.width='75px';"
                       "tf.style.textAlign='center';"
                       "}"
                       "var element=document.getElementById('afiv');"
                       "for (idx=0;idx<cp.length;idx++)"
                       "{"
                       "tf=document.createElement('input');"
                       "element.appendChild(tf);"
                       "tf.value=cpdata[idx];"
                       "tf.id='idinput'+cp[idx];"
                       "tf.disabled = true;"
                       "tf.style.width='75px';"
                       "tf.style.textAlign='center';"
                       "}"

                       //add sliders
                       "var element=document.getElementById('arrrangeinput');"
                       "var element2=document.getElementById('arrrangeinputtags');"
                       //"tf2=document.createElement('PRE');"
                       //"element2.appendChild(tf2);"
                       "for(idx=0;idx<cp.length;idx++)"
                       "{"
                       //"tf2.innerHTML+=cp[idx]+'&#10&#10';"
                       "tf=document.createElement('input');"
                       "element.appendChild(tf);"
                       "tf.outerHTML=`<label  for=`+`'idrange`+cp[idx]+`'>`+cp[idx]+`</label>`+`<input style='width:800px;' type='range'value='`+cpdata[idx]+`'id='idrange`+cp[idx]+`'oninput='updatehtmltext(&#34;`+cp[idx]+`&#34;);'min='`+cpmin[idx]+`'max='`+cpmax[idx]+`'step='`+cpstep[idx]+`'><br>&#10`;"
                       //"tf.outerHTML=`<label style='float:left;' for=`+`'idrange`+cp[idx]+`'>`+cp[idx]+`</label>`+`<input type='range'value='`+cpdata[idx]+`'id='idrange`+cp[idx]+`'oninput='updatehtmltext(&#34;`+cp[idx]+`&#34;);'min='`+cpmin[idx]+`'max='`+cpmax[idx]+`'step='0.1'style='width:500px;'>`;"
                       //"tf.outerHTML=`<input type='range'value='`+cpdata[idx]+`'id='idrange`+cp[idx]+`'oninput='updatehtmltext(&#34;`+cp[idx]+`&#34;);'min='`+cpmin[idx]+`'max='`+cpmax[idx]+`'step='0.1'style='width:765px;height:24px;'>`;"
                       "}"

                       //get and decode the messages sent from the esp8266
                       "var i=0;var k=0;"

                       //##############################################################################################################
                       // The javascript below processes the data as it is sent from the ESP8266.  It saves the data in arrays are uses this to draw the charts
                       //
                       "var connection=new WebSocket('ws://'+location.hostname+':81/',['arduino']);"

                       "connection.onopen=function(){connection.send('Connect '+new Date());};"
                       "connection.onerror=function(error){console.log('WebSocket Error ',error);};"
                       "connection.onclose=function(event){var readystate = connection.readyState;"
                       "console.log('readstate:',readystate);"

                       "if(readystate == 3){location.reload(1);}"
                       "};"
                       "connection.onmessage=function(e){console.log('Server:',e.data);"
                       "var msg=e.data;"
                       "if(msg!='Connected'){"
                       "if(msg=='"HEATER_ON"'){circle1.style.fill='red';}"
                       "else if(msg=='"HEATER_OFF"'){circle1.style.fill='DarkRed';}"
                       "else if(msg=='"MODE_AUTO"'){document.getElementById('idMode').value='"MODE_AUTO"';}"
                       "else if(msg=='"FORCED_HIGH"'){document.getElementById('idMode').value='"FORCED_HIGH"';}"
                       "else if(msg=='"FORCED_LOW"'){document.getElementById('idMode').value='"FORCED_LOW"';}"

                       "else if(msg=='"INVERTED"'){document.getElementById('idActive').value='"INVERTED"';}"
                       "else if(msg=='"NONINVERTED"'){document.getElementById('idActive').value='"NONINVERTED"';}"

                       "else if(msg.indexOf('"PID_DATA_PREAMBLE"') == 0){"
                       "msg = msg.slice(1);"
                       "var arrmsg=msg.split(','),ticks=parseInt(arrmsg[0]), PV=parseFloat(arrmsg[1]), error=parseFloat(arrmsg[2]), pTerm=parseFloat(arrmsg[3]), iTerm=parseFloat(arrmsg[4]), dTerm=parseFloat(arrmsg[5]), CV=parseInt(arrmsg[6]), millis_t=arrmsg[7], currentHumidity=parseFloat(arrmsg[8]), strstate=arrmsg[9], countdowntimer=arrmsg[10];"
                       //"var ticks, currentHeat, error, pTerm, iTerm, dTerm, CV, millis_t, currentHumidity, strstate, countdowntimer=msg.split(',');"
                       "var elements=document.getElementById('afov');"
                       "for (idx=0;idx<aFO.length;idx++)"
                       "{"
                       //"if(isNaN(arrmsg[idx])){ arrmsg[idx] = -1;}"
                       "if(idx == 1 && units == 'F'){"
                       "elements.children[idx].value=Math.round(arrmsg[idx]* 1.8 + 32);"
                       "}else{"
                       "elements.children[idx].value=arrmsg[idx];"
                       "}"
                       "}"
                       "if(isNaN(PV)){PV=-1;}" //Otherwise the auto scaling breaks if PV is ever a NaN i.e Prope disconnected
                       "if(isNaN(currentHumidity)){currentHumidity=-1;}"

                       //pv (index=1) Chart temperature

                       "var canvas=document.getElementById('mypvCanvas');"

                       "var ctx=canvas.getContext('2d');"
                       "ctx.clearRect(0,0,sCX,sCY);"
                       "ctx.beginPath();"
                       "ctx.fillStyle='black';" //lightgrey';"
                       "ctx.fillRect(0,0,sCX,sCY);"
                       "ctx.beginPath();"

                       "ctx.font = 'Courier New';"
                       //"ctx.stroke();"

                       "if(arrpv.length >sCX){arrpv.push(arrpv.shift());arrpv.pop();}"
                       "arrpv.push(PV);"

                       //"ctx.strokeStyle='green';"
                       //"console.log(Math.max(...arrpv));"
                       //"if( PV > mPV) {mPV = PV;}"
                       "mPV = Math.max(...arrpv)+0.2;"
                       "minPV = Math.min(...arrpv)-0.2;"
                       "PVS=sCY/(mPV-minPV);"
                       "var stage1=(mPV-idinputsp1.value)*PVS;"
                       "var stage2=(mPV-idinputsp2.value)*PVS;"

                       //"ctx.rect(0,stage1dry,sCX,stage2dry-stage1dry);" //Draw upper-lower limits
                       "ctx.rect(0,stage1,sCX,stage2-stage1);"
                       "ctx.stroke();"
                       "ctx.beginPath();"

                       "ctx.strokeStyle='red';"
                       "for(cnt=0;cnt<arrpv.length;cnt++){"
                       "ctx.beginPath();"
                       "ctx.moveTo(cnt,sCY-((arrpv[cnt]-minPV)*PVS));"
                       "ctx.lineTo((cnt+1),sCY-((arrpv[cnt+1]-minPV)*PVS));"
                       "ctx.stroke();"
                       "}"

                       //hum (index=8) Chart humidty
                       "if(arrhum.length >sCX){arrhum.push(arrhum.shift());arrhum.pop();}"
                       "arrhum.push(currentHumidity);"

                       "ctx.strokeStyle='lightblue';"
                       "for(cnt=0;cnt<arrhum.length;cnt++){"
                       "ctx.beginPath();"
                       // "ctx.moveTo(cnt,sCY-((arrhum[cnt]-minPV)*PVS));"
                       // "ctx.lineTo((cnt+1),sCY-((arrhum[cnt+1]-minPV)*PVS));"
                       "ctx.moveTo(cnt,sCY-((sCY/100) * arrhum[cnt]));"
                       "ctx.lineTo((cnt+1),sCY-((sCY/100) * arrhum[cnt+1]));"
                       "ctx.stroke();"
                       "}"

                       //draw tick marks for PV
                       "ctx.strokeStyle = 'green';"
                       "var j=0;"
                       "for(i =0;i< sCX;i+=60){" //600000/ document.getElementById('idinputtc').value){"
                       "ctx.beginPath();"
                       "ctx.moveTo(i,0);"
                       "ctx.lineTo(i,sCY);"
                       "ctx.stroke();"
                       "ctx.strokeText(j+'min',i+1,sCY-2);" //.toFixed(2)   ctx.strokeText(parseFloat(j).toFixed(2)+'min',i+1,sCY-2);
                       "j=j+document.getElementById('idinputtc').value/1;" //10
                       "}"

                       //draw min max ticks
                       "if(units == 'F'){"
                       "var dispval = minPV * 1.8 + 32;"
                       "ctx.strokeText(dispval.toFixed(1),0,sCY-10);" //-(minPV*PVS));"
                       "var dispval = mPV * 1.8 + 32;"
                       "ctx.strokeText(dispval.toFixed(1),0,10);" //sCY-(mPV*PVS));"
                       "} else {"
                       "ctx.strokeText(minPV.toFixed(1),0,sCY-10);" //-(minPV*PVS));"
                       "ctx.strokeText(mPV.toFixed(1),0,10);" //sCY-(mPV*PVS));"
                       "}"

                       //CV (index=6) Chart pwm aka mv Manipulated variable

                       "var canvas=document.getElementById('mymvCanvas');"

                       "var ctx=canvas.getContext('2d');"
                       "ctx.clearRect(0,0,sCX,sCY);"
                       "ctx.beginPath();"
                       "ctx.beginPath();"
                       "ctx.fillStyle='black';" //"ctx.fillStyle='whitesmoke';"
                       "ctx.fillRect(0,0,sCX,sCY);"
                       "ctx.beginPath();"

                       "if(arrmv.length >sCX){arrmv.push(arrmv.shift());arrmv.pop();}"
                       "arrmv.push(CV);"

                       "for(cnt=0;cnt<arrmv.length;cnt++){"
                       "ctx.beginPath();"
                       "ctx.moveTo(cnt,sCY-((arrmv[cnt]-minYpwm)*CVS));"
                       "ctx.lineTo((cnt+1),sCY-((arrmv[cnt+1]-minYpwm)*CVS));"
                       "ctx.strokeStyle='red';" //#000FFF';"
                       "ctx.stroke();"
                       "}"

                       //pterm (index=3) Chart proportional error

                       "if(arrpterm.length >sCX){arrpterm.push(arrpterm.shift());arrpterm.pop();}"
                       "arrpterm.push(pTerm);"

                       "for(cnt=0;cnt<arrpterm.length;cnt++){"
                       "ctx.beginPath();"
                       "ctx.moveTo(cnt,sCY-((arrpterm[cnt]-minYpwm+125)*CVS));"
                       "ctx.lineTo((cnt+1),sCY-((arrpterm[cnt+1]-minYpwm+125)*CVS));"
                       "ctx.strokeStyle='green';"
                       "ctx.stroke();"
                       "}"

                       //iTerm (index=4) Chart integral error

                       "if(arriterm.length >sCX){arriterm.push(arriterm.shift());arriterm.pop();}"
                       "arriterm.push(iTerm);"

                       "for(cnt=0;cnt<arriterm.length;cnt++){"
                       "ctx.beginPath();"
                       "ctx.moveTo(cnt,sCY-((arriterm[cnt]-minYpwm)*CVS));"
                       "ctx.lineTo((cnt+1),sCY-((arriterm[cnt+1]-minYpwm)*CVS));"
                       "ctx.strokeStyle='lightblue';"
                       "ctx.stroke();"
                       "}"

                       //dterm (index=5) Chart differential error

                       "if(arrdterm.length >sCX){arrdterm.push(arrdterm.shift());arrdterm.pop();}"
                       "arrdterm.push(dTerm);"

                       "for(cnt=0;cnt<arrdterm.length;cnt++){"
                       "ctx.beginPath();"
                       "ctx.moveTo(cnt,sCY-((arrdterm[cnt]-minYpwm+125)*CVS));"
                       "ctx.lineTo((cnt+1),sCY-((arrdterm[cnt+1]-minYpwm+125)*CVS));"
                       "ctx.strokeStyle='white';"
                       "ctx.stroke();"
                       "}"
                       //draw tick marks
                       "ctx.strokeStyle = 'green';"
                       "var j=0;"
                       "for(i =0;i< sCX;i+=60){" //0000/ document.getElementById('idinputtc').value){"
                       "ctx.beginPath();"
                       "ctx.moveTo(i,0);"
                       "ctx.lineTo(i,sCY);"
                       "ctx.stroke();"
                       "ctx.strokeText(j+'min',i+1,sCY-2);" //j.toFixed(2)
                       "j=j+document.getElementById('idinputtc').value/1;" //10;" divide by 1 forces j to a number otherwise it is a string and concates like a string
                       //"j=j+10;"
                       "}" //end of for above
                       "}" //end of else
                       "}" //end of if connected
                       "};" //end of onmessage
                       "</script>"));

}
