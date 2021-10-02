# Fan Controller

This is a simple project in development to provide a multi temperature sensor IoT device including a Fan Controller, the focus resides in providing a JSON HTTP interface for scrape and configure remotely.

## State of the Project

The projects actual state is a working temperature fancontroller including metrics export in prometheus format.

Features:

* Control a fans speed by temp sensor of your choice
* Up to 8 temperature sensors
* Naming sensors
* Configuration is done over http
* Configuration is saved to flash memory and read on start
* Prometheus metrics export endpoint

Requriements:

* A compatible ESP8266 (i am using the  ESP-12E variant)
* DS18B20 Temparatur Sensors
* PWM Controlles FAN (4wire)

## Quickstart

Create a `src/Credentials.h` with your wifi credentials, use `src/Credentials.h.tpl`

On remote Host:

```shell
pio remote agent start
```

Connect Monitor:

```shell
pio remote device monitor -b 115200
```

Upload Filesystem:

```shell
pio remote run -t uploadfs --force-remote
```

Upload and Flash remote:

```shell
pio remote run -t upload --force-remote
```

## Test

To use the http calls provided by the `test/` folder you can use vscode extension `REST Client` and this config under `.vscode/settings.json`:

```json
{
  "rest-client.environmentVariables": {
    "$shared": {
        "fanControllerHost": "1.2.3.4"
    }
  }
}
```
