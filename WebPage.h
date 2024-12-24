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
    margin: 0;
    padding: 20px;
}
.container {
    max-width: 600px;
    margin: auto;
}
.color-picker {
    width: 200px;
    height: 50px;
    margin: 20px auto;
}
.btn {
    padding: 10px 20px;
    margin: 5px;
    border: none;
    border-radius: 5px;
    cursor: pointer;
}
.red { background-color: #ff0000; color: white; }
.green { background-color: #00ff00; }
.blue { background-color: #0000ff; color: white; }
    </style>
    <script>
      function setColor(r, g, b) {
    fetch(`/setColor?r=${r}&g=${g}&b=${b}`)
        .then(response => console.log('Color set'));
}

function handleColorPicker(event) {
    const color = event.target.value;
    const r = parseInt(color.substr(1,2), 16);
    const g = parseInt(color.substr(3,2), 16);
    const b = parseInt(color.substr(5,2), 16);
    setColor(r, g, b);
}
    </script>
</head>
<body>
    <h1>ESP32 Control Panel</h1>

  
    <div class="container">
        <h2>RGB LED Control</h2>
    </div>

    <div class="container">
    <h2>RGB LED Control</h2>
    <input type="color" class="color-picker" onchange="handleColorPicker(event)">
    <div>
        <button class="btn red" onclick="setColor(255,0,0)">Red</button>
        <button class="btn green" onclick="setColor(0,255,0)">Green</button>
        <button class="btn blue" onclick="setColor(0,0,255)">Blue</button>
    </div>
</div>


</body>
</html>
)rawliteral";

#endif