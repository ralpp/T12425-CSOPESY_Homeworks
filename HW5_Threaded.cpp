#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <string>
#include <windows.h> 
#include <conio.h>   

// Interface for Keyboard Events
class IKeyboardEvent {
public:
    virtual void OnKeyDown(char key) = 0;
    virtual void OnKeyUp(char key) = 0;
};

// Implementation of IKeyboardEvent
class KeyboardEventHandler : public IKeyboardEvent {
public:
    void OnKeyDown(char key) override {

    }

    void OnKeyUp(char key) override {

    }
};

class BouncingMarquee {
public:
    // Add refreshRate and pollingRate in milliseconds
    BouncingMarquee(const std::string& message, int refreshRateMs = 100, int pollingRateMs = 100)
        : message(message), running(true), x(0), y(3), dirX(1), dirY(1), command(""),
        refreshRate(refreshRateMs), pollingRate(pollingRateMs) {
        // Get console dimensions
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1; // Width of the console window
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1; // Height of the console window
        y = height / 2; // Start in the middle of the screen
    }

    void start() {
        thread = std::thread(&BouncingMarquee::run, this);
    }

    void stop() {
        running = false;
        if (thread.joinable()) {
            thread.join();
        }
    }

private:
    std::string message;
    std::string command; 
    int width, height;
    int x;           
    int y;           
    int dirX;       
    int dirY;       
    std::atomic<bool> running;
    std::thread thread;
    KeyboardEventHandler keyboardHandler; // Keyboard event handler

    int refreshRate;  
    int pollingRate;  

    void printStaticText() {
        // Print the static top text
        setCursorPosition(0, 0);
        std::cout << "*********************************************************" << std::endl;
        setCursorPosition(0, 1);
        std::cout << "*               Displaying a marquee console!           *" << std::endl;
        setCursorPosition(0, 2);
        std::cout << "*********************************************************" << std::endl;

        // Print the static command footer text
        setCursorPosition(0, height - 2);
        std::cout << "Enter a command for MARQUEE CONSOLE: " << std::flush; // Static footer
    }

    void setCursorPosition(int x, int y) {
        COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void run() {
        printStaticText();
        while (running) {
            // Clear the current position of the marquee line
            setCursorPosition(x, y);
            std::cout << std::string(message.length(), ' ') << std::flush; // Clear the message line

            // Update horizontal and vertical position
            x += dirX;
            y += dirY;

            if (x + message.length() >= width) {
                x = width - message.length();
                dirX = -1; // Change direction 
            }
            else if (x < 0) {
                x = 0;
                dirX = 1; // Change direction 
            }
        
            if (y >= height - 3) { 
                y = height - 3;
                dirY = -1; // Change direction 
            }
            else if (y <= 3) { 
                dirY = 1; // Change direction 
            }

            // Set cursor position and print the message
            setCursorPosition(x, y);
            std::cout << message << std::flush;

            handleKeyboardInput();

            // Adjust the sleep time based on refresh rate
            std::this_thread::sleep_for(std::chrono::milliseconds(refreshRate));
        }
    }

    void handleKeyboardInput() {
        // Polling for keyboard input
        std::this_thread::sleep_for(std::chrono::milliseconds(pollingRate)); // Polling rate

        // Check if a key has been pressed
        if (_kbhit()) { // Check if a key is pressed
            char key = _getch(); // Get the pressed key
            if (key == 13) { // Enter key
                processCommand(command);
                command.clear(); // Clear command after processing
                clearCommandInput(); // Clear command input line on Enter
            }
            else if (key == 8) { 
                if (!command.empty()) {
                    command.pop_back(); 
                    clearCommandInput(); 
                }
            }
            else if (key >= 32 && key <= 126) {
                command += key; 
            }


            keyboardHandler.OnKeyDown(key);
            keyboardHandler.OnKeyUp(key);
        }

        setCursorPosition(0, height - 1); 
        std::cout << "Enter a command for MARQUEE CONSOLE: " << command << std::flush; 
    }

    void processCommand(const std::string& command) {
        // Move cursor to the command display area
        setCursorPosition(0, height - 2);
        std::cout << "Command processed in MARQUEE_CONSOLE: " << command << std::string(30, ' ') << std::flush; // Display the command
    }

    void clearCommandInput() {
        setCursorPosition(0, height - 1); 
        std::cout << "Enter a command for MARQUEE CONSOLE: " << std::string(50, ' ') << std::flush; 
    }
};

int main() {
    BouncingMarquee marquee("Hello World in marquee!", 30, 50); //(refresh,polling)
    marquee.start();


    try {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (...) {
        marquee.stop();
    }

    return 0;
}
