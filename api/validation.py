def validate_telemetry(data):
    errors = []

    required_fields = [
        "temperature",
        "humidity",
        "fan_speed_rpm",
        "fan_status"
    ]

    for field in required_fields:
        if field not in data:
            errors.append(f"{field} is missing")

    return errors