def determine_health_status(data, settings):
    fan_status = data["fan_status"]
    fan_speed_rpm = data["fan_speed_rpm"]
    temperature = data["temperature"]
    humidity = data["humidity"]

    invalid_sensors =  has_invalid_sensor_data(
        temperature, 
        humidity, 
        fan_speed_rpm, 
        fan_status
        )
        
    if invalid_sensors: 
        return "CRITICAL"

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

def has_invalid_sensor_data(temperature, humidity, fan_speed_rpm, fan_status):
    invalid = []


    values = {
        "temperature": temperature, 
        "humidity": humidity, 
        "fan_speed_rpm": fan_speed_rpm, 
        "fan_status": fan_status
        }
    
    expected_types = {
        "temperature": (int, float),
        "humidity": (int, float),
        "fan_speed_rpm": (int, float),
        "fan_status": str
    }

    for name, value in values.items():
        if value is None:
            invalid.append(name)
            continue

        if not isinstance(value, expected_types[name]):
            invalid.append(name) 
            continue
        
        if name == "temperature" and not -50 <= value <= 60:
            invalid.append(name) 
            continue

        if name == "humidity" and not 0 <= value <= 100:
            invalid.append(name) 
            continue
        
        if name == "fan_speed_rpm" and value < 0:
            invalid.append(name) 
            continue

        if name == "fan_status" and value not in ("running", "stopped"):
            invalid.append(name) 
            continue
        

    
    return invalid
    
    

