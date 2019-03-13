import serial as s
import time
from datetime  import datetime  as dt
import RPi.GPIO as g

isValid=False

g.setmode(g.BOARD)
g.setup(40,g.OUT)

gsm=s.Serial("/dev/serial0",9600,timeout=0.2)
gps=s.Serial("/dev/ttyUSB0",9600,timeout=0.2)

url="avrt-98d63.firebaseio.com/myjson.json"

g.output(40,True)
time.sleep(0.2)
g.output(40,False)

def expect(ret):

	found=False
	while found==False:
		x=gsm.readline()
		print x
		if ret in x:
			break
			found=True

def gsm_init():
	gsm.write('ATE0'+'\n')
	expect("OK")
	
	gsm.write('AT+SAPBR=3,1,"Contype","GPRS"'+'\n')
	expect("OK")

	gsm.write('AT+SAPBR=3,1,"APN","www"'+'\n')
	expect("OK")


	gsm.write('AT+SAPBR=1,1'+'\n')
	expect("OK")

	gsm.write('AT+SAPBR=2,1'+'\n')
	expect("SAPBR")

def http_init():

	gsm.write('AT+HTTPINIT'+'\n')
	expect("OK")
	
	gsm.write('AT+HTTPSSL=1'+'\n')
	expect("OK")

	gsm.write('AT+HTTPPARA="CID",1'+'\n')
	expect("OK")

	gsm.write('AT+HTTPPARA="URL",\"avrt-98d63.firebaseio.com/.json\"'+'\n')
	expect("OK")

gsm_init()
http_init()
time.sleep(6)
rec=""

while True:
	
	prevtime=time.time()

	gps.reset_input_buffer()
	while isValid==False:

		data=gps.readline()
		rec=data.split(",")

		print data

		if rec[0]=="$GPRMC" and rec[2]=="A":
			latitude=rec[3][0:]
	        	longitude=rec[5][1:]
	        	speed=rec[7]

        		latitude=float(latitude)/100
                	longitude=float(longitude)/100
                	speed=float(speed)*1.85				#converting into km/hr

			isValid==True

	        	print "latitude is",latitude
			myjson="{\"lat\":"+str(latitude)+",\"lon\":"+str(longitude)+"}"
			break

	gsm.write('AT+HTTPDATA='+str(len(myjson))+",10000"+'\n')
	expect("DOWNLOAD")

	gsm.write(myjson+"\n")
	expect("OK")

	gsm.write('AT+HTTPACTION=1'+'\n')
	expect("ACTION")

	g.output(40,True)
	time.sleep(0.2)
	g.output(40,False)

	gsm.write('AT+HTTPREAD'+'\n')
	expect("READ")

	isValid==False
	data="hello"

	print "\t\t\t\t\t\t",time.time()-prevtime
