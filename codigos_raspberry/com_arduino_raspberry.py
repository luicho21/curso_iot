##
# author: Luis Salgueiro
# date: 03-11-2019
# description: Receive data from arduino and print it


import serial

ser=serial.Serial("/dev/ttyACM1",9600)

while 1: ser.readline()
