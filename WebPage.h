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
            font-family: Arial;
            text-align: center;
            background-color: #f0f0f0;
            padding: 20px;
        }
        .container {
            background-color: white;
            border-radius: 10px;
            padding: 20px;
            margin: 20px auto;
            max-width: 800px;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
        }
        .button {
            background-color: #4CAF50;
            border: none;
            color: white;
            padding: 16px 40px;
            text-decoration: none;
            font-size: 20px;
            margin: 2px;
            cursor: pointer;
            border-radius: 5px;
            transition: all 0.3s ease;
        }
        .button:hover {
            opacity: 0.9;
        }
        .button.off {
            background-color: #f44336;
        }
        .color-controls {
            display: flex;
            gap: 10px;
            justify-content: center;
            flex-wrap: wrap;
            margin: 10px 0;
        }
        #colorPicker {
            width: 50px;
            height: 50px;
            padding: 0;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }
        .controls-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 20px;
        }
    </style>
    <script>
    function setCustomColor(hex) {
        fetch(`/color?hex=${hex.substring(1)}`);
    }

    function setColor(color) {
        fetch(`/color?c=${color}`);
    }

    function toggleRelay(number, state) {
        fetch(`/relay?relay=${number}&state=${state}`);
    }
    </script>
</head>
<body>
    <h1>ESP32 Control Panel</h1>

  
    <div class="container">
        <h2>RGB LED Control</h2>
        <div class="color-controls">
            <button class="button" style="background-color: #ff0000" onclick="setColor('red')">Red</button>
            <button class="button" style="background-color: #00ff00" onclick="setColor('green')">Green</button>
            <button class="button" style="background-color: #0000ff" onclick="setColor('blue')">Blue</button>
            <input type="color" id="colorPicker" onchange="setCustomColor(this.value)">
            <button class="button off" onclick="setColor('off')">OFF</button>
        </div>
    </div>
</body>
</html>
)rawliteral";

#endif