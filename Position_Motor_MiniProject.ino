//Erik Salazar
//Feb 10, 2021
//Velocity left and Right of Wheels
//Description: calculate the velocities of the left and right wheel  
#define PI 3.1415926535897932384626433832795
#define MV1 9
#define VS1 7
#define pin4 4
#define pin12 12
int a = 2;
int b = 6;  //initialize pins
int time;
int pwmOutput;
int lastState = 0;
int counter1 = 0; //keeps track of position of encoder
int counter2 = 0;
float AngularVelocity1 = 0;  //initialize all variables
float StartTime1 = 0;
float EndTime1 = 0;

float StartLoop = 0;
float EndLoop = 0;
float AngularPosition1 = 0;
void setup() {
  
  // set pinmodes for digital inputs to have pullup resistor:
  pinMode(a, INPUT_PULLUP);
  digitalWrite(a, HIGH);
  pinMode(b, INPUT_PULLUP);
  digitalWrite(b, HIGH);
  // initialize serial communication at 9600 bits per second:
  pinMode (MV1,OUTPUT);// put your setup code here, to run once:
  pinMode (VS1,OUTPUT);
  pinMode (pin4,OUTPUT);
  pinMode (pin12,INPUT);
  digitalWrite(pin4,HIGH);
  Serial.begin(9600);
  time = 0;
  StartTime1 = micros();
  //these are too keep track of the time in the loop and interrupt
  StartLoop = micros();
  attachInterrupt(digitalPinToInterrupt (a),interruptEncoder1, RISING);
  //initialize the interrupts and sets the flag
  
}

// the loop routine runs over and over again forever:
void loop() {
  if (time == 0){
    pwmOutput = 0;
  }
  else{
    pwmOutput = 255;
  }
  analogWrite(9,pwmOutput);
  if (time < 2000){
     Serial.print((StartLoop/1000000),5); //print time
     Serial.print("\t"); //print tab
     Serial.print(AngularPosition1);
     Serial.print("\t");
     //equation for the velocity of the left wheel
     Serial.println(AngularVelocity1,5);
  }
  time = millis();
  while (time!=lastState +1){
    lastState++;
  }
  AngularVelocity1 = 0;
  //resets the velocity to zero because the car should not be moving 
  //delay(20); // delay between reads
  StartLoop = EndLoop; // set the beginning time to current time
  EndLoop = micros(); // initialize the end time
  }
void interruptEncoder1 (){ //interrupt
  // read the input pin:
  if (digitalRead(a) == digitalRead(b)){ //checks to see if A channel is equal to Chanel B
    EndTime1 = micros();  // end time for interrupt1 to calculate angular velocity
    counter1 = counter1 - 2;
    if (counter1 == -3200){
      counter1 = 0;
    }
    AngularPosition1 = (counter1*2*PI)/3200;
    AngularVelocity1 = (-2*2*PI/3200)/((EndTime1 - StartTime1)/1000000);
    //Serial.println(AngularVelocity1*r,5);
  }else{
    EndTime1 = micros();
    counter1 = counter1 + 2;
    if (counter1 == 3200){
      counter1 = 0;
    }
    AngularPosition1 = (counter1*2*PI)/3200;
    AngularVelocity1 = (2*2*PI/3200)/((EndTime1 - StartTime1)/1000000);
  }
  StartTime1 = EndTime1; //reset the start time to current time
}
