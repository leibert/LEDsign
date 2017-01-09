# LED Information Sign
##Uses an LED display to provide MBTA bus and weather information

Uses a Datum bc632 time clock display, which, to add to confusion, is really a modified BetaBrite single-led LED display. Using a reversed engineered commandset, you can push information via serial to the Datum controller, which converts it into commands the BetaBrite can interpret. The Datum controller can be bypassed, but sending information straight to the BetaBrite seems to be more finicky.

**Bus Time**<br>
<img src="http://images.cwm.eml.cc/IOSstuff/bustime.jpg?variant=small" width="50%" height="50%">
<br><br>
**Weather Forecast**<br>
<img src="http://images.cwm.eml.cc/IOSstuff/weather.jpg?variant=small" width="40%" height="40%">


