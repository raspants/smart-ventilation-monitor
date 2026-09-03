-- Jag har lagt en grund så vi har något att utgå ifrån. 
-- Vi justerar schemat när vi spikat payload/API.
-- Stort auto-ID som primärnyckel, device-ID som måste finnas och vara unikt, 
-- och timestamp som automatiskt sätts till nu.


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
    temperature DOUBLE PRECISION, -- double precision för högre precision
    humidity DOUBLE PRECISION,
    vibration DOUBLE PRECISION,
    fan_state VARCHAR(20),
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    FOREIGN KEY (device_id)
        REFERENCES devices(device_id)
);

CREATE TABLE alerts (
    id BIGSERIAL PRIMARY KEY,
    device_id VARCHAR(100) NOT NULL,
    severity VARCHAR(100) NOT NULL,
    reason TEXT NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    FOREIGN KEY (device_id)
        REFERENCES devices(device_id)
)
