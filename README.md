# thunderbird-onvif
Implementation of ONVIF Profile S, providing device management for networked camera streams over SOAP protocol.

# Usage
```yaml
server:
  username:  luminosity
  password:  123456
  http_port: 80
  devices:
  - name: camera0
    rtsp_port: 9000
    rtsp_uri: rtsp://localhost:9000
  - name: camera1
    rtsp_port: 9001
    rtsp_uri: rtsp://localhost:9001
```

# Source Code Configs
## Building
1) Clone the repository:  
```
git clone https://github.com/ASU-LL/thunderbird-onvif
```
2) Install necessary libraries for gSOAP protocol and YAML parser:
```
sudo apt-get install -y --no-install-recommends gsoap libgsoap-dev libssl-dev zlib1g-dev libyaml-cpp-dev
```
3) Run make (this will take ~2 minutes):
```
cd thunderbird-onvif
make
```
## Configuring
### Device Parameters & PTZ Control
### RTSP URLs
### Global Context