def detect_alert(data):
    if data["health_status"] == "CRITICAL":
        return {
            "device_id": data["device_id"],
            "severity": "CRITICAL",
            "reason": "Critical ventilation condition detected",
        }

    return None