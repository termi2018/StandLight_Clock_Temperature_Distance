#include <iostream>
#include <wiringPi.h>
#include "Button.h"
#include "Led.h"
#include "Listener.h"
#include "Controller.h"
#include "View.h"
#include "Service.h"
#include "LCD.h"
#include "ClockView.h"
#include "ClockService.h"
#include "ClockCheck.h"
#include "I2C.h"
#include "DHT11.h"
#include "UltraSonic.h"
#include "TempHumidService.h"
#include "TempHumidView.h"
#include "Motor.h"

int main()
{
    std::cout << "Hello World!" << std::endl;

    Button modeButton(27);
    Button powerButton(28);
    Button fanOnOffButton(29);

    ClockCheck clockCheck;
    Led led1(21);
    Led led2(22);
    Led led3(23);
    Led led4(24);
    Led led5(25);
    DHT11 dht(7);
    UltraSonic ultraSonic(5, 4);
    Motor motor(26);
    LCD lcd(new I2C("/dev/i2c-1", 0x27));
    View view(&led1, &led2, &led3, &led4, &led5, &lcd, &motor);
    TempHumidView tempHumidView(&lcd, &motor, &view);
    ClockView clockView(&lcd);
    Service service(&view);
    ClockService clockSerivce(&clockView);
    TempHumidService tempHumidService(&tempHumidView);
    Controller control(&service, &clockSerivce, &tempHumidService);
    Listener listener(&modeButton, &powerButton, &fanOnOffButton, &control, &clockCheck, &dht, &ultraSonic);
    
    while (1)
    {
        listener.checkEvent();
        view.lightView();
        view.motorView();
        delay(10);
    }

    return 0;
}