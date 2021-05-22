from flask import Flask, jsonify, render_template, request, Response
from flask_mqtt import Mqtt
from datetime import datetime
import webbrowser
import time
import json
import logging

temperature = 0
heart_rate = 0
gas = 0

app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = 'AWS_IP_adress' # replace your AWS IP_adress
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = 'mqtt_user_name' # replace your mqtt user_name
app.config['MQTT_PASSWORD'] = 'mqtt_Passwd' # replace your mqtt Passwd
app.config['MQTT_REFRESH_TIME'] = 1.0
mqtt = Mqtt(app)

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('esp32/temperature')
    mqtt.subscribe('esp32/heart_rate')
    mqtt.subscribe('esp32/gas')

@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):

    global temperature, heart_rate, gas

    topic = message.topic
    if topic == 'esp32/temperature':
        temp = message.payload.decode()
        temperature = json.loads(temp)

    if topic == 'esp32/heart_rate':
        hrt = message.payload.decode()
        heart_rate = json.loads(hrt)

    if topic == 'esp32/gas':
        gs = message.payload.decode()
        gas = json.loads(gs)

    # print(temperature, heart_rate, gas)

@app.route('/_stuff', methods = ['GET'])
def stuff():
    global temperature, heart_rate, gas
    return jsonify( temperature=temperature, heart_rate=heart_rate, gas=gas  )

@app.route('/cool_form', methods=['GET', 'POST'])
def cool_form():
    return render_template('cool_form.html')

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(host = '0.0.0.0', port = 8080)
