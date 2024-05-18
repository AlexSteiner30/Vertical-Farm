# Vertical Farm System - README

## Introduction
Welcome to the Vertical Farm System project! This system integrates Express.js, EJS, Mongoose, Arduino, and ESP32 Camera to create an efficient and user-friendly vertical farming solution. The system allows for real-time monitoring and control of multiple farms via a web interface, making it scalable and adaptable for various users.

## Table of Contents
1. [Features](#features)
2. [System Architecture](#system-architecture)
3. [Installation](#installation)
4. [Configuration](#configuration)
5. [Usage](#usage)
6. [Troubleshooting](#troubleshooting)
7. [Future Improvements](#future-improvements)

## Features
- Real-time monitoring of sensor data from multiple farms.
- Control of water and fan systems based on sensor data.
- ESP32 Camera integration for visual monitoring.
- Dynamic addition of new farms to the system.
- Data visualization with graphs for sensor values and pie charts for plant types.
- Configurable farm settings through a web interface.

## System Architecture
1. **ESP32 Camera**: Captures real-time images and streams them to the web server.
2. **Arduino**: Handles sensor data collection and relays control commands.
3. **Express.js**: Serves the web application and handles API requests.
4. **EJS**: Templating engine for rendering dynamic web pages.
5. **Mongoose**: Manages MongoDB database interactions.
6. **MongoDB**: Stores farm data, sensor values, and configuration settings.

## Installation
To set up the Vertical Farm System, follow these steps:

1. **Clone the Repository**
   ```bash
   git clone https://github.com/AlexSteiner/vertical-farm.git
   cd vertical-farm/Server
   ```

2. **Install Dependencies**
   ```bash
   npm install mongoose ping body-parser express
   ```

3. **Configure MongoDB**
   Ensure MongoDB is installed and running on your machine or use a cloud MongoDB service. Update the connection string in `Server/index.js`.

4. **Upload Code to ESP32 and Arduino**
   - Navigate to the `VerticalFarm/` directories to upload the necessary code to the Arduino.
   - Navigate te the  `CameraWebServer/` directories to upload the necessary code to the ESP32 Camera.

## Configuration
1. **Network Configuration**
   - Ensure your ESP32 and Arduino are connected to the same network as the web server.
   - If required, configure port forwarding for external access to the ESP32 Camera.

## Usage
1. **Start the Server**
   ```bash
   node server
   ```

2. **Access the Web Interface**
   - Open your browser and navigate to `http://localhost:4000` (or the configured port).
   - The index page displays active farms, power usage, recent activities, and sensor data graphs.

3. **Manage Farms**
   - Use the sidebar to navigate between different farms.
   - Each farm page displays real-time sensor values, camera feed, and control options for the water and fan systems.
   - Modify farm settings using the provided forms and save changes to update the database and devices in real-time.

## Troubleshooting
- **ESP32 Camera Not Accessible**: Ensure the camera is connected to the same network and check port forwarding settings if accessing externally.
- **Sensor Data Not Updating**: Verify connections between the Arduino and sensors, and ensure the web server is running.
- **Database Connection Issues**: Check MongoDB service status and ensure the connection string in `config/db.js` is correct.

## Future Improvements
- Implement authentication and user roles for better security.
- Add more sensor types and expand the control features.
- Enhance the user interface with more detailed analytics and visualizations.
- Improve scalability to handle a larger number of farms and sensors.

## Conclusion
This Vertical Farm System provides a comprehensive solution for managing and monitoring vertical farms. With real-time data, camera integration, and dynamic control features, it aims to make farming more efficient and accessible. For any questions or contributions, please refer to the GitHub repository or contact the project maintainer.

---

Thank you for using the Vertical Farm System. Happy farming!

**Kind Regards,**
Alex Steiner

---

## Demonstration Video

https://github.com/AlexSteiner30/Vertical-Farm/assets/65250433/973e4426-5b04-41d2-abb3-793950017274

