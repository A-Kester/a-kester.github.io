<link rel="stylesheet" href="style.css">

# Dough Bot
### Anna Kester & Grayson Wade

## Introduction
<div id="intro">
    <p>DoughBot was born out of a problem that troubles many bakers, specifically those tasked with letting dough rise. It can be difficult to visually determine when dough in a standard concave bowl has doubled in size, fundamentally undermining the precise nature of baking. Thus, we set forth to develop an embedded system that will sense when the dough has doubled in size and alert the baker of its status.</p>
    <p>Existing solutions are almost exclusively for commercial baking applications <a href="#footnote-1"><sup>[1]</sup></a>, or one home baking <em>smart</em> bowl "Crustello" has yet to be become publically available<a href="#footnote-2"><sup>[2]</sup></a>.
    After a semester of learning about embedded systems and how MCUs interface with sensors to collect data, we chose to implement our project using a Time of Flight (TOF) sensor, stepper motor, and LCD screen. </p>
</div>

## Methods
<div id="methods">
    <h4>Devices & Peripherals</h4>
    <p style="margin-left: 40px;"><b>Adafruit HUZZAH32 - ESP32 Feather</b><br>We chose to use the ESP32 because of its small size, integration with FeatherWings, and options for WiFi capability.<br>
    <b>DC Motor + Stepper FeatherWing Add-on</b><br>This FeatherWing was the recommended Feather motor controller / motor shield for our stepper motor. It is extremely light and handles all controls via I2C. Additionally, the pre-existing Arduino software library is an added benefit for ease of use.<br>
    <b>Mini Stepper Motor</b><br>We chose the mini stepper motor because of its small size, which ideal due to the size and weight contrained nature of the design of the physical stand supporting all components<br>
    <b>Adafruit VL53L0X Time of Flight Distance Sensor</b><br>The VL53L0X has a ~30-1000mm range which is ideal for the scope of our project and well within the height range of a typical bowl used for allowing dough to rise. Communication is also done over I2C and there is an Arduino wrapper library for easy use with the ESP32 or other Arduino boards.<br>
    <b>Standard LCD 20x4</b><br>This standard white text on blue background LCD was chosen to display dough progress as well as other pertinent information. This LCD is the same as the one we used in class, so we knew communication with the display would be straightforward and in line with our needs for the project.<br>
    <b>DHT20 - AHT20 Temperature and Humidity Sensor</b><br>This sensor is again a peripheral we tested in class and found suitable for our purposes. The simple I2C communication allows us to easily measure the temperature and humidity where the dough is rising – important metrics for devoted sourdough bakers.<br></p>
    <h4>Libraries</h4>
    <p style="margin-left: 40px;"><b>Adafruit_VL53L0X</b><br>This library is the sensor driver that enables reading data from the VL53L0X TOF sensor. It is simple and abstracted for easy use.<br>
    <b>Adafruit_MotorShield</b><br>This library enables motor controls, communicating the number of steps and the port to the stepper motor.<br>
    <b>Adafruit_AHTX0</b><br>This library enables communication and reading from the temperature and humidity sensor.<br>
    <b>LiquidCrystal</b><br>This built-in library handles communication and writing to the LCD.<br>
    <b>math</b><br>This library is also built-in to handle our volume computations using pi.<br></p>
    <h4>Code Logic</h4>
    <h4>User Interface</h4>
    <h4>BOM</h4>
    <table style="margin-left: 40px;">
        <tr style="font-weight: bold;">
            <td>Part</td>
            <td>Price</td>
            <td>Vendor</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/3317">Adafruit VL53L0X Time of Flight Distance</a></td>
            <td>$14.95</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/5396?srsltid=AfmBOopMHsuxksPDmVNcoLZCWIrgcbpCehMNrM5qmUyA9YXdC5VhLQc5">Adafruit VL53L4CD Time of Flight Distance Sensor - ~1 to 1300mm - STEMMA QT / Qwiic</a></td>
            <td>$14.95</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/4411">Mini Stepper Motor - 200 Steps - 20x30mm NEMA-8 Size</a></td>
            <td>$19.95</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/2927">DC Motor + Stepper FeatherWing Add-on For All Feather Boards</a></td>
            <td>$19.95</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/5183">DHT20 - AHT20 Pin Module - I2C Temperature and Humidity Sensor</a></td>
            <td>$4.50</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/198">Standard LCD 20x4 + extras - white on blue</a></td>
            <td>$17.95</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/1010">Colorful 12mm Square Tactile Button Switch Assortment - 15 pack</a></td>
            <td>$5.95</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/64">Half Sized Premium Breadboard - 400 Tie Points</a></td>
            <td>$4.95</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/5837">Premium Colorful Silicone Extension Jumper Wires - 200mm x 30 pc - Multi-Colored</a></td>
            <td>$9.95</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/5801">Vertical Wall Power Supply with USB C - 5V 3A Output and Switch</a></td>
            <td>$5.95</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/3972">USB Micro B Male Plug to 5-pin Terminal Block</a></td>
            <td>$6.50</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td><a href="https://www.adafruit.com/product/239">Full Sized Premium Breadboard - 830 Tie Points</a></td>
            <td>From Vaccari</td>
            <td>Adafruit</td>
        </tr>
        <tr>
            <td>PLA Filament for 3D Printed Components</a></td>
            <td>From Makerspace</td>
            <td>?</td>
        </tr>
        <tr style="font-weight: bold;">
            <td>Total</td>
            <td>$125.55</td>
            <td> </td>
        </tr>
    </table>
