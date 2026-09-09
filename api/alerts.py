def detect_alert(data, settings):
    alerts = []

    if data["fan_status"] == "running" and data["fan_speed_rpm"] == 0:
        alerts.append({
            "device_id": data["device_id"],
            "severity": "CRITICAL",
            "reason": f"Fan is running but RPM is {data['fan_speed_rpm']}"

    })
 
    

    return alerts
