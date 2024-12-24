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
.relax-btn {
    background: linear-gradient(45deg, #ff0000, #00ff00, #0000ff);
    color: white;
    padding: 15px 30px;
    font-size: 18px;
    margin: 20px;
    background-size: 200% 200%;
    animation: gradient 5s ease infinite;
}
@keyframes gradient {
    0% {background-position: 0% 50%;}
    50% {background-position: 100% 50%;}
    100% {background-position: 0% 50%;}
}
    </style>
    <script>
let relaxationMode = false;

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

function toggleRelaxation() {
    relaxationMode = !relaxationMode;
    const btn = document.getElementById('relaxBtn');
    
    if (relaxationMode) {
        btn.textContent = 'Stop Relaxation Mode';
        btn.classList.add('active');
    } else {
        btn.textContent = 'Start Relaxation Mode';
        btn.classList.remove('active');
    }
    
    fetch('/toggleRelax?enabled=' + relaxationMode)
        .then(response => console.log('Relaxation mode:', relaxationMode));
}
    </script>
</head>
<body>
    <h1>ESP32 Control Panel</h1>
    
    <div class="container">
        <h2>RGB LED Control</h2>
        <input type="color" class="color-picker" onchange="handleColorPicker(event)">
        <div>
            <button class="btn red" onclick="setColor(255,0,0)">Red</button>
            <button class="btn green" onclick="setColor(0,255,0)">Green</button>
            <button class="btn blue" onclick="setColor(0,0,255)">Blue</button>
        </div>
        <div>
            <button id="relaxBtn" class="btn relax-btn" onclick="toggleRelaxation()">
                Start Relaxation Mode
            </button>
        </div>
    </div>
</body>
</html>
)rawliteral";

#endif