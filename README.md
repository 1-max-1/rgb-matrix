# RGB Matrix
PCB and software for driving an 8x8 matrix of RGB LED's.

⚠ NOTE: The software is not 100% complete, most of it works but it needs a few final tweaks and cleanup.

![top_view](https://github.com/1-max-1/rgb-matrix/assets/44454544/49b84bfe-8572-452c-836f-eb40d9d4f547)
![side_view](https://github.com/1-max-1/rgb-matrix/assets/44454544/6721da1e-6e35-452b-baea-d5e43391b232)

When digging through my pile of old electronics components I came across an LED matrix that I purchased many years ago. I was never able to get it working at the time, especially since I was using a breadboard and about a billion jumper cables (this matrix on a breadboard is an awful experience, would not recommend). I decided to try again now that I have acquired new skills and improved my old ones.

The software is a standard PlatformIO project, see the relevant folders for code. The PCB is a KiCAD project located in the `rgb-matrix-pcb` folder.

# Electronics

The circuit consists of four 8-bit shift registers chained together (along with some decoupling capacitors and resistors for the LED's). The shift registers are driven through SPI. The SPI lines are routed to a JST connector, so the PCB can communicate with a microcontroller. The first 3 shift registers control the cathodes of the R, G and B LED's. Each pin is assigned to one row. The final shift register is connected to the common anode of the LED's, with each pin assigned to one column. The matrix can then be driven with multiplexing by scanning through each column and briefly illuminating the desired rows. Persistence of Vision will take care of the rest.

Note: There are far better ways to implement this idea. There are more modern and efficient IC's that are designed specifically for this task. However, I wanted to do it my way and use the four 74hc595 shift registers because they are the same ones I tried to use on my original attempt. I wanted to finally get them working, just because I can.

I did not want to solder the matrix directly to the PCB (this turned out to be a lifesaving desicion later on), instead there are 2 rows of header pins. The matrix connects to these and sits on top of the PCB like a shield.

### The transistor board
I made a few mistakes. The biggest was that I completely overestimated the amount of current that the shift resgister pins could source and sink. For some reason I thought it would be enough to drive an entire row of LED's. I was wrong. Again, there are IC's like constant current shift registers that address this problem. Rather than redesign the PCB and wait for it to be manufactured again, I printed a little shelf with holes that fit 8 BJT's. I then did a little bit of hacky soldering so that the connection to the LED matrix is first routed to the transistors, which amplify the current before connecting back to the matrix.

# Software

The software is intended to be run on ESP32, it is untested for other boards. It uses hardware SPI to communicate with the matrix board. It operates by turning on each column of the matrix, one at a time. The desired rows for each column are then enabled. The LED's are then turned off and the cycle repeats with the next row. If done fast enough, the human eye will percieve the LED's as being constantly illuminated. One of the ESP32 hardware timers is used to control the cycle speed. With this method, brightness control can be achieved by not illuminating a row for some number of scan cycles.