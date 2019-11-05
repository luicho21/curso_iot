## 
# Author: Luis Salgueiro
# date: 03-11-2019
# description: Send command to arduino by serial com


import serial

ser = serial.Serial("/dev/ttyACM1",9600)
print("INICIO....")

while True:
	comando=input("Input command ON-OFF: ")
	ser.write(comando.encode())
	if comando=="ON":
		print("LED ON")
	elif comando=="OFF":
		print("LED OFF")
	else:
		print("Wrong command")


ser.close()
