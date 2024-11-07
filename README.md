# onvif_server
Implementation of ONVIF Profile S, providing device management for networked camera streams over SOAP protocol.

# Source Code Configs
## Building
1) Clone the repository:  
```
git clone https://github.com/danimal4326/onvif_server
```
2) Install necessary libraries for gSOAP protocol and YAML parser:
```
sudo apt-get install -y --no-install-recommends gsoap libgsoap-dev libssl-dev zlib1g-dev libyaml-cpp-dev
```
3) Run make (this will take ~2 minutes):
```
cd onvif_server
make
```
## Configuring and Running

Create a yaml file with configuration details:

config.yaml
```yaml
server:
  username:  admin
  password:  admin
  http_port: 1000
  Manufacturer: Onvif
  Model: Camera
  devices:
  - name: Stream0
    token: RTSP
    rtsp_url: rtsp://localhost/rtsp_stream
    snapshot_url: http://localhost/image.jpg
    width: 1280
    height: 720
    framerate: 30
    bitrate: 1600
```

`onvif_server -c config.yaml`
