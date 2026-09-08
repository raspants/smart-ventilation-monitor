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
    SELECT device_id, fan_speed_setting, measurement_interval
    FROM settings
    WHERE device_id = %s
    """
    with get_connection() as conn:
        with conn.cursor(cursor_factory=psycopg2.extras.RealDictCursor) as cur:
            cur.execute(query, (device_id,))
            row = cur.fetchone()

            if row is not None:
                return dict(row)

            return None;

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
