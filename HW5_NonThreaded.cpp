#include <iostream>
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
    BouncingMarquee(const std::string& message, int animationRate = 100, int pollingRate = 50)
        : message(message), running(true), x(0), y(3), dirX(1), dirY(1), command(""),
        animationRate(animationRate), pollingRate(pollingRate) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        y = height / 2;
    }

    void start() {
        run();
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
    KeyboardEventHandler keyboardHandler;
    int animationRate;
    int pollingRate;

    // Prints static text at the top and bottom of the console
    void printStaticText() {
        setCursorPosition(0, 0);
        std::cout << "*********************************************************" << std::endl;
        setCursorPosition(0, 1);
        std::cout << "*               Displaying a marquee console!           *" << std::endl;
        setCursorPosition(0, 2);
        std::cout << "*********************************************************" << std::endl;

        setCursorPosition(0, height - 2);
        std::cout << "Enter a command for MARQUEE CONSOLE: " << std::flush;
    }

    // Sets cursor to the specified position in the console
    void setCursorPosition(int x, int y) {
        COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }


    // Main loop to run the marquee animation and handle input
    void run() {
        printStaticText();
        auto lastAnimationTime = std::chrono::steady_clock::now();
        auto lastPollingTime = std::chrono::steady_clock::now();

        while (running) {
            auto now = std::chrono::steady_clock::now();

            // Check if it's time to handle keyboard input based on polling rate
            if (now - lastPollingTime >= std::chrono::milliseconds(pollingRate)) {
                handleKeyboardInput();  // Handle any input from the user
                lastPollingTime = now;
            }

            // Check if it's time to update the marquee's position based on the animation rate
            if (now - lastAnimationTime >= std::chrono::milliseconds(animationRate)) {
                // Clear the previous position of the marquee
                setCursorPosition(x, y);
                std::cout << std::string(message.length(), ' ') << std::flush;

                // Update position (left or right)
                x += dirX;

                // Bounce off the right edge of the console
                if (x + message.length() >= width) {
                    x = width - message.length();  //adjust position to stay within the console's bounds
                    dirX = -1;  //change direction
                }
                // Bounce off the left edge of the console
                else if (x < 0) {
                    x = 0;  //adjust position
                    dirX = 1;  //change direction
                }

                // Update the vertical position (move up or down)
                y += dirY;

                // Bounce off the bottom edge
                if (y >= height - 3) {  //avoid overlapping with the footer
                    y = height - 3;  //adjust position
                    dirY = -1;  //change direction
                }
                // Bounce off the top edge 
                else if (y <= 3) {  //avoid overlapping with the header
                    y = 3;  //adjust position
                    dirY = 1;  //change direction
                }

                // Print the marquee at its new position
                setCursorPosition(x, y);
                std::cout << message << std::flush;

                lastAnimationTime = now;  // Update animation time
            }
        }
    }

    // Handles keyboard input and processes commands
    void handleKeyboardInput() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 13) {
                processCommand(command);
                command.clear();
                clearCommandInput();
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

    // Processes a given command
    void processCommand(const std::string& command) {
        setCursorPosition(0, height - 2);

        if (command == "exit") {
            std::cout << "Exiting MARQUEE_CONSOLE..." << std::flush;
            running = false;
        }
        else {
            std::cout << "Command processed in MARQUEE_CONSOLE: " << command << std::string(30, ' ') << std::flush;
        }
    }

    // Clears the command input area
    void clearCommandInput() {
        setCursorPosition(0, height - 1);
        std::cout << "Enter a command for MARQUEE CONSOLE: " << std::string(50, ' ') << std::flush;
    }
};

int main() {
    BouncingMarquee marquee("Hello World in marquee!", 50, 10); //refresh rate, polling rate
    marquee.start();

    return 0;
}
