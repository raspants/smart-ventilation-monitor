import os
import paho.mqtt.client as mqtt
import json
from validation import validate_telemetry
from db import known_device, insert_measurement


MQTT_HOST = os.getenv("MQTT_HOST", "broker")
MQTT_PORT = int(os.getenv("MQTT_PORT", "1883"))
MQTT_TOPIC = os.getenv("MQTT_TOPIC", "smartvent/+/telemetry")

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print(
            f"Connected to MQTT broker at {MQTT_HOST}:{MQTT_PORT}",
            flush=True
        )
        
        client.subscribe(MQTT_TOPIC)

        print(f"Subscribed to {MQTT_TOPIC}", flush=True)

    else:
        print(f"MQTT connection failed: {reason_code}", flush=True)

def on_message(client, userdata, message):
    payload = message.payload.decode("utf-8")

    print(f"MQTT message received", flush=True)
    print(f"Topic: {message.topic}", flush=True)
    print(f"Payload: {payload}", flush=True)

    try:
        data = json.loads(payload)
        print(f"Parsed telemetry: {data}", flush=True)

    except json.JSONDecodeError:
        print("Invalid JSON payload", flush=True)
        return

    errors = validate_telemetry(data)

    if errors:
        print(f"Invalid telemetry: {errors}", flush=True)
        return

    print("Telemetry validated successfully", flush=True)

    device_id = data["device_id"]

    if not known_device(device_id):
        print(f"Unknown device: {device_id}", flush=True)
        return

    insert_measurement(data)

    print(f"Measurement stored for {device_id}", flush=True)


def start_mqtt():
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(MQTT_HOST, MQTT_PORT, 60)

    client.loop_start()

    return client
