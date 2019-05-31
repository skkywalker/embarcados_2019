print("importing libs...")
import socket
import network
import time
import machine

print("setting up pins...")
pin_servo = machine.Pin(5)
pin_button = machine.Pin(16, machine.Pin.IN)
red = machine.Pin(4, machine.Pin.OUT)
red.value(1)
green = machine.Pin(0, machine.Pin.OUT)
green.value(0)

servo = machine.PWM(pin_servo, freq=50)
servo.duty(40)

print("setting up network...")
sta_if = network.WLAN(network.STA_IF)
ap_if = network.WLAN(network.AP_IF)
ap_if.active(False)
if not sta_if.isconnected():
    sta_if.active(True)
    sta_if.connect('id', 'password')
    while not sta_if.isconnected():
        pass

addr_info = socket.getaddrinfo("192.168.1.198", 3945)
addr = addr_info[0][-1]
    

print("defininf functions...")
def getFaces(addr):
	for i in range(5):
		s = socket.socket()
		s.connect(addr)
		data = s.recv(1)
		s.close()
		time.sleep_ms(20)
	value = str(data, 'utf8')
	print(value)
	return value

print("entering main loop...")
while True:
	green.value(1)
	red.value(0)
	if (pin_button.value()):
		green.value(0)
		red.value(1)
		if int(getFaces(addr)):
			servo.duty(110)
			time.sleep_ms(2000)
			servo.duty(40)
			time.sleep_ms(500)
		else:
			servo.duty(60)
			time.sleep_ms(200)
			servo.duty(40)
			time.sleep_ms(200)
			servo.duty(60)
			time.sleep_ms(200)
			servo.duty(40)
			time.sleep_ms(500)
