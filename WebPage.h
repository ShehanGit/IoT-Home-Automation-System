#ifndef WEBPAGE_H
#define WEBPAGE_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>ESP32 Control Panel</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: 'Arial', sans-serif;
            margin: 0;
            padding: 0;
            background: #f4f4f9;
            color: #333;
            text-align: center;
        }
        header {
            background: #4CAF50;
            color: white;
            padding: 20px;
            font-size: 24px;
            font-weight: bold;
        }
        .container {
            max-width: 800px;
            margin: 20px auto;
            padding: 20px;
            background: white;
            border-radius: 10px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }
        h2 {
            color: #4CAF50;
            margin-bottom: 10px;
        }
        .section {
            margin-bottom: 30px;
        }
        .btn-group {
            display: flex;
            gap: 10px;
            justify-content: center;
        }
        .btn {
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 14px;
            font-weight: bold;
            transition: background 0.3s, transform 0.2s;
        }
        .btn:hover {
            transform: translateY(-2px);
        }
        .btn.on {
            background: #4CAF50;
            color: white;
        }
        .btn.off {
            background: #f44336;
            color: white;
        }
        .color-picker {
            margin: 20px auto;
            width: 200px;
            height: 40px;
            border: none;
            border-radius: 8px;
            cursor: pointer;
        }
        .sensor-card {
            display: inline-block;
            margin: 10px;
            padding: 20px;
            width: 150px;
            background: #f9f9f9;
            border-radius: 10px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            text-align: center;
        }
        .sensor-value {
            font-size: 24px;
            font-weight: bold;
            color: #333;
        }
        .sensor-label {
            margin-top: 5px;
            color: #555;
            font-size: 14px;
        }
    </style>
    <script>
        // Control relay
        function controlRelay(relay, state) {
            fetch(`/relay?relay=${relay}&state=${state}`)
                .then(response => response.text())
                .catch(error => console.error('Error:', error));
        }

        // Set RGB LED color
        function setColor(r, g, b) {
            fetch(`/setColor?r=${r}&g=${g}&b=${b}`)
                .then(response => console.log('Color set'))
                .catch(error => console.error('Error:', error));
        }

        // Handle color picker change
        function handleColorPicker(event) {
            const color = event.target.value;
            const r = parseInt(color.substr(1, 2), 16);
            const g = parseInt(color.substr(3, 2), 16);
            const b = parseInt(color.substr(5, 2), 16);
            setColor(r, g, b);
        }

        // Fetch and update sensor data
        function updateSensorData() {
            fetch('/getSensorData')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('temperature').textContent = data.temperature.toFixed(1) + '°C';
                    document.getElementById('humidity').textContent = data.humidity.toFixed(1) + '%';
                })
                .catch(error => console.error('Error:', error));
        }

        // Periodic updates for sensor data
        window.onload = function () {
            updateSensorData();
            setInterval(updateSensorData, 2000); // Update every 2 seconds
        }
    </script>
</head>
<body>
    <header>
        ESP32 Control Panel
    </header>
    <div class="container">

        <!-- Relay Control Section -->
        <div class="section">
            <h2>Relay Control</h2>
            <div class="btn-group">
                <button class="btn on" onclick="controlRelay(1, 'on')">Relay 1 ON</button>
                <button class="btn off" onclick="controlRelay(1, 'off')">Relay 1 OFF</button>
            </div>
            <div class="btn-group">
                <button class="btn on" onclick="controlRelay(2, 'on')">Relay 2 ON</button>
                <button class="btn off" onclick="controlRelay(2, 'off')">Relay 2 OFF</button>
            </div>
            <div class="btn-group">
                <button class="btn on" onclick="controlRelay(3, 'on')">Relay 3 ON</button>
                <button class="btn off" onclick="controlRelay(3, 'off')">Relay 3 OFF</button>
            </div>
            <div class="btn-group">
                <button class="btn on" onclick="controlRelay(4, 'on')">Relay 4 ON</button>
                <button class="btn off" onclick="controlRelay(4, 'off')">Relay 4 OFF</button>
            </div>
        </div>

        <!-- RGB LED Control Section -->
        <div class="section">
            <h2>RGB LED Control</h2>
            <input type="color" class="color-picker" onchange="handleColorPicker(event)">
        </div>

        <!-- Sensor Data Section -->
        <div class="section">
            <h2>Sensor Data</h2>
            <div class="sensor-card">
                <div class="sensor-value" id="temperature">--°C</div>
                <div class="sensor-label">Temperature</div>
            </div>
            <div class="sensor-card">
                <div class="sensor-value" id="humidity">--%</div>
                <div class="sensor-label">Humidity</div>
            </div>
        </div>
    </div>
</body>
</html>
)rawliteral";

#endif
