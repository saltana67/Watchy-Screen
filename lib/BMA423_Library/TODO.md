BMA423 is EOL, Bosch removed driver code from [github](https://github.com/BoschSensortec/BMA423-Sensor-API).  
Code ist not a problem, as it could be written from scratch using documentation and references,  
 but there is `const uint8_t bma423_config_file[]` in **BMA423_Library/src/bma423.c**,  
 containing a binary, program or config, which must be loaded to the sensor.
- check if drivers for other similar chips would function e.g. [BMA456](https://github.com/boschsensortec/BMA456_SensorAPI/tree/master)
- try to ugrade to latest Bosch code found: https://github.com/wasp-os/BMA423-Sensor-API

another link: https://github.com/lewisxhe/BMA423_Library/tree/master