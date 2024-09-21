//Control syntax: #1on.
#include<SoftwareSerial.h>
#define btn0         A0 // Positive low
#define btn1         A1 // Positive low
#define btn2         A2 // Positive low
#define btn3         A3 // Positive low
#define btn4         A4 //opening skylight
#define btn5         A5 //closing skylight
#define mi1          2 //analog
#define mi2          3 //digital
#define ht1          4 //LOW if the skylight is fully open
#define ht2          5 //LOW if the skylight is fully close
#define relay4       6 //device 1
#define relay3       7 //device 2
#define relay2       8 //device 3
#define relay1       9 //device 4
SoftwareSerial mySerial(10,11); // TX, RX
#define rainmodule   12 //Positive low



int answer;
int xbuff=0;
int sms_mode = 0;
const int buffer_size = 160;       // Buffer size you can adjust this size  
char c=0;
char c1=0;
char aux_string[30];
char buffer[buffer_size]; 
char command[30];
char *ptr1=0;
char phonenumber[]="+84935014779";       // Your phone phonenumber0868211855
char r1_on[] = "1on";              
char r1_of[] = "1off";              
char r2_on[] = "2on";              
char r2_of[] = "2off";              
char r3_on[] = "3on";              
char r3_of[] = "3off";              
char r4_on[] = "4on";              
char r4_of[] = "4off"; 
bool Rainstas = false; // variable to remember if rain notification has been displayed
unsigned long previousMillis = 0; // variable to remember the last time we checked
const long interval = 100; // check every 0.1 second   


//******************************************************************************************************
void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(relay4,OUTPUT);
  pinMode(btn0,INPUT);
  pinMode(btn1,INPUT);
  pinMode(btn2,INPUT);
  pinMode(btn3,INPUT);
  
  pinMode(mi2,OUTPUT);
  pinMode(mi1,OUTPUT);
  pinMode(rainmodule,INPUT);
  pinMode(ht1,INPUT);
  pinMode(ht2,INPUT);
  pinMode(btn4,INPUT);
  pinMode(btn5,INPUT);

  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,HIGH);
  digitalWrite(relay4,HIGH);
  digitalWrite(mi2,LOW);
  digitalWrite(mi1,LOW); 
  Connect_sim800l();  
  Init_sim800l();
  Erase_buffer();
}
//******************************************************************************************************
void loop() {
  Receive_uart();
  Control_devide_sms();
  Control_devide_btn();
  Control_skylight();
}
//******************************************************************************************************
void Receive_uart() { 
  while(mySerial.available()>0) {
    c = mySerial.read();
      if(c=='#'){
        c1=c;
        xbuff=0;
    }
    if(c=='.' && c1=='#'){
      sms_mode=true;
    }
    if (xbuff >= buffer_size){
      xbuff=0;
    }
    else{ 
      buffer[xbuff++]=c;
    }
  }
}
//******************************************************************************************************
void Control_devide_btn(){
  if(digitalRead(btn0) == LOW) {
    digitalWrite(relay1, !digitalRead(relay1));
    String stas = "OFF";
    if(!digitalRead(relay1)){ stas = "ON"; }
    send_SMS("relay 1 : " + stas );
    while(digitalRead(btn0) == LOW);
  }
  if(digitalRead(btn1) == LOW) {
    digitalWrite(relay2, !digitalRead(relay2)); 
    String stas = "OFF";
    if(!digitalRead(relay2)){ stas = "ON"; }
    send_SMS("relay 2 : " + stas );
    while(digitalRead(btn1) == LOW);
  }
  if(digitalRead(btn2) == LOW) {
    digitalWrite(relay3, !digitalRead(relay3)); 
    String stas = "OFF";
    if(!digitalRead(relay3)){ stas = "ON"; }
    send_SMS("relay 3 : " + stas );
    while(digitalRead(btn2) == LOW);
  }
  if(digitalRead(btn3) == LOW) {
    digitalWrite(relay4, !digitalRead(relay4)); 
    String stas = "OFF";
    if(!digitalRead(relay4)){ stas = "ON"; }
    send_SMS("relay 4 : " + stas );
    while(digitalRead(btn3) == LOW);
  }
}
//******************************************************************************************************
void Control_devide_sms(){
  if (sms_mode == true){
    ptr1=strstr(buffer,r1_on);                          // Checking for turn on device 1   
    if (strncmp(ptr1,r1_on,3)==0){           
      digitalWrite(relay1,LOW);
      send_SMS(r1_on);
    } 
    ptr1=strstr(buffer,r1_of);      
    if (strncmp(ptr1,r1_of,3)==0){           
      digitalWrite(relay1,HIGH);
      send_SMS(r1_of);
    }
    ptr1=strstr(buffer,r2_on);
    if (strncmp(ptr1,r2_on,3)==0){           
      digitalWrite(relay2,LOW);         
      send_SMS(r2_on);
    } 
    ptr1=strstr(buffer,r2_of);
    if (strncmp(ptr1,r2_of,3)==0){
      digitalWrite(relay2,HIGH);          
      send_SMS(r2_of);
    } 
    ptr1=strstr(buffer,r3_on);
    if (strncmp(ptr1,r3_on,3)==0){
      digitalWrite(relay3,LOW);        
      send_SMS(r3_on);
    } 
    ptr1=strstr(buffer,r3_of);
    if (strncmp(ptr1,r3_of,3)==0){
      digitalWrite(relay3,HIGH);         
      send_SMS(r3_of);
    } 
    ptr1=strstr(buffer,r4_on);
    if (strncmp(ptr1,r4_on,3)==0){ 
      digitalWrite(relay4,LOW);        
      send_SMS(r4_on);
    } 
    ptr1=strstr(buffer,r4_of);
    if (strncmp(ptr1,r4_of,3)==0){
      digitalWrite(relay4,HIGH);        
      send_SMS(r4_of);
    }
    Erase_buffer();
  }
}
//******************************************************************************************************
void Control_skylight() {
  bool Raining = (digitalRead(rainmodule) == LOW);
  bool ht1_stas = (digitalRead(ht1) == LOW);
  bool ht2_stas = (digitalRead(ht2) == LOW);
  bool btn4_pressed = (digitalRead(btn4) == LOW);
  bool btn5_pressed = (digitalRead(btn5) == LOW);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (Raining && !ht2_stas) {
      if (!Rainstas) {
        send_SMS("It's raining!");
        Rainstas = true;
      }
    }
    else { Rainstas = false; }
  }
  if (Raining && !ht2_stas) {
      analogWrite(mi2, 70);
      digitalWrite(mi1, LOW);
    } else if (!Raining && !ht2_stas && btn5_pressed) {
      analogWrite(mi2, 70);
      digitalWrite(mi1, LOW);
    } else if (!Raining && !ht1_stas && btn4_pressed) {
      analogWrite(mi2, 180);
      digitalWrite(mi1, HIGH);
    } else {
      analogWrite(mi2, 0);
      digitalWrite(mi1, LOW);
    }
}
//******************************************************************************************************
int sendAT(char* ATcommand, char* expected_answer, unsigned int timeout) {
  unsigned long start_time = millis();
  mySerial.println(ATcommand);
  Serial.println(ATcommand);
  while ((millis() - start_time) < timeout) {
    if (mySerial.find(expected_answer)) {
      Serial.println(expected_answer);
      return 1;
    }
  }
  Serial.println(expected_answer);
  return 0;
}

