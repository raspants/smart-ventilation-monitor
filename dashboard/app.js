const unitList = document.querySelector("section:first-child ul");
const selectedUnit = document.querySelector("#selected-unit");



fetch("http://localhost:5001/devices")
    .then(response => response.json())
    .then(data => {
        data.forEach(device => {
            console.log(device.device_id);
            
            const unit = document.createElement("li");
            unit.textContent = device.device_id;
            unit.addEventListener("click", () => {
                fetch(`http://localhost:5001/devices/${device.device_id}/latest`)
                    .then(response => response.json())
                    .then(measurement => {
                        const healthClass = measurement.health_status.toLowerCase();
                        const fanClass = measurement.fan_status.toLowerCase();
                        selectedUnit.innerHTML = `
                        <p class="health-status ${healthClass}">Health status: ${measurement.health_status}</p> 
                        <p class="fan-status ${fanClass}">Fan status: ${measurement.fan_status}</p>
                        <p>Temperature: ${measurement.temperature} °C</p>
                        <p>Humidity: ${measurement.humidity} %</p>
                        <p>Fan RPM: ${measurement.fan_speed_rpm}</p>
                        <p>Fan Speed: ${measurement.fan_speed_setting}</p> 
                        `
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