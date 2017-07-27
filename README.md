# GestAKey

    Gest A Key, yet so powerful...

### Components

#### Arduino

- Collects data from capactive touch matrix
- Send raw data to Python
    
#### Python

- Gesture recognizer algorithm
- Receives data from Arduino
- Classifies gesture and sends it to Node server

    ```
    cd data
    pyton3 data_read.py
    ```

#### C
- Listens to keypresses (up/down/mod)
- Adapted from [Daniel Beard](https://danielbeard.wordpress.com/2010/10/29/listening-for-global-keypresses-in-osx/)
- Sends keypresses to Node server

    ```
    cd sys
    gcc -Wall -o globalkeypress globalkeypress.c -framework ApplicationServices
    ./globalkeypress
    ```

#### JavaScript

- Node.js server that orchestrates everything
- Has three sockets
    - In socket: receives gestures from Python
    - In socket: receives keypresses (up/down/mod) from C
    - Out socket: sends keypresses to Python
    <br>

    ```
    cd app
    node index.js
    ```
