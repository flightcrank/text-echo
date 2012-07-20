#TEXT ECHO

This is a simple SDL program that reads text from a given input file
and each time a ANY key is pressed will print out one character from that
file on the screen. It will display it in a bitmaped font, which can also
be changed

This program helps achieve an effect seen in TV and films. An actor will
often type fast on the keys and appear to have perfect input when really all 
there doing is mashing the keys. This program allows you to do this while the 
out put on the screen will mirror the text file

To compile you need the SDL library installed.
Also a text file for input and a bitmaped font which are both included.

`gcc text_echo.c -lSDL -o app`

## Controls
* ESC will exit the program
* any other key will echo one character from the text file to the screen

## Images
This is a sample image of the program outputting its own source code.
![screen shot](http://i.imgur.com/SCwI2.png)

