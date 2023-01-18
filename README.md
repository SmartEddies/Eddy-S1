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

Set the MQTT broker name/address to connect to. If `:PORT` is not specified, 1883 or 8883 is used depending on whether SSL is enabled.

`mos config-set mqtt.enable=true mqtt.server=MQTT_BROKER:PORT`
### 5. Configure the WiFi connection
Connect your device to your local WiFi network. Type the following command on a terminal.

`mos wifi WIFI_NETWORK_NAME WIFI_PASSWORD`
### 6. Set the button mode (optional)
The B1 button supports 4 different modes:
|Mode|||
|--|--|--|
|0|DASH_BUTTON|The button runs like a [dash button](https://github.com/diy365-mgos/bbutton) raising [events](https://github.com/diy365-mgos/bbutton/blob/master/README.md#mgos_bbutton_event), but without changing the relay's status.|
|1|DETACHED|The button is detached and it doesn't change the relay's status.|
|2|TOGGLE_ON_PUSH|The button toggle the relay's status every time it is presed(closed) and then released(open).|
|3|TOGGLE_ON_EDGE|The button toggle the relay's status every time it is presed(closed) or released(open). This is the defualt mode.|

Type the following command on a terminal.

`mos config-set eddy.sw1.mode=MODE`
### 7. Set the relay inching timeout (optional)
Set the relay's inching timeout in seconds. The defualt value `0` means the inching is disabled. Type the following command on a terminal.

`mos config-set eddy.relay1.inching_timeout=TIMEOUT`
### 8. Connect the devide to mDash (optional)
[Add the device to the mDash management dashboard](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md#8-add-device-to-the-mdash-management-dashboard) and type the following command on a terminal to enable the [mDash](https://mdash.net/) connection.

`mos config-set dash.enable=true dash.token=TOKEN`
