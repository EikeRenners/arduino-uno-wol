# Arduino UNO Wake On Lan (WOL) example

This is a tiny code snippet I use to wake my MSI Laptop via WOL magic packet. This notebook claims to be able to boot on ac power connected, but in truth it simply doesn't work.

In my current setup a hard wired LAN connection is neither an option, no way to run a cable "hidden" to the next switch.

Thus, the only way to automate boot on power is to put the notebook in hybernate instead of shutting it down entirely and then using an Arduino to send repeated WOL packages via short direct LAN connection when power is restored to both, notebook plus microcontroller.

## Hardware

- Arduino Uno
- W5500 Ethernet shield (or similar)
- Ethernet cable
- USB cable
- Jumpwires (if your adapter does not come as UNO compatible shield)

I wired my Arduino UNO following standard SPI setup using the pre-installed SPI header on the UNO. The only connection to add then was the CS pin, which I choose to put on PIN6 instead of PIN10 (default), since I had a connector already soldered there.

Connection Diagram on a standard 6 PIN SPI header:

```txt
<= USB Connector facing left 

  MISO   [o][o]    5V
  SCK    [o][o]    MOSI
  RST    [o][o]    GND
```

## Code

I used the Ethernet3 library by sstaub, also available over Platformio:

```ini
lib_deps:
  sstaub/Ethernet3@^1.5.5
```

The code itself is basically an adapted version of this code snipped found on [MEGUNOLINK.com](https://www.megunolink.com/download/libraries/wake-on-lan/).

I included a zip version of this library here too.
