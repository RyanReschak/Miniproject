#include <Wire.h>

/*Ryan Reschak
 * Make sure the Arduino is connected to the RPi in the A4 and A5 pins
 * 
 * Basically adjusts the sent number from the pi based on the offset
 * Sends that number back
 */
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

void setup() {
  // put your setup code here, to run once:
pinMode(13,OUTPUT);
Serial.begin(115200);//start serial for output
//initialize i2c as Slave
Wire.begin(SLAVE_ADDRESS);

Wire.onReceive(receiveData);
Wire.onRequest(sendData);

Serial.println("Ready!");
}

void loop() {
  // put your main code here, to run repeatedly:
delay(100);
}

void receiveData(int byteCount){
  Serial.print("data received: ");
  int offset = -1;
  while(Wire.available()){
    number = Wire.read();
    
    Serial.print(number);
    Serial.print(' ');
    //based on the offset it adjust the number to write back
    if (offset == -1) {
      offset = number;
    } else if (offset == 0){
      number += 5;
    } else if (offset == 1){
      number += 10;
    }
  
  }
  Serial.println(' ');
}

void sendData(){
  Wire.write(number);
}
