def determine_health_status(data, settings):
    fan_status = data["fan_status"]
    fan_speed_rpm = data["fan_speed_rpm"]

    temperature = data["temperature"]
    humidity = data["humidity"]

    target_rpm = settings["target_rpm"]
    target_temperature = settings["target_temperature"]
    target_humidity = settings["target_humidity"]

    #Fläkten ska gå men står still
    if fan_status == "running" and fan_speed_rpm == 0:
        return "CRITICAL"

    # RPM får avvika ±15 %
    if not within_tolerance(fan_speed_rpm, target_rpm, 0.15):
        return "WARNING"

    # Temperatur får avvika ±10 %
    if not within_tolerance(temperature, target_temperature, 0.10):
        return "WARNING"

    # Luftfuktighet får avvika ±15 %
    if not within_tolerance(humidity, target_humidity, 0.15):
        return "WARNING"

    return "HEALTHY"

def within_tolerance(actual, target, tolerance):
    lower = target * (1 - tolerance)
    upper = target * (1 + tolerance)

    return lower <= actual <= upper
