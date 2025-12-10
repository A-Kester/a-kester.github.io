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
    <h5>Header files</h5>
<p style="margin-left: 40px;">[Explain code logic, how it works, why its designed that way]
     Doughbot's source code is mostly broken into a number of header files that contain functions relating to specific library. The only two exceptions to this are Helper.h, which contains helper functions for a variety of files, and Buttons.h, that mostly contains the ISR's and a function to detect specific button presses.
</p>
    <h5>State Machine</h5>
    <p>Explain state machine (im still cleaning it up!)</p>
    <h4>User Interface</h4>
    <p style="margin-left: 40px;">Users can interact with the system via the LCD screen and four buttons. Starting from the user's far left, the red button allows the user to access the system settings. Pressing the button again will return the user to their last screen. The settings are only accessible before a rise has been started. In the settings menu, the user can toggle the wireless connection*, alerts*, the elapsed rise time, a rise progress bar, and select timed or volume based rise, desired rise time**, target rise volume**, and scan frequency in multiples of 10 minutes** ('*' denotes incomplete options, '**' denotes a non boolean field). The next two buttons are the down (yellow) and up (blue) buttons, which allow the user to scroll through the menu, and increment and decrement the 3 non-boolean fields. These buttons currently have no effect outside of the settings menu. The final button is the select/start button. In the settings menu, this button will toggle boolean options and enter edit mode for non-boolean options. Outside of the settings menu, the start button allows the user to progress through the state machine. Aside from the settings menu, there is no way to return to a previous state.
</p>
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
    <p style="margin-left: 40px;"><b>3D Printed Components</b></p>
    <div class="parts">
        <img src="/images/img_4825.jpeg" alt="centering mechanism" width="375" height="450">
        <img src="/images/img_4955.jpeg" alt="feet" width="500" height="375">
    </div>
</div>

## Results

## Accessibility

<div id="accessibility">
    <p>Using our device requires the ability to adjust the stand and centering mechanism around the bowl, push buttons, and read the display. This current design may be difficult for people with limited use of their hands or vision problems.<br>To make our ES more accessible we would work to transmit the data displayed on the LCD to a web server where users can then use a screen reader to receive updates on the status of their dough. Other changes we could implement in the future to improve accessibility include designing a more simple case / stand for the device to be housed in and then attached to the user's bowl, including language settings, and considering challenges that might face those who are color-blind. One potential challenge for those blue-green colorblindness is that the "start" button is green and the "up" button is blue, possibly causing confusion if not labeled or explicitly identified otherwise.</p> 
</div>

## Ethical Implications

<div id="ethics">
    <p>Given that our device does not stream any data, we find few ethical issues. If data were to be streamed to a web server, this would introduce data privacy and security concerns. Furthermore, our device does not collect an personal information or store any data collected via the TOF or temperature/humidity sensors.</p>
</div>

## Schedule

<div id="schedule" class="container" style="margin-left: 40px; flex-direction: row;"> 
    <div id="original">
        <br>
        <h5><u>Original Schedule</u></h5>
        <table>
            <tr>
                <td>Oct. 24th</td>
                <td>Proposal Due<br>
                    <ul>
                        <li>Finalize materials</li>
                        <li>Find drivers for sensors</li>
                        <li>Finalize device design</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 3rd</td>
                <td>
                    <ul>
                        <li>Connect sensors and read data</li>
                        <li>Start writing bowl scanning logic</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 10th</td>
                <td>
                    <ul>
                        <li>Motor assembly</li>
                        <li>Integrate sensors</li>
                        <li>Make adjustments to 3D prints</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 17th</td>
                <td>
                    <ul>
                        <li>Measure volume</li>
                        <li>Assemble stand</li>
                    </ul>
                </td>
            </tr>
            <tr>
                <td>Week of Nov 24th (Thanksgiving)</td>
                <td>
                    <ul>
                        <li>User interface implementation</li>
                        <li>Optional software feature / dough prediction</li>
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
                <td>
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

<div id="issues">
    <p>Over the course of the project we encounted a number of issues which are ennumerated below with our corresponding resolutions.</p>
    <table style="margin-left: 40px;">
        <tr style="font-weight: bold;">
            <td>Issue</td>
            <td>Resolution</td>
        </tr>
        <tr>
            <td>Our 3D prints were designed with no tolerances so pieces did not fit together.</td>
            <td>We reprinted some pieces. Others were shaved down with a knife, drill, or sandpaper.</td>
        </tr>
        <tr>
            <td>The breadboard we ordered was half-sized and didn't comfortable fit all our components.</td>
            <td>We are borrowing a full-size breadboard from Professor Vaccari.</td>
        </tr>
        <tr>
            <td>The motor was slipping not allowing the gear to effectively push the rack holding the TOF sensor.</td>
            <td>We super glued the gear to the motor.</td>
        </tr>
        <tr>
            <td>The TOF sensor would not run on the same bus as the motor.</td>
            <td>The TOF sensor is now getting power and ground from a different source, alleviating the issue.</td>
        </tr>
        <tr>
            <td>The LCD requires 5V which we had not accounted for initially.</td>
            <td>We were able to power it up with 5V but drive it from the logic pins of the ESP32.</td>
        </tr>
    </table>
</div>

## Future Work

## References

<p class="footnote" id="footnote-1"> [1] “Dough Processing Sensors | KEYENCE America,” accessed December 7, 2025, https://www.keyence.com/products/measure/industries/food-and-beverage/dough-processing-sensors.jsp.</p>
<p class="footnote" id="footnote-2"> [2] “Crustello: Smart AI Dough & Sourdough Tracker,” Kickstarter, March 1, 2026, https://www.kickstarter.com/projects/racebox/crustello.</p>
