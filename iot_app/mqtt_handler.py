# mqtt_handler.py (or wherever you handle MQTT messages)
from django.contrib.auth.models import User
from .models import ReceivedMessage

def handle_mqtt_message(user_id, message, topic):
    user = User.objects.get(id=user_id)
    ReceivedMessage.objects.create(user=user, topic=topic, message=message)
