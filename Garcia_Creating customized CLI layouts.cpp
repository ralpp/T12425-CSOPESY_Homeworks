#include <iostream>
#include <string>
#include <windows.h>

const int MAX_PROCESS_NAME_LENGTH = 40;

std::string truncateString(const std::string& str) {
    if (str.length() > MAX_PROCESS_NAME_LENGTH) {
        return str.substr(0, MAX_PROCESS_NAME_LENGTH - 3) + "...";
    } else {
        return str + std::string(MAX_PROCESS_NAME_LENGTH - str.length(), ' ');
    }
}

void setConsoleSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    
    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void printGpuSummary() {
    std::cout << "Fri Mar 29 18:42:38 2024\n";
    std::cout << "+------------------------------------------------------------------------------------+\n";
    std::cout << "| NVIDIA-SMI 551.86              Driver Version: 551.86       CUDA Version: 12.4     |\n";
    std::cout << "+--------------------------------------+----------------------+----------------------+\n";
    std::cout << "| GPU  Name                   TCC/WDDM | Bus-Id        Disp.A | Volatile Uncorr. ECC |\n";
    std::cout << "| Fan  Temp  Perf        Pwr:Usage/Cap |         Memory-Usage | GPU-Util Compute M.  |\n";
    std::cout << "|                                      |                      |              MIG M.  |\n";
    std::cout << "|======================================+=======-==============+======================|\n";
    std::cout << "|   0  NVIDIA GeForce GTX 1080 WDDM    |  00000000:26:00.0 On |                  N/A |\n";
    std::cout << "| 28%   37C    P8           11W / 180W |     701MiB / 8192MiB |      0%      Default |\n";
    std::cout << "|                                      |                      |                  N/A |\n";
    std::cout << "+-------------------------------+----------------------+-----------------------------+\n\n";
}

void printProcesses() {
    std::cout << "+------------------------------------------------------------------------------------+\n";
    std::cout << "| Processes:                                                                         |\n";
    std::cout << "|  GPU   GI   CI   PID   Type   Process name                              GPU Memory |\n";
    std::cout << "|        ID   ID                                                          Usage      |\n";
    std::cout << "+------------------------------------------------------------------------------------+\n";
    std::cout << "|    0   N/A  N/A  1368  C+G   " << truncateString("C:\\Windows\\System32\\dwm.exe") << "     N/A      |\n";
    std::cout << "|    0   N/A  N/A  2116  C+G   " << truncateString("...wekyb3d8bbwe\\vboxGameBarWidgets.exe") << "     N/A      |\n";
    std::cout << "|    0   N/A  N/A  4224  C+G   " << truncateString("...on123.0.2420.65\\msedgewebview2.exe") << "     N/A      |\n";
    std::cout << "|    0   N/A  N/A  5684  C+G   " << truncateString("C:\\Windows\\explorer.exe") << "     N/A      |\n";
    std::cout << "|    0   N/A  N/A  6676  C+G   " << truncateString("...tCBS_cw5n1h2txyewy\\SearchHost.exe") << "     N/A      |\n";
    std::cout << "+------------------------------------------------------------------------------------+\n";
}

int main() {
    setConsoleSize(120, 30); 

    std::string userInput;
            printGpuSummary(); 
            printProcesses(); 
    while (true) { 
        std::cout << "PS C:/Users/Administrator> ";
        std::getline(std::cin, userInput); 

        if (userInput == "nvidia-smi") {
            printGpuSummary(); 
            printProcesses();  
        } else if (userInput == "exit") {
            std::cout << "Exiting program." << std::endl;
            break;  
        } else {
            std::cout << "Command not recognized." << std::endl;
        }
    }

    return 0;
}
