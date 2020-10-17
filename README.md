# Arduino_BME680_Coldfire5282_CGI_Serial

Arduino_BME680_Coldfire5282_CGI_Serial is a C CGI project which gathers data from a BME680 sensor connected to an Arduino to a Colfire 5282 developement board.

Communication is made through RS232 protocol, but should work with other serial protocols with some adjustments.

![Alt text](https://github.com/Etik-Line/Arduino_BME680_Coldfire5282_CGI_Serial/Montage.png?raw=true "Montage")


## Installation

Considering you have installed a free COM image on your 5282,

You'll find the ```.ino``` file for the Arduino UNO.

The ```index.html``` file goes to ```home/httpd/```

The read_serial.c must be compiled from the [cross-compiler](http://www.dilnetpc.com/mHT5280-12.pdf) and put in the ```home/httpd/cgi-bin/``` folder and with a ```.cgi``` extension.

Be carfeull that support for this compiler need you to have Ubuntu 9.04 with no updates (your favourite VM is your friend).
 

## Usage
Simply access the main.html via any WebClient.

Command line : (mostly for testing, as it prints <html> tags)
```bash
./read_serial x
```
Where x can be traded for a - temperature
                          b - humidity
                          c - pressure
                          d - gaz levels

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[GPL-3.0 License](https://choosealicense.com/licenses/gpl-3.0/)
