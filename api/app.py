from flask import Flask, jsonify, request
from flask_cors import CORS
import os
import socket
from mqtt import start_mqtt
from offline import start_offline_monitor


from db import( 
    get_connection,
    get_devices,
    get_latest_measurement,
    get_measurements,
    known_device,
    get_settings,
    update_settings,
    get_alerts,
)
app = Flask(__name__)
CORS(app)

APP_VERSION = os.getenv("APP_VERSION", "v1")
POD_NAME = socket.gethostname()

@app.get("/health")
def health():
    try:
        conn = get_connection()
        conn.close()
        return jsonify({
        "status": "ok",
        "database": "ok"
        }), 200

    except Exception:
        return jsonify({
        "status": "error",
        "database": "unavailable"        
        }), 503

@app.get("/devices")
def devices():
    return jsonify(get_devices()), 200


@app.get("/devices/<device_id>/latest")
def latest(device_id):

    if not known_device(device_id):
        return jsonify({"error": "unknown device"}), 404

    measurement = get_latest_measurement(device_id)

    if measurement is None:
        return jsonify({"error": "Measurement not found"}), 404

    return jsonify(measurement), 200


@app.get("/devices/<device_id>/measurements") #adjust to be callable with amount of time
def measurements(device_id):

    if not known_device(device_id):
        return jsonify({"error": "unknown device"}), 404

    measurements = get_measurements(device_id)

    return jsonify(measurements), 200

@app.get("/devices/<device_id>/settings")
def settings(device_id):
    
    if not known_device(device_id):
        return jsonify({"error": "unknown device"}), 404

    settings = get_settings(device_id)

    if settings is None:
        return jsonify({"error": "Settings not found"}), 404

    return jsonify(settings), 200

@app.put("/devices/<device_id>/settings")
def update(device_id):
    if not known_device(device_id):
        return jsonify({"error": "unknown device"}), 404

    data = request.get_json()

    fan_speed_setting = data["fan_speed_setting"]
    measurement_interval = data["measurement_interval"]

    updated = update_settings(
        device_id,
        fan_speed_setting,
        measurement_interval
    )

    if not updated:
        return jsonify({"error": "Settings not found"}), 404

    return jsonify({
        "message": "Settings updated",
        "device_id": device_id,
        "fan_speed_setting": fan_speed_setting,
        "measurement_interval": measurement_interval
    }), 200  

@app.get("/devices/<device_id>/alerts")
def alerts(device_id):

    if not known_device(device_id):
        return jsonify({"error": "unknown device"}), 404
    
    alerts = get_alerts(device_id)
    return jsonify(alerts), 200




mqtt_client = start_mqtt()
offline_thread = start_offline_monitor()

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=False)