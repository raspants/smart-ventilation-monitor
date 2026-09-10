from analysis import has_invalid_sensor_data

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
            "reason": f"{sensor} sensor is invalid"
            })

    if data["fan_status"] == "running" and data["fan_speed_rpm"] == 0:
        alerts.append({
            "device_id": data["device_id"],
            "severity": "CRITICAL",
            "reason": f"Fan is running but RPM is {data['fan_speed_rpm']}"
    })
 
    
    

    return alerts
