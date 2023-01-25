# thunderbird-onvif
Implementation of ONVIF Profile S, which provides device management for networked cameras over the SOAP protocol.

# Downloading
Use a release
# Building from Source
1) Clone the repository:  
```
git clone https://github.com/ASU-LL/thunderbird-onvif
```
2) Install necessary libraries for gSOAP protocol:
```
sudo apt-get install -y --no-install-recommends gsoap libgsoap-dev libssl-dev zlib1g-dev
```
3) Run make:
```
cd thunderbird-onvif
make
```