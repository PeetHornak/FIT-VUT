import socket
import sys
import os
import json

host = 'api.openweathermap.org'
port = 80
city = (os.environ['city'])
appid = (os.environ['api_key'])

try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as clientsocket:
        try:
            ip = socket.gethostbyname(host)
        except socket.gaierror:
            print('Hostname could not be resolved.')
            sys.exit()

        clientsocket.connect((ip, port))
        request = str.encode('GET /data/2.5/weather?q=' + city + '&APPID=' + appid + ' HTTP/1.0\r\n\r\n')
        try:
            clientsocket.sendall(request)
        except socket.error:
            print('Request could not be sent')
            sys.exit()

        recieved = clientsocket.recv(4096)
        decoded = recieved.decode('utf-8')
        decoded = decoded.split('\r\n')[-1]
        json_data = json.loads(decoded)
except socket.error:
    print('Socket could not be created')
    sys.exit()

if json_data['cod'] == 401:
    print('Invalid API key.')
elif json_data['cod'] == '404':
    print('City not found.')
elif json_data['cod'] != 200:       #Ina navratova hodnota
    print('Error')
else:
    print(json_data['name'])
    print(json_data['weather'][0]['description'])
    print("temp: {}°C".format(round(json_data['main']['temp'] - 273.15, 2)))
    print("humidity: {}%".format(json_data['main']['humidity']))
    print("preassure: {}hPa".format(round(json_data['main']['pressure'], 2)))
    print("wind-speed: {}km/h".format(round(json_data['wind']['speed'] * 3.6, 2)))
    print("wind-deg: {}".format(round(json_data['wind']['deg'], 2)))