</div>

## Result

## Accesibility

## Ethical Implications

## Schedule
<div id="schedule" class="container" style="margin-left: 40px; flex-direction: row;"> 
    <div id="original">
        <br>
        <h5><u>Original Schedule</u></h5>
        <table>
            <tr>
                <td>Oct. 24th</td>
                <td>Proposal Due
                    <ul>
                        <li>Finalize materials</li>
                        <li>Find drivers for sensors</li>
                        <li>Finalize functional design</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 3rd</td>
                <td>
                    <ul>
                        <li>Parts arrive?</li>
                        <li>Have software drivers ready to go</li>
                        <li>Test components (individual)</li>
                        <li>Rough stand design</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 10th</td>
                <td>
                    <ul>
                        <li>Integrate sensors</li>
                        <li>Write code?</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 17th</td>
                <td>
                    <ul>
                        <li>Integrate stand</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 24th (Thanksgiving)</td>
                <td>NO HARDWARE
                    <ul>
                        <li>Bug fixes</li>
                        <li>WDT?</li>
                        <li>Wireless?</li>
                        <li>Optional software feature</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Dec 1st</td>
                <td>
                    <ul>
                        <li>Finalize integration</li>
                        <li>Work on presentation and website</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Finals Week</td>
                <td>
                    <ul>
                        <li>Finish</li>
                    </ul>
                </td>
            </tr>
        </table>
    </div>
    <div id="actual">
        <br>
        <h5><u>Actual Schedule</u></h5>
        <table>
            <tr>
                <td>Oct. 24th</td>
                <td>Proposal Due
                    <ul>
                        <li>Finalized materials</li>
                        <li>?? Find drivers for sensors</li>
                        <li>Finalized functional design</li>
                        <li>Began 3D printing</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 3rd</td>
                <td>
                    <ul>
                        <li>Have software drivers ready to go</li>
                        <li>Test components (individual)</li>
                        <li>Rough stand design</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 10th</td>
                <td>
                    <ul>
                        <li>Finished 3D prints</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 17th</td>
                <td>
                    <ul>
                        <li>Started writing base code</li>
                        <li>Started testing sensors</li>
                        <li>Testing motor</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 24th (Thanksgiving)</td>
                <td>NO HARDWARE
                    <ul>
                        <li>Wrote volume calculation code</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Dec 1st</td>
                <td>
                    <ul>
                        <li>Assembled stand and centering mount</li>
                        <li>Started website</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Finals Week</td>
                <td>
                    <ul>
                        <li>Finish</li>
                    </ul>
                </td>
            </tr>
        </table>
    </div>
</div>

## Issues

## Future Work

## References
<p class="footnote" id="footnote-1"> [1] “Dough Processing Sensors | KEYENCE America,” accessed December 7, 2025, https://www.keyence.com/products/measure/industries/food-and-beverage/dough-processing-sensors.jsp.</p>
<p class="footnote" id="footnote-2"> [2] “Crustello: Smart AI Dough & Sourdough Tracker,” Kickstarter, March 1, 2026, https://www.kickstarter.com/projects/racebox/crustello.</p>