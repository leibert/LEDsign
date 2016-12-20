#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

//WIFI ssid and password in include file
#include <passwords.h>

char* CMDCTRLaddr;
char* CMDCTRLhost;

String SIGNcontent;



SoftwareSerial swSer(D3, D2, false, 256);  //D2 WHITE TX, D3 GREY RX



//init WiFi propoerties
WiFiClient client;
WiFiServer server(80);

//timer
int MCLKmsec, MCLKsec, MCLKminutes, MCLKhours;
int TMRmsec, TMRsec, TMRminutes, TMRhours;

int defaultDELAY;
bool waitDELAY;
bool secondFLAG;
bool forceUpdate;
int currentDELAY;
int CMDCTRLposition;

bool overrideFLAG;




//connect to wifi
void startWIFI() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //connect to network using cons credentials
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { //wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}


String SIGNCOLOR(char color) {
  String S = "";
  switch (color) {
    case 'R':
      S = "COLOR 1 1";
      break;
    case 'G':
      S = "COLOR 1 2";
      break;
    case 'Y':
      S = "COLOR 1 3";
      break;
    default: //anything else just set to RED
      S = "COLOR 1 1";
      break;
  }
  return S;
}

void SCROLLSIGN(String text, char color, int timedly) {
  
  Serial.println("SIGN SCROLLING"+text);
  swSer.println("ADDR 000");  //SET ADDRESS TO SIGN
  swSer.println("HALT");  //STOP ANYTHING ELSE GOING ON
  swSer.println("FORMAT 1 5");  //ENSURE ITS IN LABEL MODE
  swSer.println(SIGNCOLOR(color)); //SET COLOR
  text=text+"          ";
  
  while (text.length()>0 && text.length()>9){
    swSer.println("LABEL 1 " + text.substring(0,12)); //SEND TEXT
    swSer.println("STEP 1");  //SET SIGN TO TEXT
    text=text.substring(1);
    Serial.println(text);
    delay(500);    
  }
}



void WRITESIGN(String text, char color) {
  //  Serial.println("ADDR 000");  //SET ADDRESS TO SIGN
  //  Serial.println("HALT");  //STOP ANYTHING ELSE GOING ON
  //  Serial.println("LABEL 1 " + text); //SEND TEXT
  //  Serial.println("FORMAT 1 5");  //ENSURE ITS IN LABEL MODE
  //  Serial.println(SIGNCOLOR(color)); //SET COLOR
  //  Serial.println("STEP 1");  //SET SIGN TO TEXT
  Serial.println("WRITE TO SIGN:" + text);
//  if(text.length()>14){
//    Serial.println("TOO LONG, SCROLLING");
//    SCROLLSIGN(text,color,200);
//    return;
//  }


  swSer.println("ADDR 000");  //SET ADDRESS TO SIGN
  swSer.println("HALT");  //STOP ANYTHING ELSE GOING ON
  swSer.println("LABEL 1 " + text); //SEND TEXT
  swSer.println("FORMAT 1 5");  //ENSURE ITS IN LABEL MODE
  swSer.println(SIGNCOLOR(color)); //SET COLOR
  swSer.println("STEP 1");  //SET SIGN TO TEXT

}

void CLEARSIGN() {
  //  Serial.println("ADDR 000");  //SET ADDRESS TO SIGN
  //  Serial.println("HALT");  //STOP ANYTHING ELSE GOING ON
  //  Serial.println("LABEL 0 "); //SEND TEXT
  //  Serial.println("FORMAT 0 5");  //ENSURE ITS IN LABEL MODE
  //  Serial.println("STEP 0");  //SET SIGN TO TEXT
  Serial.println("CLEAR SIGN");

  swSer.println("ADDR 000");  //SET ADDRESS TO SIGN
  swSer.println("HALT");  //STOP ANYTHING ELSE GOING ON
  swSer.println("LABEL 0 "); //SEND TEXT
  swSer.println("FORMAT 0 5");  //ENSURE ITS IN LABEL MODE
  swSer.println("STEP 0");  //SET SIGN TO TEXT

}


void initLEDSIGN() {
  CLEARSIGN();
  delay(1000);
  WRITESIGN("Internet", 'R');
  delay(2000);
  WRITESIGN("of", 'G');
  delay(2000);
  for (int i = 0; i < 10; i++) {
    WRITESIGN("SHIT", 'Y');
    delay(300);
    CLEARSIGN();
    delay(150);
  }
  for (int i = 0; i < 5; i++) {
    WRITESIGN(WiFi.localIP().toString(), 'R');
    delay(500);
    CLEARSIGN();
    delay(200);
  }
  WRITESIGN(WiFi.localIP().toString(), 'G');
  delay(2000);

}


void getCMDCTRL(char* url) {
  Serial.println("localtion is" + String(CMDCTRLaddr));
  char* host = CMDCTRLhost;



  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  SIGNcontent = "";

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    //    Serial.print("asciR");
    String line = client.readStringUntil('\r');
    Serial.print(line);
    SIGNcontent += line;
  }

  Serial.println();
  Serial.println("closing connection");


}

void MCLKreset() {
  MCLKmsec = 0;
  MCLKsec = 0;
  MCLKminutes = 0;
  MCLKhours = 0;
}

