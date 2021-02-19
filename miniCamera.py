# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import math
from time import sleep
import numpy as np
#Arcuo Detection Imports
from cv2 import aruco
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib as mpl
import pandas as pd

name = "test.jpg"
camera = PiCamera()

def takePic():
    # allow the camera to warmup
    time.sleep(0.1)
    # grab an image from the camera

#    print("Capturing Image...")
    try:
        camera.resolution = (1920, 1080)
        camera.capture(name)
    except:
       print("Failed to capture")
       
    image = cv2.imread(name,0)
   
    try:
        cv2.imwrite(name, image)
    except:
        print("Couldn't save "+name)
        pass

#Read the image in grayscale is 3 times faster than Color


def arucoDetection():   
#Processing Aruco Images is spooky
    frame = cv2.imread(name)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    aruco_dict = aruco.Dictionary_get(aruco.DICT_6X6_250)
    parameters =  aruco.DetectorParameters_create()
    corners, ids, rejectedImgPoints = aruco.detectMarkers(gray, aruco_dict, parameters=parameters)
    frame_markers = aruco.drawDetectedMarkers(frame.copy(), corners, ids)
    plt.figure()
    plt.imshow(frame_markers)
#This is for not erroring out if there is not an image in the photo
    if ids is not None:
        for i in range(len(ids)):
            c = corners[i][0]
            plt.plot([c[:, 0].mean()], [c[:, 1].mean()], "o", label = "id={0}".format(ids[i]))
#        plt.legend()
#        plt.show()
#        print(ids)
        return corners #Need this for determining angle and distance
            
    else:
        print("No Markers Found")
    
def manyPics():
#This just loops through capturing an image every 1 second and performs detection
    while(True):
        takePic()
        print(arucoDetection())
        
def locator(corners):
    imageH = 1080
    imageW = 1920
    camCenter = [int(imageW/2), int(imageH/2)]
    objH = 95
    objW = 95
    focalL = 1841.36842105 #This is in pixels
    widthA = 0
    if corners is not None:
        
#Corners: corners[0][0][0][0] is X value for top Left Corner
# corners[0][0][1][0] is X value for top right Corner        
        
        distL = abs(imageW/2 - corners[0][0][1][0])
        distR = abs(corners[0][0][0][0] - imageW/2)
        avgX = int((corners[0][0][0][0] + corners[0][0][1][0] + corners[0][0][2][0] + corners[0][0][3][0]) /4)
        avgY = int((corners[0][0][0][1] + corners[0][0][1][1] + corners[0][0][2][1] + corners[0][0][3][1]) /4)
        markerCenter = [avgX, avgY]

        #Run 4 if statements comparing markerCenter and camCenter and return based on that#
        if (markerCenter[0] < camCenter[0]) and (markerCenter[1] < camCenter[1]):
            print(markerCenter)
            print(camCenter)
            return 1 #NorthWest
        elif (markerCenter[0] > camCenter[0]) and (markerCenter[1] < camCenter[1]):
            print(markerCenter)
            print(camCenter)
            return 2 #NorthEast
        elif (markerCenter[0] > camCenter[0]) and (markerCenter[1] > camCenter[1]):
            print(markerCenter)
            print(camCenter)
            return 3 #SouthEast
        elif (markerCenter[0] < camCenter[0]) and (markerCenter[1] > camCenter[1]):
            print(markerCenter)
            print(camCenter)
            return 4 #SouthWest

def markerDetection():    
    
    takePic()
    marker = arucoDetection()
    #marker = manyPics() #Use this for continual pics
    print(marker)
    location = locator(marker)
    print(location)
    #Uncomment for testing if need be
    #image = cv2.imread(name,0)
    #cv2.imshow("Image", image)
    #cv2.waitKey(0)
    return location
