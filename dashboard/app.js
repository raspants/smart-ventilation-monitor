const unitList = document.querySelector("section:first-child ul");
const selectedUnit = document.querySelector("#selected-unit");
const saveSettings = document.querySelector("#save-settings");
const fanSpeed = document.querySelector("#fan-speed");
const measurementInterval = document.querySelector("#measurement-interval");
const fanSpeedValue = document.querySelector("#fan-speed-value");
const alertsList = document.querySelector("#alerts-list")

let selectedDeviceId = null;


fetch("http://localhost:5001/devices")
    .then(response => response.json())
    .then(data => {
        data.forEach(device => {
            console.log(device.device_id);
            
            const unit = document.createElement("li");
            unit.textContent = device.device_id;
            unit.addEventListener("click", () => {
                selectedDeviceId = device.device_id;
    
                fetch(`http://localhost:5001/devices/${device.device_id}/alerts`)
                    .then(response => response.json())
                    .then(alerts => {
                        alertsList.innerHTML = "";

                        alerts.forEach(alert => {
                            const item = document.createElement("li");
                            
                            const time = new Date(alert.created_at).toLocaleString("sv-SE", {
                                dateStyle: "short",
                                timeStyle: "short"
                            });

                            item.textContent = `${time} | ${alert.severity}: ${alert.reason}`;
                            alertsList.appendChild(item);
                        });
                    });

fetch(`http://localhost:5001/devices/${device.device_id}/latest`)
    .then(response => {
        if (!response.ok) {
            selectedUnit.innerHTML = `
                <p>No measurements available for this unit.</p>
            `;
            return null;
        }

        return response.json();
    })
    .then(measurement => {
        if (measurement === null) {
            return;
        }

        const healthClass = measurement.health_status.toLowerCase();
        const fanClass = measurement.fan_status.toLowerCase();
        
        fetch(`http://localhost:5001/devices/${device.device_id}/settings`)
            .then(response => response.json())
            .then(settings => {
                fanSpeed.value = settings.fan_speed_setting;
                fanSpeedValue.textContent = `${settings.fan_speed_setting}%`;
                measurementInterval.value = settings.measurement_interval;

                selectedUnit.innerHTML = `
                    <p class="health-status ${healthClass}">Health status: ${measurement.health_status}</p>
                    <p class="fan-status ${fanClass}">Fan status: ${measurement.fan_status}</p>
                    <p>Temperature: ${measurement.temperature} °C</p>
                    <p>Humidity: ${measurement.humidity} %</p>
                    <p>Fan RPM: ${measurement.fan_speed_rpm}</p>
                    <p>Fan Speed: ${measurement.fan_speed_setting}</p>
                    
                    <p>Target temperature: ${settings.target_temperature} °C</p>
                    <p>Target humidity: ${settings.target_humidity} %</p>
                    <p>Target RPM: ${settings.target_rpm}</p>

                `;
            });



        fanSpeed.addEventListener("input", () => {
            fanSpeedValue.textContent = `${fanSpeed.value}%`;
        });

        
    });
    });



            const health = document.createElement("span")

            let emoji;
            let fanText;

            switch (device.status) {
                case "HEALTHY":
                    emoji = "🟢";
                    break;
                case "WARNING":
                    emoji = "🟡";
                    break;
                case "CRITICAL":
                    emoji = "🔴";
                    break;
                case "OFFLINE":
                    emoji = "⚫";
                    break;
            }
            
            health.textContent = `${emoji} ${device.status}`;

            unit.appendChild(health);

            fetch(`http://localhost:5001/devices/${device.device_id}/latest`)
                .then(response => response.json())
                .then(measurement => {
                    console.log(measurement);

                    const fanStatus = document.createElement("span");
    
                    let emoji;

                    switch (measurement.fan_status) {
                        case "running":
                            emoji = "🟢";
                            fanText = "Running";
                            break;
                        case "offline":
                            emoji = "⚫";
                            fanText = "Offline";
                            break;
                    }
                    
                    fanStatus.textContent = `${emoji} ${fanText}`;

                    unit.appendChild(fanStatus);
                });

            unitList.appendChild(unit);

            
        });
    });


saveSettings.addEventListener("click", () => {
    if (selectedDeviceId === null) { 
        return;
    }

    const fanSpeed =  document.querySelector("#fan-speed").value; 
    const measurementInterval = document.querySelector("#measurement-interval").value; 

    fetch(`http://localhost:5001/devices/${selectedDeviceId}/settings`, {
        method: "PUT",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            fan_speed_setting: Number(fanSpeed),
            measurement_interval: Number(measurementInterval)
        })
    });
});