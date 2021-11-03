# Geo-location Application Server

This folder contains the files describing the node-red application server code.

## Install & Configure

This Application Server needs the following to be installed first:

- Node-red
- semtech-wsp-apps/node-red-contrib-loracloud-utils
  - and the dependencies of this package

The install procedure for these is described [here](https://lora-developers.semtech.com/build/software/lora-basics/lora-basics-for-end-nodes/developer-walk-through/?url=application_server.html#setup). Be careful to **NOT** import the examples for End Nodes Demo flow.

Instead of importing the End Nodes Demo flow, import the files *modem.json* and *geolocation.json* available in the *geolocation_application_server/* directory.

The web page also indicates how to [configure the flow](https://lora-developers.semtech.com/build/software/lora-basics/lora-basics-for-end-nodes/developer-walk-through/?url=application_server.html#configure-the-flow) concerning the MQTT connections with LoRaWAN Network Servers and LoRa Cloud.
Follow the same procedure but for the flow distributed in this folder.

This application server needs to use both *LoRa Cloud Geolocation* and *LoRa Cloud Device & Application Services*.
Obtaining both *LoRa Cloud Device & Application Services* token and URL is explained in the web page here-before.
The *LoRa Cloud Geolocation* token and URL are also available on the LoRa Cloud account, but in *LoRa Cloud Geolocation* section instead of *LoRa Cloud Device & Application Services*.

### Maps

The Application Server generates and display interactive maps that are made available at the following URLs:

- `<NODE-RED-INSTANCE-BASE-URL>:<NODE-RED-INSTANCE-BASE-PORT>/worldmap_wifi` : for Wi-Fi results
- `<NODE-RED-INSTANCE-BASE-URL>:<NODE-RED-INSTANCE-BASE-PORT>/worldmap_gnss_single` : for GNSS Single-frame solving
- `<NODE-RED-INSTANCE-BASE-URL>:<NODE-RED-INSTANCE-BASE-PORT>/worldmap_gnss_multiframe_grouping` : for GNSS Multi-frame solving with grouping accumulation

The fields `<NODE-RED-INSTANCE-BASE-URL>` and `<NODE-RED-INSTANCE-BASE-PORT>` are typically the one used to connect to the Node-red interface. On a classical local installation these are respectively `http://127.0.0.1` and `1880`.

The meaning concerning the different maps is provided here-after.

## Important note

When executing the Application Server instance, it is important to ensure that there is no other Application Server is monitoring the same devices. This can be the case if the examples from *semtech-wsp-apps/node-red-contrib-loracloud-utils* have been installed previously.

If this is the case, the flows corresponding to the examples from *semtech-wsp-apps/node-red-contrib-loracloud-utils* must be disabled (refer to the [doc](https://nodered.org/docs/user-guide/editor/workspace/flows#enabling-or-disabling-a-flow) for the procedure).

## Usage

Once deployed, the Application Server runs alone and does not need intervention. When the LoRa Network Server it is connected to receives an uplink, the Application Server receives a MQTT message.

This reception of the MQTT message triggers a http request to the *LoRa Cloud Device & Application Services*. The *LoRa Cloud Device & Application Services* may respond with a downlink request that is propagate automatically by the Application Server to the LoRaWAN Network Server.

The reception of the MQTT message also executes the Geolocation flow. If LoRaWAN port of the uplink corresponds to the geolocation application port (195) then the payload is interpreted as a geolocation result:

- Wi-Fi result; or
- GNSS result; or
- unknown result.

If the payload cannot be interpreted as Wi-Fi result of GNSS result, it is discarded.

If the payload is interpreted successfully as a Wi-Fi or GNSS result, then these results are solved to obtain a location estimation of the device that is displayed on different maps. The details of the result obtaining is provided hereafter.

### Wi-Fi result

If the geolocation payload corresponds to a Wi-Fi result, then the *LoRa Cloud Device & Application Services* is contacted to solve the location of the device based on these Wi-Fi result. If the *LoRa Cloud Device & Application Services* responds with a valid location, then this location is displayed on the `/worldmap_wifi` map.

When clicking on each location points, the MAC addresses scanned are provided.

### GNSS result

Every GNSS payload reception triggers the three following pipelines:

- execution of a Single frame solving; and
- appending to the grouping queue.

The GNSS single frame solving is executed by a call to the *LoRa Cloud Device & Application Services*. If the solve is successful, then the location is displayed on the map `/worldmap_gnss_single`.

The grouping queue allows to store together scans that belongs to the same scan group (refer to readme of the GNSS geolocation demo for details). When a GNSS payload frame is appending to the grouping queue, the Application Server determine if it belongs to the same scan group as the GNSS payload already present in the queue.

If the new GNSS payload is from a scan group different from the payload already in the queue, then the following is executed:

1. a multi frame solving with the content of the grouping queue;
1. flush of the grouping queue; and
1. appending of the new GNSS payload.

In case of successful solve, the result is displayed on the map `/worldmap_gnss_multiframe_grouping`.
