'''
Ryan Reschak
Uses I2C to send a image taken integer to the Arduino
Ardunio spits back wheel position
Make sure to check the connections.
'''
import serial
import smbus
import time
import board
import busio
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd
#place holder for what doyle has
import miniCamera as camera

# for RPI version 1, use “bus = smbus.SMBus(0)”
bus = smbus.SMBus(1)
ser = serial.Serial("/dev/ttyACM0", 115200, timeout=1, writeTimeout = 3)
ser.flush()

# This is the address we setup in the Arduino Program
address = 0x04
#offset = 1
# Initialise I2C bus.
i2c = busio.I2C(board.SCL, board.SDA)
# Modify this if you have a different sized Character LCD
lcd_columns = 16
lcd_rows = 2
# Initialise the LCD class
lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_columns, lcd_rows)
#camera.balanceExposure()

def writeNumber(value):
    try:
        val = bytes(str(value) + '\n', 'utf-8')
        
        ser.write(val)
    except IOError:
        print("Can't Write to Arduino")
        
        return "err_w"

def ReadfromArduino():
    #print(ser.in_waiting)
    while (ser.inWaiting() > 0):
        try:
            line = ser.readline().decode("utf-8").rstrip()
            #print("serial output : ", line)
            
            return line
    
        except:
            print("Communication Error")
           

def print_LCD(set_pos, real_pos):
    try:
        lcd.clear()
        #if statement needed for set pos and real pos
        lcd.message = "Set point:" + print_set(str(set_pos)) + "\n" + "Real point:" + str(real_pos)
        #print("Set point:" + str(set_pos) + "\n" + "Real point:" + str(real_pos))
    except IOError:
        print("Can't write to LCD\n")

def print_set(set_pos):
    if (set_pos == "1"):
      return "0"
    elif (set_pos == "2"):
      return "PI/2"
    elif (set_pos == "3"):
      return "PI"
    elif (set_pos == "4") :
      return "3*PI/2"
    return set_pos
    

lcd.clear()
# Set LCD color to red
lcd.color = [100, 0, 0]
#prev_pos = None
set_pos = 0;

ser.flush()

time_to_flush = 0
prev_pos = 0
while True:
        set_pos = camera.markerDetection()
        
        #writes to the arduino where it needs to go
        if set_pos is not None:
            prev_pos = set_pos
            writeNumber(set_pos)
        else:
            set_pos = prev_pos
            writeNumber(prev_pos)
        
        # gets the actual position of the wheel from the encoder
        real_pos = ReadfromArduino()
        #time.sleep(1)
        
       
        #prints it all to the LCD screen
        print_LCD(set_pos, real_pos)

