'''
Ryan Reschak
Uses I2C to send a image taken integer to the Arduino

Make sure to check the connections.
'''
import smbus
import time
import board
import busio
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd
#place holder for what doyle has
import "camera.py" as camera
# for RPI version 1, use “bus = smbus.SMBus(0)”
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04
offset = 1
# Initialise I2C bus.
i2c = busio.I2C(board.SCL, board.SDA)
# Modify this if you have a different sized Character LCD
lcd_columns = 16
lcd_rows = 2
# Initialise the LCD class
lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_columns, lcd_rows)

def writeNumber(value):
    	bus.write_byte(address, value)

def readNumber():
	#arduino will send encoder location 
    	return bus.read_byte(address)
    	

def print_LCD(set_pos, real_pos):
	lcd.clear()
	#if statement needed for set pos and real pos
	lcd.message = "Set point" + str(set_pos) + "\n" + "Real point" + str(real_pos)

lcd.clear()
while True:
    	# Set LCD color to red
    	lcd.color = [100, 0, 0]
    	# get set position from Aruco Marker
    	set_pos = camera.take_pic()
	#writes to the arduino where it needs to go
    	writeNumber(set_pos)
    	# sleep a tehnth of a second
    	time.sleep(0.1)
	# gets the actual position of the wheel from the encoder
    	real_pos = readNumber()
	#prints it all to the LCD screen
    	print_LCD(set_pos, real_pos)
 
