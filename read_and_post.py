import requests
import json
import time
from time import gmtime, strftime

import serial
ser = serial.Serial('/dev/tty.usbmodem1441', 9600)

# the id of the sensor
SENSORID = 'BBB987'

# stuff for RestDB.io
headers = {'Content-Type': 'application/json'}
payload = {'apikey': 'YOURKEY'}

url = "https://fbb-demos.restdb.io/rest/apiv1"
location = [5.329214,60.383332]

while True:
    try:
        s = ser.readline().strip()
        (dust, temp, hum) = s.split(",")
        print dust, temp, hum


        data = {'type': 'temp',
                'value': temp ,
                'sensorid': SENSORID,
                'loc':{'type': 'Point', 'coordinates': location }}
        r = requests.post(url, params=payload, headers=headers, data=json.dumps(data))
        # print r.text

        data = {'type': 'humid',
                'value': hum ,
                'sensorid': SENSORID,
                'loc':{'type': 'Point', 'coordinates': location }}
        r = requests.post(url, params=payload, headers=headers, data=json.dumps(data))

        data = {'type': 'part',
                'value': dust ,
                'sensorid': SENSORID,
                'loc':{'type': 'Point', 'coordinates': location }}
        r = requests.post(url, params=payload, headers=headers, data=json.dumps(data))

    except:
        exit()
        print "Failed to read or post"





