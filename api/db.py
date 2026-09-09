import os
from decimal import Decimal
import psycopg2
import psycopg2.extras


def get_connection():
    return psycopg2.connect(
        host=os.getenv("DB_HOST", "db"),
        port=os.getenv("DB_PORT", "5432"),
        dbname=os.getenv("DB_NAME", "smart_vent"),
        user=os.getenv("DB_USER", "admin"),
        password=os.getenv("DB_PASSWORD", "admin"),
    )

def known_device(device_id):
    query = """
    SELECT 1
    FROM devices
    WHERE device_id = %s
    LIMIT 1
    """    
    with get_connection() as conn:
        with conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor) as cur:
            cur.execute(query, (device_id,))
            return cur.fetchone() is not None
    

def get_devices():
    query = """
    SELECT device_id, status
    FROM devices
    ORDER BY device_id
    """
    with get_connection() as conn:
        with conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor) as cur:
            cur.execute(query)
            return [dict(row) for row in cur.fetchall()]

def get_latest_measurement(device_id):
    query = """
    SELECT device_id,
        temperature,
        humidity,
        fan_speed_setting,
        fan_speed_rpm,
        fan_status,
        health_status
    FROM measurements
    WHERE device_id = %s
    ORDER BY created_at DESC
    LIMIT 1;
    """
    with get_connection() as conn:
        with conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor) as cur:
            cur.execute(query, (device_id,))
            row = cur.fetchone()

            if row is not None:
                return dict(row)
            
            return None

def get_measurements(device_id):
    query = """
    SELECT device_id,
        temperature,
        humidity,
        fan_speed_setting,
        fan_speed_rpm,
        fan_status,
        health_status
    FROM measurements
    WHERE device_id = %s
    ORDER BY created_at DESC
    """
    with get_connection() as conn:
        with conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor) as cur:
            cur.execute(query, (device_id,))
            return [dict(row) for row in cur.fetchall()]

def get_settings(device_id):
    query = """
    SELECT device_id, 
    fan_speed_setting, 
    measurement_interval,
    target_temperature,
    target_humidity,
    target_rpm
    FROM settings
    WHERE device_id = %s
    """
    with get_connection() as conn:
        with conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor) as cur:
            cur.execute(query, (device_id,))
            row = cur.fetchone()

            if row is not None:
                row = dict(row)

                if isinstance(row["target_temperature"], Decimal):
                    row["target_temperature"] = float(row["target_temperature"])

                if isinstance(row["target_humidity"], Decimal):
                    row["target_humidity"] = float(row["target_humidity"])

                return row

            return None

def update_settings(device_id, fan_speed_setting, measurement_interval):
    query = """
    UPDATE settings
    SET fan_speed_setting = %s,
        measurement_interval = %s
    WHERE device_id = %s;            
    """
    with get_connection() as conn:
        with conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor) as cur:
            cur.execute(query, (fan_speed_setting, measurement_interval, device_id))
            return cur.rowcount > 0
    

def insert_measurement(data):
    query = """
    INSERT INTO measurements (
        device_id,
        temperature,
        humidity,
        fan_speed_setting,
        fan_speed_rpm,
        fan_status,
        health_status
    )
    VALUES (%s, %s, %s, %s, %s, %s, %s)
    """

    with get_connection() as conn:
        with conn.cursor() as cur:
            cur.execute(
                query,
                (
                    data["device_id"],
                    data["temperature"],
                    data["humidity"],
                    data["fan_speed_setting"],
                    data["fan_speed_rpm"],
                    data["fan_status"],
                    data["health_status"],
                ),
            )

def insert_alert(alert):
    query = """
    INSERT INTO alerts (
        device_id,
        severity,
        reason
    )
    VALUES (%s, %s, %s)
    """

    with get_connection() as conn:
        with conn.cursor() as cur:
            cur.execute(
                query,
                (
                    alert["device_id"],
                    alert["severity"],
                    alert["reason"],
                ),
            )

def update_device_status(device_id, status):
    query = """
    UPDATE devices
    SET status = %s,
        last_seen = NOW()
    WHERE device_id = %s
    """

    with get_connection() as conn:
        with conn.cursor() as cur:
            cur.execute(query, (status, device_id))

def mark_offline_devices(timeout_seconds):
    query = """
    UPDATE devices
    SET status = 'OFFLINE'
    WHERE last_seen IS NOT NULL
      AND last_seen < NOW() - (%s * INTERVAL '1 second')
      AND status != 'OFFLINE'
    """

    with get_connection() as conn:
        with conn.cursor() as cur:
            cur.execute(query, (timeout_seconds,))
            return cur.rowcount

def get_alerts(device_id):
    query = """
    SELECT id, device_id, severity, reason, created_at
    FROM alerts
    WHERE device_id = %s
    ORDER BY created_at DESC
    """ 
    with get_connection() as conn:
        with conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor) as cur:
            cur.execute(query, (device_id,))
            return [dict(row) for row in cur.fetchall()]