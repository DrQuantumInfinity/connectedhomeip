Test the light
    -hard code inject a serial message
    -may lead to moooore

why brightness/hsv not work

Test multicluster devices
    -like how does a RGB and temperature light at the same time work?

ESPnow class
    -provide an array of clusters to the device
    -bidirectional serialization from ESPNowMessage to cluster readers/writers
    -makes unique device types obsolete

Figure out mode clusters

linux bridge

Thermostat

other circuit board

persistent device detection


OTA
https://project-chip.github.io/connectedhomeip-doc/guides/esp32/ota.html

thread
https://openthread.io/codelabs/openthread-border-router#1

Mode
https://github.com/project-chip/connectedhomeip/blob/f09120f41f3d32678454db1915730f6f5730bb39/src/app/clusters/mode-base-server/README.md?plain=1#L7



nightlights for christmas

rgb pwm led
identify pins
app task to setup leds
3d printing/boxes

stretch goals
switch to change led brightness

temp sensor


Linux Bridge Setup:
sudo apt-get install libmosquitto-dev
wget http://launchpadlibrarian.net/741613665/libssl1.1_1.1.1f-1ubuntu2.23_arm64.deb
sudo dpkg -i libssl1.1_1.1.1f-1ubuntu2.23_arm64.deb 