/* 
 *
 * Time library set through serial port messages.
 * Messages consist of the letter T followed by ten digit time (as seconds since Jan 1 1970)
 * you can send the text on the next line using Serial Monitor to set the clock to noon Jan 1 2013
 * T1357041600  
 *
 * First iteration of Arcadian-Kidney RoboGarden project.
 *
 * 1. Set arduino time via serial port
 * 2. Maintain set time with use of battery backup
 * 2. Turn on or off a relay based on the time
 *
 */ 
 
#include <Time.h>  

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


int relay2 = 2;
bool relayState = false;
const int hourOn = 8;
const int hourOff = 20;


void setup()  {
  pinMode(13, OUTPUT); //initilize led for data
  pinMode(relay2, OUTPUT); //initialize relay pin
  turnOffRelay(); //set relay to HIGH/off initially
  Serial.begin(9600);
  //while (!Serial) ; // Needed for Leonardo only
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
}

void loop(){    
  if (Serial.available()) {
    processSyncMessage();
  }
 // if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  //}
  if (timeStatus() == timeSet) {
  digitalWrite(13, HIGH); // LED on if synced
  } else {
    digitalWrite(13, LOW);  // LED off if needs refresh
  }
  delay(1000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); //prints a line return
 
if(hour()>=hourOn && hour()<hourOff && relayState == false){
turnOnRelay();
}else if (hour()>=hourOff && relayState == true){
  turnOffRelay();
  }else if (hour()<hourOn && relayState == true){
  turnOffRelay();}
  



  
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
        }
    }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

void turnOnRelay() {
   digitalWrite(relay2, LOW);
    Serial.print("Turning Relay Pin2 on/LOW");
    Serial.println(); //prints a line return
    relayState = true;
}
  
void turnOffRelay() {
   digitalWrite(relay2, HIGH);
    Serial.print("Turning Relay Pin2 off/HIGH");
    Serial.println(); //prints a line return
    relayState = false;
}


