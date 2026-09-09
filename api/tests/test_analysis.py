from analysis import determine_health_status

def test_healthy():
    data = {
        "temperature": 22.0,
        "humidity": 45.0,
        "fan_speed_rpm": 1200,
        "fan_status": "running"
    }

    settings = {
        "target_temperature": 22.0,
        "target_humidity": 45.0,
        "target_rpm": 1200
    }

    assert determine_health_status(data, settings) == "HEALTHY"


def test_warning():
    data = {
        "temperature": 30.0,
        "humidity": 45.0,
        "fan_speed_rpm": 1200,
        "fan_status": "running"
    }

    settings = {
        "target_temperature": 22.0,
        "target_humidity": 45.0,
        "target_rpm": 1200
    }

    assert determine_health_status(data, settings) == "WARNING"


def test_critical():
    data = {
        "temperature": 22.0,
        "humidity": 45.0,
        "fan_speed_rpm": 0,
        "fan_status": "running"
    }

    settings = {
        "target_temperature": 22.0,
        "target_humidity": 45.0,
        "target_rpm": 1200
    }

    assert determine_health_status(data, settings) == "CRITICAL"