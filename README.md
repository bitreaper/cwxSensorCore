# cwxSensorCore

## Synopsis

CheapWeather project sensor core firmware.  This project enables an ESP8266 to read an LM75 (or compatible) sensor
and send that data over WiFi to a server running the cwxDataCollector software.  This software sends the VCC measurement
as well, so that battery voltage levels can be tracked.


## Loading it

You will need the LM75 library found here: https://github.com/thefekete/LM75

You will also need the SimpleShell library found here: https://github.com/bitreaper/SimpleShell

Make sure you load them into the libraries/ folder under the Arduino install directory.


## Configuring it

cwxSensorCore stores any of the data it would need in the flash filesystem emulated by the EEPROM library calls.
Configuration of a node uses the serial console shell.  If you have just programmed it via the Arduino
environment, you should be able to launch the serial monitor by doing Ctrl+Shift+M or by going to Menu->Tools->SerialMonitor.
When the sensor node boots, you should see output showing the sensor node attempting to connect to what might look
like garbage characters for the SSID.  This is normal, since the EEPROM area that was read on boot isn't formatted with
useful data yet.  The commands available can be displayed by using the `help` command.  To see what data you have
saved, use the `dump` command.  Since SimpleShell is used, it means that any of the parameters to the commands below
cannot have spaces in them, as the space character is used to split the line read on the serial console input.  Should
you need to use spaces (for example, in the password field) you may need to make the command smarter by re-joining its
arguments with a space.

This software has two modes: sense and command.  To enter command mode, you will need to connect GPIO 14 (pin 14) to
ground, and reboot the sensor.  This will keep the node from sensing, sending and sleeping, and drop straight into
command line mode.

### Setting the SSID and password

Credentials for attaching to the WiFi network.

```
 creds <ssid> <password>
```

### Setting the name of the sensor node

This field is what the sensor uses to identify itself with the data collector server.  Usually you will want
to make it unique, because this is the string you will need when querying the sqlite3 database.

```
 name <sensor name>
```

### Setting the server string

This is the IP address or hostname that is resolvable via DNS and port (if you haven't modified the
config of the data collector).


```
 server http://192.168.0.4:8080
```

### Setting the sleep time

Time in seconds that you wish the sensor node to sleep before waking up and sending a measurement to the
data collector.

```
 time 300
```

After configuring all four fields, reboot the node to get it to connect to the WiFi network.  Once this
is successful, you may use the `send` command to attempt to measure and send data to the server.  You will
see "200 OK" return codes if it was successful, and other codes if it was not.  You can adjust the server
string if needed and resend as much as you need to ensure it gets through.  While you are testing this,
be sure to be looking at the console in which the data collector is running, because it will also give
you hints as to what it saw coming from the sensor node.



