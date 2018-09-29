# AgruminoAt

The AT commands

The first implementation of a set of AT commands (AT stands for "Attention") dates back to 1981, when Dennis Hayes and Dale Heatherington released their Hayes Smartmodem 300, which was able to operate in two different ways, "date mode "and" command mode "and communicated via an RS-232 serial port.
To go into "command mode" it was necessary to send the modem a "+++" escape sequence, after which the data sent through the serial line were interpreted by the modem as commands for operations such as making a call, closing a communication, setting the connection parameters and so on.
The Hayes command set, hereinafter referred to as "AT" for copiright reasons, consists of a series of short text strings, which are combined to produce the desired operations, and which can be divided into four groups of commands:

    a) basic commands
    b) extended set of commands
    c) owner set of commands
    d) register commands
    
The speed with which we passed from the Smartmodem model 300 to 1200 and then to 2400 caused Hayes's decision not to change the programming system based on the set of AT commands, effectively opening the road to standardization. In fact, given the success of Hayes modems, the competition also equipped its modems with a set of "compatible" commands.
Due to the ease of use and the low computational resources required, a set of AT commands, specially made, is ideal for configuring IOT devices.
