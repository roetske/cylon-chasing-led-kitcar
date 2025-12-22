# cylon-chasing-led-knightrider chasing led
scripts for running cylon effect with ledstrips <br/>
sk6812 was a nightmare to get settings right grbw was the setting jeezes <br/>
Had this strip laying around and did test, problem is spacing between the leds for my strip 25mm too big. <br/>
Fastled did not get working for sk6812. <br/>
Adafruit neopixel worked ;). 
Worked also with buffer for com levelshifter 3.3v to 5v. sn74ahct125n <br/>
Nonused ports sn74 to gnd an notenable to 5v.<br/>
Common mistake seperate powersupply for led and controller do not forget <br/>
to connect the grounds together when using 2 seperate powersupplies.<br/>
Ledstripcode nmbr of leds ,lenght scan and tail setting in percentage  for 144led/m ws1812 grb with tail which you can set. <br/>
