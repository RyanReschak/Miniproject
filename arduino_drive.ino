

/*Ryan Reschak and Erik Salazar and Mia Wehrlen
 * Make sure the Arduino is connected to the RPi in the A4 and A5 pins
 * Velocity left and Right of Wheels
 * calculate the velocities of the left and right wheel
 * Sends that number back
 */
#define SLAVE_ADDRESS 0x04
#define PI 3.1415926535897932384626433832795
#define MV1 9
#define VS1 7
#define VS2 8
#define pin4 4
#define pin12 12
#define outPin 9
//These are all motor controller values
int a = 2;
int b = 6;  //initialize pins
int time;
int PWMOutput = 0;
int lastState = 0;
int counter1 = 0; //keeps track of position of encoder
int counter2 = 0;
float AngularVelocity1 = 0;  //initialize all variables
float StartTime1 = 0;
float EndTime1 = 0;
float StartLoop = 0;
float EndLoop = 0;
float AngularPosition1 = 0;

const float Kp = 0.2274788; 
const float K = 6.4;
const float Ki = 0.0517371; 

float I = 0;
float e_past = 0;
float Ts = 0;
float Tc;
float currPos=0;
float desPos = 0; // This should be set as whatever the desired position for the wheel is 
float error = 0;
float error_past = 0;

//These are all value sent and recieved values (i2c)
String number = "";
float setPosition = PI;
bool stringComplete = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  // set pinmodes for digital inputs to have pullup resistor:
  pinMode(a, INPUT_PULLUP);
  digitalWrite(a, HIGH);
  pinMode(b, INPUT_PULLUP);
  digitalWrite(b, HIGH);
  // initialize serial communication at 9600 bits per second:
  pinMode (MV1,OUTPUT);// put your setup code here, to run once:
  pinMode (VS1,OUTPUT);
  //pinMode (VS2,OUTPUT);
  digitalWrite (VS1, LOW);
  //digitalWrite (VS2, LOW);
  pinMode (pin4,OUTPUT);
  pinMode (pin12,INPUT);
  digitalWrite(pin4,HIGH);
  time = 0;
  StartTime1 = micros();
  //these are too keep track of the time in the loop and interrupt
  StartLoop = micros();
  Tc =micros();
  attachInterrupt(digitalPinToInterrupt (a),interruptEncoder1, RISING);
  //initialize the interrupts and sets the flag

  Serial.begin(115200);//start serial for output

  //important for saving data
  number.reserve(2);
  //Serial.println("Ready!");
}

void loop() {
  
  if (stringComplete) {
    if (number == "1") {
      setPosition = 2*PI;
    } else if (number == "2") {
      setPosition = PI/2;
    } else if (number == "3") {
      setPosition = PI;
    } else if (number == "4") {
      setPosition = 3*PI/2;
    }
    sendData();
    stringComplete = false;
    number = "";
    I = 0;
    
  }
 
  PIcontroller();

  time = millis();
  
  StartLoop = EndLoop; // set the beginning time to current time
  EndLoop = micros(); // initialize the end time
  
}

void serialEvent(){
  
  while(Serial.available() > 0){
    char s = Serial.read();
    
    if (s != '\n') {
    number += s;
    } else {
      stringComplete = true;
      break;
    }
  }
 
}

void sendData() {
  //can't send floating point number
  //converts into degree for easier translation over i2c
  
  Serial.println(-1*AngularPosition1);
}

void interruptEncoder1 (){ //interrupt
  // read the input pin:
  if (digitalRead(a) == digitalRead(b)){ //checks to see if A channel is equal to Chanel B
    EndTime1 = micros();  // end time for interrupt1 to calculate angular velocity
    counter1 = counter1 - 2;
    if (counter1 == -1600){
      counter1 = 0;
    }
    if(counter1==1600){
      counter1 = 0;
    }
    AngularPosition1 = (counter1*2*PI)/1600;
   
  }else{
    EndTime1 = micros();
    counter1 = counter1 + 2;
    if (counter1 == 1600){
      counter1 = 0;
    }
    AngularPosition1 = (counter1*2*PI)/1600;
    
  }
  StartTime1 = EndTime1; //reset the start time to current time
}

void PIcontroller() {
  // Put this code in a loop within the main code, probably only active when the wheel is not independently moving
  // previous to the loop below, you should set desPos to the position that you wish to maintain
  // Make sure that AngularPosition is updated before this point
  currPos = -1*AngularPosition1;//whatever you used for the angular position
  //currPos=AngularPosition1;
  
  error=setPosition-currPos;
  
  
  I=I+(Ts/1000)*error;
  
  PWMOutput = error*(Kp+Ki*I);
  

  if(abs(PWMOutput)>255){
    PWMOutput=constrain(PWMOutput,-1,1)*255;
    error = constrain(error,-1,1)*min(255/Kp,abs(error));
  }
 
 
  digitalWrite (VS1, HIGH);
  PWMOutput = abs(PWMOutput);

  analogWrite(outPin, PWMOutput);
  Ts=micros()-Tc;
  Tc=micros();

  
}
