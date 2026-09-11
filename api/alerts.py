from analysis import has_invalid_sensor_data, within_tolerance

def detect_alert(data, settings):
    alerts = []

    invalid_sensors = has_invalid_sensor_data(
        data["temperature"],
        data["humidity"],
        data["fan_speed_rpm"],
        data["fan_status"]
    )

    for sensor in invalid_sensors:
        alerts.append({
            "device_id": data["device_id"],
            "severity": "CRITICAL",
            "reason": f"{sensor} sensor is invalid".capitalize()
            })

    if data["fan_status"] == "running" and data["fan_speed_rpm"] == 0:
        alerts.append({
            "device_id": data["device_id"],
            "severity": "CRITICAL",
            "reason": f"Fan is running but RPM is {data['fan_speed_rpm']}"
    })

    if "temperature" not in invalid_sensors and not within_tolerance(
        data["temperature"],
        settings["target_temperature"],
        0.10
    ):
        alerts.append({
            "device_id": data["device_id"],
            "severity": "WARNING",
            "reason": f"Temperature is {data['temperature']}°C, target is {settings['target_temperature']}°C"           
        })

    if "humidity" not in invalid_sensors and not within_tolerance(
        data["humidity"],
        settings["target_humidity"],
        0.15
    ):
        alerts.append({
            "device_id": data["device_id"],
            "severity": "WARNING",
            "reason": f"Humidity is {data['humidity']}%, target is {settings['target_humidity']}%"           
        })
 
    
    

    return alerts
