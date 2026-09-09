def detect_alert(data, settings):
    alerts = []

    if data["health_status"] == "CRITICAL":
        alerts.append({
            "device_id": data["device_id"],
            "severity": "CRITICAL",
            "reason": f"Fan is running but no RPM is {data['fan_speed_rpm']}",

    })
 
    

    return alerts
