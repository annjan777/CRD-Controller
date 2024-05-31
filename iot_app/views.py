from django.shortcuts import render
from django.http import JsonResponse
from paho.mqtt import client as mqtt_client
import random
import threading
import time

broker = '43.205.50.167'
port = 1883
topic = "MQTT"
client_id = f'client-{random.randint(0, 100)}'

# Global variable to store received messages
received_messages = []
sent_messages = []

# MQTT Client setup
mqtt_client_instance = mqtt_client.Client(client_id=client_id)

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print(f"Failed to connect, return code {rc}\n")

    mqtt_client_instance.on_connect = on_connect
    mqtt_client_instance.connect(broker, port)
    mqtt_client_instance.loop_start()
    return mqtt_client_instance

def subscribe():
    client = connect_mqtt()

    def on_message(client, userdata, msg):
        message = msg.payload.decode().strip()  # Remove any line breaks or spaces
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

# Start the subscriber thread once at the beginning
subscriber_thread = threading.Thread(target=subscribe)
subscriber_thread.start()

def publish_to_iot(request):
    if request.method == 'POST':
        message_payload = request.POST.get('message', '').strip()  # Ensure no trailing spaces or line breaks
        client = mqtt_client_instance
        client.publish(topic, message_payload)
        return JsonResponse({"message": f"Message '{message_payload}' published to MQTT broker"})
    return render(request, 'iot_app/publish_form.html', {'received_messages': received_messages})

def view_logs(request):
    if request.is_ajax():
        return JsonResponse({'received_messages': received_messages})
    return render(request, 'iot_app/logs.html', {'received_messages': received_messages})

def about(request):
    return render(request, 'iot_app/about.html')

def home(request):
    return render(request, 'iot_app/home.html')

def contact(request):
    return render(request, 'iot_app/contact.html')

PARAMETER_CODES = {
    'crd_system_health': {'good': 'a', 'bad': 'b'},
    'control_supply': {'on': 'c', 'off': 'd'},
    'crd_on_off': {'on': 'e', 'off': 'f'},
    'manual_travel_length': 'tli',
    'crd_current_operations': 'opr',
    'overload_trip': 'trp',
    'cable_overtension': 'tns',
    'cable_slagness': 'slg',
    'chain_snap': 'snp',
    'motor_selection': {'h': 'VFD', 'g': 'Stall Torque'},
    'feed_selection': {'i': 'End', 'j': 'Center'},
    'local_remote_mode_selection': {'k': 'Remote', 'l': 'Local'},
    'left_right_sensor': {'m': 'proximity', 'n': 'inclination', 'o': 'PLS'},
    'travel_sensor': {'p': 'Encode', 'q': 'GLS', 'r': 'proximity'},
}

def update(request):
    if request.method == 'POST':
        data = request.POST
        encoded_parameters = []

        for param, value in data.items():
            if param in PARAMETER_CODES:
                if isinstance(PARAMETER_CODES[param], dict):
                    encoded_value = PARAMETER_CODES[param].get(value, value)
                    encoded_parameters.append(encoded_value)
                else:
                    encoded_value = f"{PARAMETER_CODES[param]},{value}"
                    encoded_parameters.append(encoded_value)
            else:
                encoded_parameters.append(value)

        # Publish to MQTT
        client = mqtt_client_instance
        for value in encoded_parameters:
            client.publish(topic, value)

        return JsonResponse({"message": "Parameters updated and sent to MQTT broker"})

    return render(request, 'iot_app/update.html')

def status(request):
    context = get_decoded_params()
    return render(request, 'iot_app/status.html', context)

def get_status(request):
    decoded_params = get_decoded_params()
    return JsonResponse(decoded_params)

def get_decoded_params():
    decoded_params = {
        'manual_travel_length': None,
        'motor_selection': None,
        'feed_selection': None,
        'local_remote_mode_selection': None,
        'left_right_sensor': None,
        'travel_sensor': None,
    }

    for msg in received_messages:
        message = msg[1].strip()
        if message.startswith("tli,"):
            param, value = decode_message(message)
            if param:
                decoded_params[param] = value
        else:
            for param, codes in PARAMETER_CODES.items():
                if isinstance(codes, dict) and message in codes:
                    decoded_params[param] = codes[message]

    return decoded_params

def decode_message(message):
    parts = message.split(',')
    if len(parts) != 2:
        return None, None
    code, value = parts
    for param, codes in PARAMETER_CODES.items():
        if codes == code:
            return param, value
    return None, None
