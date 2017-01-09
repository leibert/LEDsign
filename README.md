# LED Information Sign
##Uses an LED display to provide MBTA bus and weather information

Uses a Datum bc632 time clock display, which, to add to confusion, is really a modified BetaBrite single-led LED display. Using a reversed engineered commandset, you can push information via serial to the Datum controller, which converts it into commands the BetaBrite can interpret. The Datum controller can be bypassed, but sending information straight to the BetaBrite seems to be more finicky.


![Bus Time](http://images.cwm.eml.cc/IOSstuff/bustime.jpg?variant=small =250x)
![Weather FX](http://images.cwm.eml.cc/IOSstuff/weather.jpg?variant=small =250x)

