# Miniproject
This is the git repository for the SEED Miniproject
main.py - handles the interfacing and interaction between the Camera, LCD, and Arduino side (the motor driver and encoder)
arduino_drive.ino - ardunio script that handles that is given an integer value over serial, changes the position of the wheel
                    using PI control and by reading the encoder value, sends encoder back to PI
miniCamera.py - takes a picture and gives what quadrant the Aruco Marker is in.
