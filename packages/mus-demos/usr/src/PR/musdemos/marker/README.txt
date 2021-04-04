This demo illustrates how to start songs from markers defined in the song file and
how to attach a marker callback function.

Once executed, use button A to start the song from a marker (the marker the song
will start from is incremented).  Use button B to stop the song.

When markers are encountered by the music library, a callback function will be 
called, which will change the background color of the screen.  The screen will
change according to the marker number passed back to the callback function.

        Marker          Color
           0            Gray 
           1            Red
           2            Green
           3            Blue

The background color is reset to black when the song is restarted. 
