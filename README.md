# RFM69-LaCrosse
## Short Description
This is a c++ application for recieving data from La Crosse ( TX29 , TX35 , ...) sensor with an RFM69 directly wired to an Raspberry Pi.
The sensors can be found at [lacrossetechnology.com/](http://www.lacrossetechnology.com/)
The recieved data is provided via TCP sockect on port 8888.

## Credits
The code of this application is inspired from [rfm69-library-for-raspberry-pi](http://rdepablos.merlitec.com/mixed/rfm69-library-for-raspberry-pi) which bases on [LowPowerLab/RFM69](https://github.com/LowPowerLab/RFM69).
It also uses [wiringpi Library] (http://wiringpi.com/)

## Usage
The application can be used like this:

sensor_supply [mode [toggle_time]]

where mode can either be:
* TX29 which will only recieve data of TX29 sensors
* TX35 which will only recieve data of this sensors
* toggle where the application will automatically toggle between TX29 and TX35

In toggle mode it's possible to define the toggle interval in seconds. Default toggle interval is 15 seconds.

Recieved data from 1 or more sensors it can be read via TCP connection on port 8888. Once the connection is established application waits for the sensor number which data is desired. Sensor number has to be supplied in decimal.

If data from desired sensor number is available it will be presented in the following form:
FALSE;FALSE;20.0;FALSE;66;11.406405;
which means:
- new battery - false
- reserved flag - false
- 20.0 degree celsius
- weak battery - false
- 66 percent relativ humidity
- 11.406405 absolute humidity (calculated from sensor values)

If there is no data for the desired sensor number the response will contain the desired number and a statement that this ID was not found.
After the response the tcp connection will be closed.
