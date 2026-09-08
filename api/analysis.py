def determine_health_status(data):
    fan_status = data["fan_status"]
    fan_speed_rpm = data["fan_speed_rpm"]

    if fan_status == "running" and fan_speed_rpm == 0:
        return "CRITICAL"

    return "HEALTHY"
