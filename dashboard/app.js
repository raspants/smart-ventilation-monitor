const unitList = document.querySelector("#units-list");
const unitName = document.querySelector("#unit-name");
const addUnit = document.querySelector("#add-unit");
const selectedUnit = document.querySelector("#selected-unit");
const saveSettings = document.querySelector("#save-settings");
const fanSpeed = document.querySelector("#fan-speed");
const measurementInterval = document.querySelector("#measurement-interval");
const fanSpeedValue = document.querySelector("#fan-speed-value");
const alertsList = document.querySelector("#alerts-list")

let selectedDeviceId = null;

function loadDevices(){
    fetch("http://localhost:5001/devices")
        .then(response => response.json())
        .then(data => {
            unitList.innerHTML = "";
            data.forEach(device => {
                console.log(device.device_id);
                
                const unit = document.createElement("li");
                if (device.device_id === selectedDeviceId){
                    unit.classList.add("selected");
                }
                unit.textContent = `${device.device_id} - ${device.name}`;
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

                const fanStatus = document.createElement("span");

                let fanEmoji;

                switch (device.fan_status) {
                    case "running":
                        fanEmoji = "🟢";
                        fanText = "Running";
                        break;
                    case "stopped":
                        fanEmoji = "⚫";
                        fanText = "Stopped";
                        break;
                    default:
                        fanEmoji = "⚪";
                        fanText = "No data";
                        break;
                }

                fanStatus.textContent = `${fanEmoji} ${fanText}`;

                unit.appendChild(fanStatus);

                unitList.appendChild(unit);

                if (selectedDeviceId !== null){
                    if (selectedDeviceId === device.device_id) {
                        fetch(`http://localhost:5001/devices/${selectedDeviceId}/latest`)
                        .then(response => response.json())
                        .then(measurement => {
                            console.log("refreshing selected unit:", selectedDeviceId);
                            console.log(measurement);

                            const healthClass = measurement.health_status.toLowerCase();
                            const fanClass = measurement.fan_status.toLowerCase();

                            selectedUnit.innerHTML = `
                                <p class="health-status ${healthClass}">Health status: ${measurement.health_status}</p>
                                <p class="fan-status ${fanClass}">Fan status: ${measurement.fan_status}</p>
                                <p>Temperature: ${measurement.temperature} °C</p>
                                <p>Humidity: ${measurement.humidity} %</p>
                                <p>Fan RPM: ${measurement.fan_speed_rpm}</p>
                                <p>Fan Speed: ${measurement.fan_speed_setting}</p>
                            `;
                        })
                    }
                }
            });

        });
}

loadDevices();
setInterval(loadDevices,5000);

addUnit.addEventListener("click", () => {
    const unitName = document.querySelector("#unit-name").value;

    if (unitName === "") { 
        return;
    }

    fetch("http://localhost:5001/devices", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            name: unitName
        })
    })
     .then(() =>{
        loadDevices()
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