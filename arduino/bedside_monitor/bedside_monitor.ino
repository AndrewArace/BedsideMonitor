#include <SoftwareSerial.h>
#include <SmartThings.h>

#define PIN_THING_RX    3
#define PIN_THING_TX    2

SmartThingsCallout_t messageCallout;    // call out function forward decalaration
SmartThings smartthing(PIN_THING_RX, PIN_THING_TX, messageCallout);  // constructor

const int ledPin = 13;

const int ALERT_PIN =  12;
const int ALERT_REPEAT = 5;

bool isDebugEnabled;    // enable or disable debug in this example

//front door motion
bool hasFrontDoorMotion;

//side porch motion
bool hasSidePorchMotion;


void setup() {
  // setup default state of global variables
  isDebugEnabled = true;
  hasFrontDoorMotion = false;
  hasSidePorchMotion = false;
  
  // setup hardware pins 
  pinMode(ledPin, OUTPUT);     // define PIN_LED as an output
  digitalWrite(ledPin, LOW);   // set value to LOW (off) to match stateLED=0

  pinMode(ALERT_PIN, OUTPUT);

  if (isDebugEnabled) {
    Serial.begin(9600);
    Serial.println("setup.."); 
    runTests();
  }
}

void runTests() {
/*
  Serial.println("alertTone(front)"); 
  alertTone("front");
  delay(1000);
  
  Serial.println("alertTone(side)"); 
  alertTone("side");
  delay(1000);
*/
  Serial.println("setMotion(front, true)"); 
  setMotion("front", true);
  delay(1000);  
  
  Serial.println("setMotion(front, false)"); 
  setMotion("front", false);
  delay(1000); 
  
  Serial.println("setMotion(side, true)"); 
  setMotion("side", true);
  delay(1000);  
  
  Serial.println("setMotion(side, false)"); 
  setMotion("side", false);
  delay(1000); 
}

void loop() {
  // run smartthing logic
  smartthing.run();
}


void messageCallout(String message) {
  // if debug is enabled print out the received message
  if (isDebugEnabled) {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

  if (message.equals("frontDoorMotion.active")) {
    setMotion("front", true);    
  }
  else if (message.equals("frontDoorMotion.inactive")) {
    setMotion("front", false);
  }
  else if (message.equals("sidePorchMotion.active")) {
    setMotion("side", true);
  }
  else if (message.equals("sidePorchMotion.inactive")) {
    setMotion("side", false);
  }
}

void setMotion(String area, bool active) {
  if(area.equals("front")) {
    if(!active) {
      hasFrontDoorMotion = false;
      smartthing.shieldSetLED(0, 0, 0);
    }
    else if(active && !hasFrontDoorMotion) {
      //was just activated for the first time
      smartthing.shieldSetLED(0, 1, 0);
      alertTone("front");
    }
    else {
      //still active
    }
  }
  else if(area.equals("side")) {
    if(!active) {
      hasSidePorchMotion = false;
      smartthing.shieldSetLED(0, 0, 0);
    }
    else if(active && !hasSidePorchMotion) {
      //was just activated for the first time
      smartthing.shieldSetLED(1, 0, 0);
      alertTone("side");
    }
    else {
      //still active
    }
  }
}


void alertTone(String area) {
  if(area.equals("front")) {
    for(int i = 0; i < ALERT_REPEAT; i++) {
      tone(ALERT_PIN, 1245);  
      delay(300);
      noTone(ALERT_PIN);
      delay(300);
    }
  }
  else if(area.equals("side")) {
    for(int i = 0; i < ALERT_REPEAT; i++) {
      tone(ALERT_PIN, 988);  
      delay(300);  
      noTone(ALERT_PIN);
      delay(300);
    }
  }

  noTone(ALERT_PIN);
}


/*
void on() {
  stateLED = 1;                 // save state as 1 (on)
  digitalWrite(ledPin, HIGH);  // turn LED on
  smartthing.shieldSetLED(0, 0, 1);
  smartthing.send("on");        // send message to cloud
  Serial.println("on");
}

void off() {
  stateLED = 0;                 // set state to 0 (off)
  digitalWrite(ledPin, LOW);   // turn LED off
  smartthing.shieldSetLED(0, 0, 0);
  smartthing.send("off");       // send message to cloud
  Serial.println("off");
}

void hello() {
  Serial.println("brobasaur");
  smartthing.send("colors!");
  smartthing.shieldSetLED(1, 0, 0);
  delay(200);
  smartthing.shieldSetLED(0, 1, 0);
  delay(200);
  smartthing.shieldSetLED(0, 0, 1);
  delay(200);
  smartthing.shieldSetLED(1, 1, 0);
  delay(200);
  smartthing.shieldSetLED(1, 1, 1);
  delay(200);
  smartthing.shieldSetLED(1, 0, 1);
  delay(200);
  smartthing.shieldSetLED(0, 1, 1);
  delay(200);
  smartthing.shieldSetLED(3, 2, 1);
  delay(200);
  smartthing.shieldSetLED(1, 2, 3);
  delay(200);
  smartthing.shieldSetLED(2, 2, 4);
  delay(200);
  smartthing.shieldSetLED(4, 3, 1);
  delay(200);
  smartthing.shieldSetLED(0, 0, 0);
  smartthing.send("fancy");
}
*/