//******************************************************************************************************
void send_SMS(const String msg) {
     sprintf(command,"AT+CMGS=\"%s\"",phonenumber);
     answer = sendAT(command,"Sending",1000);
     mySerial.println(msg);    
     mySerial.write(26);// Ctrl + Z
     } 
//******************************************************************************************************
void Connect_sim800l() {
  answer = sendAT("AT", "OK", 2000);
  if (answer == 0) { 
    unsigned long start_time = millis();
    while ((millis() - start_time) < 6000) {// Wait for response with timeout
      answer = sendAT("AT", "OK", 2000);
      if (answer == 1) {
        break;
      }
    }
  }
}
//******************************************************************************************************
void Init_sim800l() {
  answer = 0;
  while (answer == 0) {
    answer = sendAT("AT+CREG?","+CREG: 0,1",500); // Checking status of Connecting to the network
  }
  answer = 0;
  while (answer == 0) {
    answer = sendAT("AT+CMGF=1", "OK",1000); // Set SMS into text mode
  } 
  answer = 0;
  while (answer == 0) {
    answer = sendAT("AT+CNMI=2,2,2,0,0", "OK",1000); 
  }
  sprintf(aux_string,"ATD%s;",phonenumber);
  answer = 0;
  while (answer == 0) {
    answer = sendAT(aux_string, "OK", 2000); 
  }
  delay(10000);
  answer = 0;
  while (answer == 0) {
    answer = sendAT("ATH", "OK",2000); 
  }
}
//******************************************************************************************************
void Erase_buffer() {
  memset(buffer, 0, buffer_size);
  memset(ptr1, 0, sizeof(ptr1));
  memset(command, 0, sizeof(command));
  xbuff = 0;
  c = 0;
  c1 = 0;
  sms_mode = false;
}
//******************************************************************************************************