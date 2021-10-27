/*
 * Complete Project Details http://randomnerdtutorials.com
 */

// Include Software Serial library to communicate with GSM
#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(7, 8);
int data,data2;
// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "START";
String listrikState = "HIDUP";

// Relay connected to pin 12
const int relay  = 2;
const int sensor = 3;
const int sensor2 = 5;
int kirimpesan;
void setup() {
  // Automatically turn on the shield
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
  
  // Set relay as OUTPUT
  pinMode(relay, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(sensor2, INPUT);
  // By default the relay is off

    digitalWrite(relay, HIGH);

    
  // Initializing serial commmunication
  Serial.begin(19200); 
  SIM900.begin(19200);

  // Give time to your GSM shield log on to network
  delay(20000);
  Serial.print("SIM900 ready...");

  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}

void loop(){
  if(SIM900.available()>0){
    textMessage = SIM900.readString();
    Serial.print(textMessage);    
    delay(10);
  } 
  data=0;
  data2=0;
  for(int i=0;i<10;i++){
    delay(200);
    data +=digitalRead(sensor);
    data2 +=digitalRead(sensor2);
    }
    Serial.println(data);
    Serial.println(data2);
    //Serial.println(kirimpesan);
    Serial.println("");
  if(textMessage.indexOf("START")>=0){
    // Turn on relay and save current state
    delay(5000);
    digitalWrite(relay, LOW);
    delay(5000);
    digitalWrite(relay, HIGH);
    lampState = "hidup";
   // Serial.println("LAMPU AKTIF");  
    textMessage = "";  
  }
  if(data>=1){
    delay(200);
    kirimpesan=0;
    }
    else if(data==0&&kirimpesan==1){delay(200);}
    else if(data==0&&kirimpesan==0){
      String message = "Mati Lampu ";
    sendSMS(message);
    
    textMessage = "";
        Serial.println("LAMPU SEDANG MATI");
        
    kirimpesan=1;
    delay(5000);
      }
    
if(data2>=1){
  lampState="";
  lampState="RUNNING";}
  else{
    lampState="";
  lampState="SHUTDOWN";
    }
if(data>=1){
  listrikState="";
  listrikState="Nyala";}
  else{
    listrikState="";
  listrikState="Mati";
    }

  if(textMessage.indexOf("STATUS")>=0){
    String message = "Engine dalam keadaan " + lampState;
    sendSMS(message);
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
  if(textMessage.indexOf("LISTRIK")>=0){
    String message = "Listrik dalam keadaan " + listrikState;
    sendSMS(message);
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
}  

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"+6282398282514\""); 
  delay(100);
  // Send the SMS
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}
