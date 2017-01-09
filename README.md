# LED Information Sign
##Uses an LED display to provide MBTA bus and weather information

[**Video of it in action**](https://photos.google.com/share/AF1QipPsXPlnKKcPZztPROOYYdfCfxQ39UM2ShO9sEnP8gY_l7avnuxHHDS1XVPucIt-kw/photo/AF1QipOHJ4UbWXHaReUzg_OA25McMiT7YUTq3ufKjc-5?key=TFFDVHZpdDFxeERhZ2lfOWNFT3FLZmFOMks0Rkln)


Uses a Datum bc632 time clock display that I found at an MIT Swapfest. It's really a modified BetaBrite single-led LED display. Using a reversed engineered commandset, you can push information via serial to the Datum controller, which converts it into commands the BetaBrite can interpret. The Datum controller can be bypassed, but sending information straight to the BetaBrite seems to be more finicky.

**Bus Time**<br>
<img src="http://images.cwm.eml.cc/IOSstuff/bustime.jpg?variant=small" width="50%" height="50%">
<br><br>
**Weather Forecast**<br>
<img src="http://images.cwm.eml.cc/IOSstuff/weather.jpg?variant=small" width="40%" height="40%">


