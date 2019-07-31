import socket
import sys
import os
import json

host = 'api.openweathermap.org'
port = 80
city = None
appid = None
json_data = None

try:
    city = (os.environ['city'])
    appid = (os.environ['api_key'])
except KeyError:
    print('Wrong parameters')
    sys.exit()

try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as clientsocket:
        try:
            ip = socket.gethostbyname(host)
        except socket.gaierror:
            print('Hostname could not be resolved.')
            sys.exit()

        clientsocket.connect((ip, port))
        request = str.encode('GET /data/2.5/weather?q=' + city + '&APPID=' + appid + '&units=metric' + ' HTTP/1.0\r\n\r\n')
        try:
            clientsocket.sendall(request)
        except socket.error:
            print('Request could not be sent')
            sys.exit()

        while True:
            received = clientsocket.recv(1024)
            if not received:
                break
            decoded = received.decode('utf-8')
            decoded = decoded.split('\r\n')[-1]
            try:
                json_data = json.loads(decoded)
            except json.JSONDecodeError:
                print('Could not parse json')
                exit()

except socket.error:
    print('Socket could not be created')
    sys.exit()

if json_data['cod'] == 401:
    print('Invalid API key.')
elif json_data['cod'] == '404':
    print('City not found.')
elif json_data['cod'] != 200:       # Ina navratova hodnota
    print('Error from server')
else:
    if 'name' in json_data:
        print(json_data['name'])
    else:
        print('not found')

    if 'weather' in json_data:
        if 'description' in json_data['weather'][0]:
            print(json_data['weather'][0]['description'])
    else:
        print('not found')

    if 'main' in json_data:
        if 'temp' in json_data['main']:
            print("temp: {}°C".format(round(json_data['main']['temp'], 2)))
        else:
            print('temp: not found')
        if 'humidity' in json_data['main']:
            print("humidity: {}%".format(json_data['main']['humidity']))
        else:
            print('humidity: not found')
        if 'pressure' in json_data['main']:
            print("pressure: {}hPa".format(round(json_data['main']['pressure'], 2)))
        else:
            print('pressure: not found')

    if 'wind' in json_data:
        if 'speed' in json_data['wind']:
            print("wind-speed: {}km/h".format(round(json_data['wind']['speed'] * 3.6, 2)))
        else:
            print('wind-speed: not found')
        if 'deg' in json_data['wind']:
            print("wind-deg: {}".format(round(json_data['wind']['deg'], 2)))
        else:
            print('wind-deg: not found')
