C++ Program Integration
To integrate the microservice into your C++ program, follow these steps:

Include the necessary headers in your C++ program
#include <iostream>
#include <wininet.h>
#include <string>
#pragma comment(lib, "wininet.lib")

example function shown in main.cpp
always send it with appropiate data, and be sure to stringify it like so.
std::string ammoData = "{\"action\": \"ammo_change\", \"value\": " + std::to_string(newAmmo) + "}";
SendDataToMicroservice(ammoData);

Microservice Functionality
The microservice provides the following functionality:

It listens for POST requests on the /log endpoint.
It expects the request data to be in JSON format with the following structure:
Based on the received action, it logs a message indicating the change in ammo or health.
It returns a JSON response with a success message.


The microservice logs the received data by printing it to the console. You can modify the log_data function in app.py to perform any desired logging operations, such as writing to a file or sending the data to another system.

Notes
Make sure the microservice is running before executing your C++ program.
The microservice runs on localhost with port 5000. If you need to change the hostname or port, update the InternetConnect function in the C++ code accordingly.

![image](https://github.com/BlainCope/ACExternal/assets/90635596/779db6b7-6566-4707-a711-7b68a95421a5)

