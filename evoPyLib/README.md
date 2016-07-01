# EvoFab Controller Python Library

## How To Use:

### EvoController:

#### Creating a Controller:

```python
from evoPyLib import EvoController
 e = EvoController('/dev/ttyACM0') #param is the serial port of the controller
 ```

#### On close:

 Call ```e.close()```

#### Available Functions:

 - bool ```.extrude()```
    - begins extruding of filament
    - returns true iff command was successfully sent to the printer
 - bool ```.pause()```
    - Stops Extruding
    - returns true iff command was successfully sent to the printer
 - bool ```.home()```
    - homes build platform to center
    - this function is blocking, and will return true when the bed is homed.
    - returns false if homing failed
 - bool ```.testHome()```
    - brings bed to the bottom right hand corner of the printer for evaluation.
    - this function is blocking, and will return true when the bed is in position.
    - returns false if homing failed
 - bool ```.disable()```
    - disables motors and stops extruder.
    - used when bed must be manually moved or when evolution is complete.
    - returns true iff command was successfully sent to the printer
 - bool ```.changeVelocity('string')```
    - takes in an 8 character string of the x and y velocities to set the motors in the form of 'XVELYVEL'
        - Ex. ```e.changeVelocity('+100-100')```
    - returns true iff velocity string was in valid form and command was successfully sent to the printer

### EvoArray:

#### Creating an Array:

```python
from evoPyLib import EvoArray
 e = EvoArray('/dev/ttyACM0') #param is the serial port of the controller
 ```

#### On close:

 Call ```e.close()```

#### Available Functions:

 - tuple ```.getNext()```
    - returns an 8-tuple of the current values of the photo resistor array (values are integers)
