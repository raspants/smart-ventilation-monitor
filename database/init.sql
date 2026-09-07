CREATE TABLE devices (
    id SERIAL PRIMARY KEY,
    device_id VARCHAR(100) UNIQUE NOT NULL,
    name VARCHAR(100),
    status VARCHAR(20) NOT NULL DEFAULT 'OFFLINE',
    last_seen TIMESTAMPTZ
);

CREATE TABLE measurements (
    id BIGSERIAL PRIMARY KEY,
    device_id VARCHAR(100) NOT NULL,

    temperature NUMERIC(5,2),
    humidity NUMERIC(5,2),
    fan_speed_setting INTEGER,
    fan_speed_rpm INTEGER,
    fan_status VARCHAR(20),
    health_status VARCHAR(20),

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    FOREIGN KEY (device_id) REFERENCES devices(device_id)
);

CREATE TABLE settings (
    device_id VARCHAR(100) PRIMARY KEY,
    fan_speed_setting INTEGER NOT NULL,
    measurement_interval INTEGER NOT NULL,

    FOREIGN KEY (device_id) REFERENCES devices(device_id)
);

CREATE TABLE alerts (
    id BIGSERIAL PRIMARY KEY,
    device_id VARCHAR(100) NOT NULL,
    severity VARCHAR(20) NOT NULL,
    reason TEXT NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    FOREIGN KEY (device_id)
        REFERENCES devices(device_id)
);
