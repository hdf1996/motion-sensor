# Motion sensor

This motion sensor is meant to be compatible with an existing Phillips HUE bridge, being connected as an IP sensor. Would love it to be Zigbee, but WIFI boards are easier & cheaper to get where I live!

## Setup

- Retrieve your Phillips HUE Bridge IP
- Press the button on your HUE Bridge
- Create an IP Sensor 
```bash
curl -X POST \
  http://192.168.0.59/api \
  -H 'Content-Type: application/json' \
  -d '{
   "devicetype": "nodemcu-motion#instance_name",
   "generateclientkey": true
}'
```
- Store the output, you'll need the `username` and `clientKey`
```json
[{"success":{"username":"MrvvPyd2rFxW03L6dBmNMgC1HQgwh-tEfEGta5HR","clientkey":"CD885EFEC9B8B400F99D3C97EBCB1486"}}]
```
- Create a sensor with
```bash
curl -X POST \
  http://192.168.0.59/api/MrvvPyd2rFxW03L6dBmNMgC1HQgwh-tEfEGta5HR/sensors \
  -H 'Content-Type: application/json' \
  -H 'hue-application-key: CD885EFEC9B8B400F99D3C97EBCB1486' \
  -d '{
   "name": "test",
   "type": "CLIPPresence",
   "modelid": "HU2024",
   "manufacturername": "HU2024",
   "swversion": "10.0",
   "uniqueid": "1234ABCD"
}'
```
- Store the output, you'll need the id
```json
[{"success":{"id":"18"}}]
```
- Update these values before flashing your NodeMCU
- list your scenes