void TMRreset() {
  TMRmsec = 0;
  TMRsec = 0;
  TMRminutes = 0;
  TMRhours = 0;
}

void ticker() {
  //  Serial.println("delay length");
  //  delay(incr);
  MCLKmsec += 100;
  TMRmsec += 100;
  //  Serial.println("TICK" + TMRmsec)

  if (MCLKmsec > 1000) {
    MCLKmsec = 0;
    MCLKsec++;
    secondFLAG=false;
    Serial.println("Second++");
  }
  if (MCLKsec > 60) {
    MCLKsec = 0;
    MCLKminutes++;
  }
  if (MCLKminutes > 60) {
    MCLKminutes = 0;
    MCLKhours++;
  }


}



void runSIGNtxt(String cmds) {

  if (waitDELAY && TMRmsec < currentDELAY)
    return;

  if (cmds.length() < 1)
    return;

  if(cmds.indexOf("~")>CMDCTRLposition||cmds.length()<CMDCTRLposition)
    CMDCTRLposition=(cmds.indexOf("~")+2);

  Serial.println(String(TMRsec) + " of " + String(currentDELAY));
  TMRmsec = 0;
  Serial.println("INCOMMANDS:" + String(CMDCTRLposition));
  Serial.println(cmds.length());

  int dl = cmds.indexOf('\n', CMDCTRLposition);

  Serial.println("to DL:" + String(dl));
  String t="";
  if(dl==-1){
    t = cmds.substring(CMDCTRLposition);
    forceUpdate=true;
  }
  else
    t = cmds.substring(CMDCTRLposition, dl);
  //WE NOW HAVE THE INDIVIDUAL LINE,CHECK FOR CONTROL CODE
  Serial.println(t);
  
  if(t[0]=='['){
    Serial.println("COMMANDS PASSED");
    char color=t[1];
    currentDELAY = (t.substring(2,t.indexOf(']'))).toInt();
    String toWrite = t.substring((t.indexOf(']')+1));
    WRITESIGN(toWrite, color);
    Serial.println(color);
    Serial.println(currentDELAY);
  }
  else{
    WRITESIGN(t, 'R'); 
    currentDELAY = defaultDELAY;  
  }
  

  CMDCTRLposition = dl+1;
  waitDELAY = true;
//  secondFLAG= true;
  return;



//IF THIS IS REACHED, HAVE RUN OUT OF DATA, LOOP BACK
//CMDCTRLposition = 0;




}

void setup() {
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  Serial.begin(57600);
  swSer.begin(9600);

  startWIFI();//Connect to WIFI network and start server
  initLEDSIGN();
  CMDCTRLhost = "192.168.0.31";
  CMDCTRLaddr = "/espserve/CMDCTRL/DWNSTRLEDsign.cmd";
  SIGNcontent = "";
  defaultDELAY = 3000;
  waitDELAY = true;

  MCLKmsec = 0;
  MCLKsec = 0;
  MCLKminutes = 0;
  MCLKhours = 0;

  TMRmsec = 0;
  TMRsec = 0;
  TMRminutes = 0;
  TMRhours = 0;
//  SCROLLSIGN("A Really long message to TEST SCROLLING is going here, this is the end",'G',200);
  Serial.println("INIT COMPLETED");
  delay(3000);
//  getCMDCTRL(CMDCTRLaddr);

  //  for (char ch = ' '; ch <= 'z'; ch++) {
  //    swSer.write(ch);
  //  }


}




void loop() {
  //  Serial.println("LOOP");
  delay(100);
  ticker();
  //if wifi connection has been lost, try to reconnect
  if (WiFi.status() != WL_CONNECTED) {
    delay(1);
    startWIFI();
    return;
  }
 

  if (forceUpdate||(MCLKsec % 45 == 0 && secondFLAG==false)){
    getCMDCTRL(CMDCTRLaddr);
    secondFLAG=true;
    forceUpdate=false;
  }
  
  if(overrideFLAG==false)
    runSIGNtxt(SIGNcontent);
  else if(MCLKminutes>30)
    overrideFLAG=true;

    // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return; //no one connected exit loop
  }
  Serial.println("CLIENT CONNECTION");
  
  String request = client.readStringUntil('\r');
  Serial.println(request); //serial debug output
  client.flush(); //trash data
  
  if (request.indexOf("TXT=") != -1) {
    String t = request.substring((request.indexOf("TXT=") + 4));
    Serial.println("Please display"+t);
    WRITESIGN(t,'R');
    TMRreset();
    MCLKreset();
    overrideFLAG=true;
  }


  else if (request.indexOf("RESTART") != -1) {
    MCLKreset();
    TMRreset();
    overrideFLAG=false;
    CMDCTRLposition=0;
    runSIGNtxt(SIGNcontent);
  }
  else if (request.indexOf("RESUME") != -1) {
    overrideFLAG=false;
    runSIGNtxt(SIGNcontent);
  }

  


//  while (swSer.available() > 0) {
//    Serial.write(swSer.read());
//  }
//  while (Serial.available() > 0) {
//    swSer.write(Serial.read());
//  }

}
