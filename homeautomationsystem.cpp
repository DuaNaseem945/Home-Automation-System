#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <atomic>
#include <chrono>
#include <thread>

using namespace std;

// Function to format time
string formatTime(time_t time) {
    struct tm* tm_info = localtime(&time);
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    return string(buffer);
}

// Function to check if it is night (let's say night is between 7 PM and 6 AM)
bool isNightTime(struct tm* localTime) {
    int hour = localTime->tm_hour;
    return (hour >= 19 || hour < 6);
}

// Function to display greeting based on the current time
void displayGreeting(struct tm* localTime) {
    int hour = localTime->tm_hour;
    if (hour >= 4 && hour < 12) {
        cout << "Good Morning!" << endl;
    } else if (hour >= 12 && hour < 17) {
        cout << "Good Afternoon!" << endl;
    } else if (hour >= 17 && hour < 21) {
        cout << "Good Evening!" << endl;
    } else {
        cout << "Good Night!" << endl;
    }
}

// Base class for all devices
class Device {
protected:
    string name;
    bool isOn;

public:
    Device(string n) : name(n), isOn(false) {}
    virtual ~Device() {}

    virtual void turnOn() {
        isOn = true;
    }

    virtual void turnOff() {
        isOn = false;
    }

    virtual void getStatus() const {
        cout << name << " is " << (isOn ? "on" : "off") << "." << endl;
    }

    bool getIsOn() const { return isOn; }

    virtual char getType() const = 0; // Pure virtual function for device type

    virtual void setSetting(int setting) {
        cout << name << " does not support settings." << endl;
    }
};

// Derived class for Light
class Light : public Device {
private:
    int brightness;

public:
    Light(string n) : Device(n), brightness(50) {}

    char getType() const override {
        return 'B'; // 'B' for Bulb
    }

    void getStatus() const override {
        cout << "Light " << name << " is " << (isOn ? "on" : "off") << " with brightness " << brightness << "." << endl;
    }

    void setSetting(int setting) override {
        brightness = setting;
    }
};

// Derived class for Air Conditioner
class AirConditioner : public Device {
private:
    int temperature;

public:
    AirConditioner(string n) : Device(n), temperature(25) {} // Default temperature

    char getType() const override {
        return 'A'; // 'A' for AC
    }

    void getStatus() const override {
        cout << "AC " << name << " is " << (isOn ? "on" : "off") << " with temperature " << temperature << "°C." << endl;
    }

    void setSetting(int setting) override {
        temperature = setting;
    }

    void updateTemperature(int currentTemperature) {
        temperature = currentTemperature;
    }
};

// Derived class for TV
class TV : public Device {
private:
    int volume;

public:
    TV(string n) : Device(n), volume(50) {}

    char getType() const override {
        return 'T'; // 'T' for TV
    }

    void getStatus() const override {
        cout << "TV " << name << " is " << (isOn ? "on" : "off") << " with volume " << volume << "." << endl;
    }

    void setSetting(int setting) override {
        volume = setting;
    }
};

// Derived class for Clock
class Clock : public Device {
public:
    Clock(string n) : Device(n) {}

    char getType() const override {
        return 'C'; // 'C' for Clock
    }

    void getStatus() const override {
        auto now = chrono::system_clock::now();
        auto now_c = chrono::system_clock::to_time_t(now);
        cout << "Clock " << name << " shows the time: " << formatTime(now_c) << "." << endl;
    }
};

// Derived class for Fan
class Fan : public Device {
public:
    Fan(string n) : Device(n) {}

    char getType() const override {
        return 'F'; // 'F' for Fan
    }

    void getStatus() const override {
        cout << "Fan " << name << " is " << (isOn ? "on" : "off") << "." << endl;
    }
};

// Derived class for Oven
class Oven : public Device {
public:
    Oven(string n) : Device(n) {}

    char getType() const override {
        return 'O'; // 'O' for Oven
    }

    void getStatus() const override {
        cout << "Oven " << name << " is " << (isOn ? "on" : "off") << "." << endl;
    }
};

// Derived class for Washing Machine
class WashingMachine : public Device {
public:
    WashingMachine(string n) : Device(n) {}

    char getType() const override {
        return 'W'; // 'W' for Washing Machine
    }

    void getStatus() const override {
        cout << "Washing Machine " << name << " is " << (isOn ? "on" : "off") << "." << endl;
    }
};

// Controller class to manage all devices
class HomeAutomationController {
private:
    static const int maxDevices = 10;
    Device* devices[maxDevices];
    int deviceCount;

public:
    HomeAutomationController() : deviceCount(0) {}

    ~HomeAutomationController() {
        for (int i = 0; i < deviceCount; ++i) {
            delete devices[i];
        }
    }

    // Function template to add a device of any type
    template <typename T>
    void addDevice(string name) {
        if (deviceCount < maxDevices) {
            devices[deviceCount++] = new T(name);
        } else {
            cout << "Cannot add more devices, maximum limit reached." << endl;
        }
    }

