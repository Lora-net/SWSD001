# Application Server Example

This folder contains the files describing a node-red application server code.

## Install & Configure

This Application Server needs the following to be installed first:

- Node-red
- semtech-wsp-apps/node-red-contrib-loracloud-utils
  - and the dependencies of this package

The install procedure for these is described [here](https://lora-developers.semtech.com/build/software/lora-basics/lora-basics-for-end-nodes/developer-walk-through/?url=application_server.html#setup). Be careful to **NOT** import the examples for End Nodes Demo flow.

Instead of importing the End Nodes Demo flow, import the file *modem.json* available in this directory.

The web page also indicates how to [configure the flow](https://lora-developers.semtech.com/build/software/lora-basics/lora-basics-for-end-nodes/developer-walk-through/?url=application_server.html#configure-the-flow) concerning the MQTT connections with LoRaWAN Network Servers and LoRa Cloud.
Follow the same procedure but for the flow distributed in this folder.

This application server needs to use both *LoRa Cloud Modem & Geolocation Services*.
Obtaining both *LoRa Cloud Modem & Geolocation Services* token and URL is explained in the web page here-before.

## Important note

When executing the Application Server instance, it is important to ensure that there is no other Application Server is monitoring the same devices. This can be the case if the examples from *semtech-wsp-apps/node-red-contrib-loracloud-utils* have been installed previously.

If this is the case, the flows corresponding to the examples from *semtech-wsp-apps/node-red-contrib-loracloud-utils* must be disabled (refer to the [doc](https://nodered.org/docs/user-guide/editor/workspace/flows#enabling-or-disabling-a-flow) for the procedure).

## Usage

Once deployed, the Application Server runs alone and does not need intervention. When the LoRa Network Server it is connected to receives an uplink, the Application Server receives a MQTT message.

This reception of the MQTT message triggers a http request to the *LoRa Cloud Modem & Geolocation Services*. The *LoRa Cloud Modem & Geolocation Services* may respond with a downlink request that is propagate automatically by the Application Server to the LoRaWAN Network Server.
