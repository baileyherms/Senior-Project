#!/usr/bin/python
import pygame
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_UP)

pygame.mixer.init()
#Arduino Connect
pygame.mixer.music.load("Instructions.wav")

while True:
    press_button = GPIO.input(23)
    if press_button == False:
        print('Button Pressed')
        
        pygame.mixer.music.play()
        while (pygame.mixer.music.get_busy() == True):
            continue
        #time.sleep(0.2)
