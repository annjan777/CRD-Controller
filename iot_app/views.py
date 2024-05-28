# iot_app/views.py

from django.shortcuts import render
from django.http import HttpResponse
from paho.mqtt import client as mqtt_client
from django.http import JsonResponse
import random
import threading
import time  # Import time module for timestamping

broker = '43.205.50.167'
port = 1883
topic = "MQTT"
client_id = f'client-{random.randint(0, 100)}'

# Global variable to store received messages
received_messages = []
sent_messages=[]

# Function to connect to MQTT broker
def connect_mqtt() -> mqtt_client.Client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print(f"Failed to connect, return code {rc}\n")

    client = mqtt_client.Client(client_id=client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

# Function to subscribe to MQTT topic and handle incoming messages
def subscribe():
    client = connect_mqtt()

    def on_message(client, userdata, msg):
        message = msg.payload.decode()
        print(f"Received `{message}` from `{msg.topic}` topic")
        current_time = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
        received_messages.append((msg.topic, message, current_time))
        if len(received_messages) > 10:  # Keep only the last 10 messages
            received_messages.pop(0)
        sent_messages.append((msg.topic, message, current_time))
        

    client.subscribe(topic)
    client.on_message = on_message

    # Start the MQTT client loop in a separate thread
    client.loop_start()

# Start the subscriber thread
subscriber_thread = threading.Thread(target=subscribe)
subscriber_thread.start()

# View function to render the publish form and handle message publishing
def publish_to_iot(request):
    if request.method == 'POST':
        message_payload = request.POST.get('message', '')  # Get message from form data
        client = connect_mqtt()
        client.loop_start()
        client.publish(topic, message_payload)
        client.loop_stop()
        return HttpResponse(f"Message '{message_payload}' published to MQTT broker")

    return render(request, 'iot_app/publish_form.html', {'received_messages': received_messages})

# View function to render the logs page
def view_logs(request):
    if request.is_ajax():
        return render(request, 'iot_app/logs.html', {'received_messages': received_messages})
    return render(request, 'iot_app/logs.html', {'received_messages': received_messages})
def about(request):
    # Add any necessary logic here
    return render(request, 'iot_app/about.html')
def home(request):
    # Add any necessary logic here
    return render(request, 'iot_app/home.html')
def update(request):
    # Add any necessary logic here
    return render(request, 'iot_app/update.html')
def contact(request):
    # Add any necessary logic here
    return render(request, 'iot_app/contact.html')