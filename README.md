# Eddy-S1   
## Smart, powerful and cheap DIY WiFi switch
Eddy-S1 is a smart, powerful and cheap WiFi switch based on a [Mongoose OS](https://mongoose-os.com/) firmware. It can be easily integrated with your home automation system using MQTT protocol.
## Functional schema
This is the functional schema of Eddy S1.

![eddy-s1 functional schema](docs/eddy-s1_functional_schema.jpg)
## GET STARTED
### 1. Download and install mos tool
[Download](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md#1-download-and-install-mos-tool) and install `mos` tool.
### 2. Connect the device to your PC
Connect your device to your PC using a USB cable.
### 3. Upload the latest firmware
Download the [latest firmware](../../releases/latest/download/fw.zip) and flash it typing the following command on a terminal.

`mos flash fw.zip` 
### 4. Configure the MQTT connection
Connect your device to your MQTT broker. Type the following commands on a terminal according your broker authentication and protocol settings.

Set username and password if your broker requires them.

`mos config-set mqtt.user=USERNAME mqtt.pass=PASSWORD`

Disable SSL communiation if your broker doesn't support it.

`mos config-set mqtt.ssl_ca_cert=`

Set the server to connect to. If `:PORT` is not specified, 1883 or 8883 is used depending on whether SSL is enabled.

`mos config-set mqtt.enable=true mqtt.server=BROKER_HOST:PORT`
### 5. Configure the WiFi connection
Connect your device to your local WiFi network. Type the following command on a terminal.

`mos wifi WIFI_NETWORK_NAME WIFI_PASSWORD`



1. Connect your PC to the Eddy S1 using an USB cable or WiFi
2. Set the button mode (optional)
3. Set the relay inching mode (optional)
4. Set [mDash](https://mdash.net/) connection (optional)
5. Set MQTT connection
6. Connect Eddy S1 to your WiFi
## Connect your PC to the Eddy S1
Connect your PC to the Eddy S1 to start the initial configuration or to update it. You can establish the connection in two ways:
## Set the button mode
The B1 button supports 4 different modes:
|Mode|||
|--|--|--|
|0|DASH_BUTTON|The button runs like a [dash button](https://github.com/diy365-mgos/bbutton) raising [events](https://github.com/diy365-mgos/bbutton/blob/master/README.md#mgos_bbutton_event), but without changing the relay's status.|
|1|DETACHED|The button is detached and it doesn't change the relay's status.|
|2|TOGGLE_ON_PUSH|The button toggle the relay's status every time it is presed(closed) and then released(open).|
|3|TOGGLE_ON_EDGE|The button toggle the relay's status every time it is presed(closed) or released(open). This is the defualt mode.|
