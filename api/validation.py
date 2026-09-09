def validate_telemetry(data):
    errors = []

    device_id = data.get("device_id")
    temperature = data.get("temperature")
    humidity = data.get("humidity")
    fan_speed_rpm = data.get("fan_speed_rpm")
    fan_status = data.get("fan_status")

    if not isinstance(device_id, str) or not device_id.strip():
        errors.append("device_id must be a non-empty string")

    if not isinstance(temperature, (int, float)):
        errors.append("temperature must be a number")

    if not isinstance(humidity, (int, float)):
        errors.append("humidity must be a number")

    if not isinstance(fan_speed_rpm, (int, float)):
        errors.append("fan_speed_rpm must be a number")

    if fan_status not in ("running", "stopped"):
        errors.append("fan_status must be 'running' or 'stopped'")

    return errors