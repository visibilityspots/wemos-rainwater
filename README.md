# wemos-rainwater
Project which will measure the amount of liters in a rainwater collector using a wemos D1 mini pro with the JSNSR04T sensor as hardware.

Those will sent the measured values over MQTT.

OTA is implemented together with the WIFI AP method to initialize the wemos device the first time.

the secrets.h.example should be renamed to secrets.h and adjusted accordingly. 

## platformio

(https://platformio.org/)[https://platformio.org/] can be used to compile the code and upload it to device based on a platformio.ini file

```
$ cat platformio.ini
[platformio]
default_envs = d1_mini_pro

[env:d1_mini_pro]
platform = espressif8266
framework = arduino
board = d1_mini_pro

upload_port = /dev/ttyUSB0
upload_speed = 115200

lib_deps =
    PubSubClient
    WiFiManager
    ArduinoJson@5.13.4
    NewPing

monitor_port = /dev/ttyUSB0
monitor_speed = 115200

[env:d1_mini_lite]
platform = espressif8266
framework = arduino
board = d1_mini_lite

lib_deps =
    PubSubClient
    WiFiManager
    ArduinoJson@5.13.4
    NewPing

```

```
$ platformio run
```

```
$ platformio run -t upload
```

```
$ platformio device monitor
```