    void getDeviceStatus() const {
        for (int i = 0; i < deviceCount; ++i) {
            devices[i]->getStatus();
        }
    }

    void setDeviceSetting(char deviceType, int setting) {
        for (int i = 0; i < deviceCount; ++i) {
            if (devices[i]->getType() == deviceType) {
                devices[i]->setSetting(setting);
            }
        }
    }

    // Function to handle dynamic behavior based on time and temperature
    void updateDevicesBasedOnTimeAndTemperature(struct tm* localTime, int currentTemperature) {
        if (isNightTime(localTime)) {
            for (int i = 0; i < deviceCount; ++i) {
                if (devices[i]->getType() == 'B') {
                    devices[i]->turnOn();
                }
            }
        } else {
            for (int i = 0; i < deviceCount; ++i) {
                if (devices[i]->getType() == 'B') {
                    devices[i]->turnOff();
                }
            }
        }

        for (int i = 0; i < deviceCount; ++i) {
            if (devices[i]->getType() == 'A') {
                AirConditioner* ac = dynamic_cast<AirConditioner*>(devices[i]);
                if (ac) {
                    ac->updateTemperature(currentTemperature);
                    if (currentTemperature > 40) {
                        ac->turnOn();
                    } else {
                        ac->turnOff();
                    }
                }
            }
        }

        // Additional time-based control logic for other devices
        int hour = localTime->tm_hour;
        int min = localTime->tm_min;
        int sec = localTime->tm_sec;

        for (int i = 0; i < deviceCount; ++i) {
            char type = devices[i]->getType();
            if (type == 'W' && hour == 8 && min == 0 && sec < 10) { // Washing machine at 8 AM
                devices[i]->turnOn();
            } else if (type == 'O' && hour == 7 && min == 0 && sec < 10) { // Oven at 7 AM
                devices[i]->turnOn();
            } else if (type == 'A' && hour == 12 && min == 0 && sec < 10) { // AC at 12 PM
                devices[i]->turnOn();
            } else if (type == 'F') { // Fan always on
                devices[i]->turnOn();
            } else if (type == 'B') { // Lights always on
                devices[i]->turnOn();
            }
        }
    }
};

void displayCurrentTime(time_t currentTime) {
    cout << "Current time: " << formatTime(currentTime) << endl;
}

void displayCurrentTemperature(int currentTemperature) {
    cout << "Current temperature: " << currentTemperature << "°C" << endl;
}

int main() {
    HomeAutomationController controller;

    // Adding devices using function template
    controller.addDevice<Light>("Garage Light");
    controller.addDevice<AirConditioner>("Living Room AC");
    controller.addDevice<TV>("Bedroom TV");
    controller.addDevice<Clock>("Wall Clock");
    controller.addDevice<Fan>("Living Room Fan");
    controller.addDevice<Oven>("Kitchen Oven");
    controller.addDevice<WashingMachine>("Laundry Washing Machine");

    atomic<bool> running(true);
    time_t currentTime = time(0);
    int currentTemperature = 25; // Default temperature

    while (running) {
        system("cls"); // clear the console on Windows, use "clear" for Linux/Mac
        displayCurrentTime(currentTime);
        displayCurrentTemperature(currentTemperature);
        struct tm* localTime = localtime(&currentTime);
        displayGreeting(localTime);
        controller.updateDevicesBasedOnTimeAndTemperature(localTime, currentTemperature);
        controller.getDeviceStatus();

        // Displaying the time change message
        cout << "Time after 10 seconds will be: " << formatTime(currentTime + 10) << endl;

        // Increment the current time by 10 seconds
        currentTime += 10;

        cout << "Enter new time (HH MM SS) or 'quit' to exit: ";
        string input;
        getline(cin, input);
        if (input == "quit") {
            running = false;
            break;
        } else {
            int hh, mm, ss;
            if (sscanf(input.c_str(), "%d %d %d", &hh, &mm, &ss) == 3) {
                localTime->tm_hour = hh;
                localTime->tm_min = mm;
                localTime->tm_sec = ss;
                currentTime = mktime(localTime);
            }
        }

        cout << "Enter new temperature or 'skip' to continue: ";
        getline(cin, input);
        if (input == "skip") {
            // Simulate waiting for the next iteration
            for (int i = 0; i < 10; ++i) {
                // Simulating a delay without this_thread::sleep_for
                for (volatile int j = 0; j < 100000000; ++j);
            }
            continue;
        } else {
            int temp;
            if (sscanf(input.c_str(), "%d", &temp) == 1) {
                currentTemperature = temp;
            }
        }

        // Simulate waiting for the next iteration
        for (int i = 0; i < 10; ++i) {
            // Simulating a delay without this_thread::sleep_for
            for (volatile int j = 0; j < 100000000; ++j);
        }
    }

    return 0;
}