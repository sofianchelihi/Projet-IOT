#include <WiFiNINA.h>

int motor11 = 0;//droite
int motor12 = 1;
int motor1pmw = 2;

int motor21 = 6;//gauche
int motor22 = 7;
int motor2pmw = 5;

int trig = 9;
int echo = 8;
int vitess =60;

int autom =0;
int autom2;




char ssid[] = "Arduino";        // your network SSID (name)
char pass[] = "12345678";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);





void avancer() {
digitalWrite(motor11, LOW);
digitalWrite(motor12, HIGH);
analogWrite(motor1pmw, vitess);

digitalWrite(motor21, HIGH);
digitalWrite(motor22, LOW);
analogWrite(motor2pmw, vitess);
}

void reculer() {
digitalWrite(motor11, HIGH);
digitalWrite(motor12, LOW);
analogWrite(motor1pmw, vitess);

digitalWrite(motor21, LOW);
digitalWrite(motor22, HIGH);
analogWrite(motor2pmw, vitess);
}


void tourner_gauche() {
digitalWrite(motor11, LOW);
digitalWrite(motor12, HIGH);
analogWrite(motor1pmw, vitess+30);

digitalWrite(motor21, LOW);
digitalWrite(motor22, HIGH);
analogWrite(motor2pmw, vitess+30);
}

void stop(){
  digitalWrite(motor11, LOW);
  digitalWrite(motor12, LOW);
  digitalWrite(motor21, LOW);
  digitalWrite(motor22, LOW);
}

void tourner_droite() {
digitalWrite(motor11, HIGH);
digitalWrite(motor12, LOW);
analogWrite(motor1pmw, vitess+30);

digitalWrite(motor21, HIGH);
digitalWrite(motor22, LOW);
analogWrite(motor2pmw, vitess+30);
}



int calculer_distance() {
  digitalWrite(trig, LOW);
  delay(2);
  digitalWrite(trig, HIGH);
  delay(10);
  digitalWrite(trig, LOW);
  long temps = pulseIn(echo, HIGH);
  int cm = temps / 29 / 2;
  Serial.println(cm);
  return cm;
}






void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(motor11, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(motor1pmw, OUTPUT);


  pinMode(motor21, OUTPUT);
  pinMode(motor22, OUTPUT);
  pinMode(motor2pmw, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}


void loop() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          
          if (currentLine.length() == 0) {
  
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();


            client.print("<!DOCTYPE html><html><head><title>Controle aspirateur automatique </title></head><style>body{display: flex; justify-content: center; align-items: center; flex-direction: column; height: 100vh;}button{width: 230px; height: 95px; border-radius: 5px; color: white; background-color: rgb(97, 97, 209); padding: 6px; font-weight: 700; font-size: 30px; border: none; margin: 10px; cursor: pointer;}</style><body> <div><button onmousedown='auto()'>Controle automatique</button><button onmousedown='man()'>Controle manuelle</button></div><button onmousedown='avancer()'>Avancer</button> <div><button onmousedown='gauche()'>Gauche</button> <button onmousedown='stop()'>Stop</button> <button onmousedown='droite()'>Droite</button></div><button onmousedown='reculer()'>Reculer</button> <script>function avancer(){fetch('/A')}function gauche(){fetch('/G')}function droite(){fetch('/D')}function reculer(){fetch('/R')}function stop(){fetch('/S')}function auto(){fetch('/T')}function man(){fetch('/M')}</script></body></html>");


          
            client.println();
         
            break;
          }
          else {     
            currentLine = "";
          }
        }
        else if (c != '\r') {   
          currentLine += c;      
        }

       
        


       

       
      
     
        
        if (currentLine.endsWith("GET /A")) {
          avancer();            
        }
        if (currentLine.endsWith("GET /G")) {
          tourner_gauche() ;  
          delay(490);
          stop();         
        }
        if (currentLine.endsWith("GET /D")) {
          tourner_droite(); 
          delay(490);
          stop();             
        }
        if (currentLine.endsWith("GET /R")) {
          reculer() ;               
        }
        if (currentLine.endsWith("GET /S")) {
          stop();             
        }



        if (currentLine.endsWith("GET /T")) {
          autom = 1;
        }
        if (currentLine.endsWith("GET /M")) {
          autom=0;    
          stop();      
        }




    }
  }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

 if(autom==1){
         
          int distance = calculer_distance();
          if(distance <= 30) {
            stop();
            delay(500);
            tourner_gauche();
            delay(400);
          } else {
            avancer();
          }
 }  
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
