/* Notes
  server.send(204); //No Content - stops the source page from reloading
  server.sendHeader("Access-Control-Allow-Origin", "*"); //allows a page to auto load content
  server.send(200, "text/html", message);

*/

#define START_HTML  server.sendContent(F("HTTP/1.1 200 OK\r\n"\
                                       "Content-Type: text/html\r\n"\
                                       "Connection: close\r\n\r\n"\
                                       "<!DOCTYPE HTML>\r\n"));

//bgcolor='#E6E6FA'
//https://codebeautify.org/image-to-base64-converter to convert images to base64
#define START_PAGE(hostname,textPageName) server.sendContent( F("<html><head>" \
    "<meta name='viewport' content='width=device-width,initial-scale=1.0,user-scalable=no' >" \
    "<link href='data:image/bmp;base64,Qk32AAAAAAAAAHYAAAAoAAAAEAAAABAAAAABAAQAAAAAAIAAAADEDgAAxA4AAAAAAAAAAAAAAAAAAAAAgAAAgAAAAICAAIAAAACAAIAAgIAAAICAgADAwMAAAAD/AAD/AAAA//8A/wAAAP8A/wD//wAA////AMzP//////zM/P/MzMzM/8/4zI////jMj//8/////8//+Mz/////zI/8+M/P/PyPz/z/jP//yP/Pz8//zMz//Pyfn/+Zmf/5+fn/mf//mf+f+fmfn/n5n5/5mf////+Zn//5/////5//+Zn/////mZ/5/5mZmZn/n5mf//////mZ' rel='icon' type='image/x-icon' />" \
    "</head><body bgcolor='lightgray'><div align='center'><h2 align='center'>")); server.sendContent(hostname) ; server.sendContent(textPageName); server.sendContent(F("</h2>"));

#define END_PAGE server.sendContent(F("<br>Standard Pages: "\
                                      "<a href='/'>Home</a>&nbsp&nbsp"\
                                      "<a href='/config'>Configuration Page</a>&nbsp&nbsp" \
                                      "<a href='/build'>Build Status</a>&nbsp&nbsp" \
                                      "<a href='/IO_read'>IO Status</a>&nbsp&nbsp"   \
                                      "<a href='/IO_write'>IO Write</a>&nbsp&nbsp"  \
                                      "<a href='/restart'>Restart-Reboot Device</a>&nbsp&nbsp" \
                                      "</div></body></html>\r\n"));

#define END_PAGE_CUSTOM server.sendContent(F("Custom Pages: "\
    "<a href='/pid_config'>PID Config</a>&nbsp&nbsp"\
    "<a href='/pid_websockets'>PID websockets</a>&nbsp&nbsp"\
    "<a href='/pid_dataonly'>PID dataonly</a>&nbsp&nbsp"\
    "<a href='/pid_status'>PID status</a>&nbsp&nbsp"\
    "<a href='/pid_help'>PID Help Pages</a>&nbsp&nbsp"));

#define START_FORM(submit_to_page) server.sendContent(F( "<form action='/"));server.sendContent( submit_to_page); server.sendContent(F("'>"));

#define END_FORM server.sendContent(F("<br><input type='submit' value='Submit'></form>"));

//#define END_FORM_STRING(string) server.sendContent("<input type='submit' value='" #string);\
//server.sendContent(#string);
//    server.sendContent("'></form>");

#define START_TABLE(textTableName) server.sendContent(F("<h2>"));server.sendContent( F(textTableName));server.sendContent(F( "</h2><table border =1>"));

#define END_TABLE server.sendContent(F("</table>"));


#define ADD_INPUT_CELL(var,name) server.sendContent(F("<td><input type='text' value='"));server.sendContent(String(var));server.sendContent(F("' name = '")); server.sendContent(String(#name));\
  server.sendContent( F("'></td>"));

#define ADD_INPUT_CELL_INDEXED(var,name,index) server.sendContent(F("<td><input type='text' maxlength='4' size='4' value='"));\
  server.sendContent( var );\
  server.sendContent(F("' name = '"));\
  server.sendContent(name);\
  server.sendContent(index) ; \
  server.sendContent(F("'></td>"));
#define ADD_CELL_VAR(var) server.sendContent(F("<td>"));server.sendContent( var) ;server.sendContent(F("</td>"));
#define ADD_CELL_STRING(string) server.sendContent(F("<td>"));\
  server.sendContent(string) ;\
  server.sendContent(F("</td>"));
#define ADD_ROW_STRING(string) server.sendContent(F("<tr><td>"));server.sendContent( string);server.sendContent(F("</td>"));
#define ADD_ROW_VAR(var) server.sendContent(F("<tr><td>"));server.sendContent(var);server.sendContent(F("</td>"));
#define END_ROW server.sendContent(F( "</tr>"));

#define START_DROP_DOWN(name) server.sendContent(name);\
  server.sendContent(F(" <select name='"));\
  server.sendContent(name); \
  server.sendContent(F("'>"));
#define ADD_OPTION(option) server.sendContent(F("<option value='"));\
  server.sendContent(option);\
  server.sendContent(F("'>"));\
  server.sendContent(option);\
  server.sendContent(F("</option>"));
#define ADD_OPTION_SELECTED(option) server.sendContent(F("<option selected='selected'>"));\
  server.sendContent(option);\
  server.sendContent(F("</option>"));


#define END_DROP_DOWN server.sendContent(F("</select>"));
#define INSERT_BREAK server.sendContent(F("<br>"));
