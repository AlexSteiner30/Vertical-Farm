# Vertical Farm System - README

## Introduction
Welcome to the Vertical Farm System project! This system integrates Express.js, EJS, Mongoose, Arduino, and ESP32 Camera to create an efficient and user-friendly vertical farming solution. The system allows for real-time monitoring and control of multiple farms via a web interface, making it scalable and adaptable for various users.

## Table of Contents
1. [Features](#features)
2. [System Architecture](#system-architecture)
3. [Installation](#installation)
5. [Configuration](#configuration)
6. [Usage](#usage)
7. [Troubleshooting](#troubleshooting)
8. [Future Improvements](#future-improvements)

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

4. **Sketch**
   The following table provides a detailed list of sensors and components needed for our project. It includes the name of each sensor, the quantity required, and convenient links to 
   purchase each item. This ensures you have all the necessary parts to complete the project.
   
   | **Name of Sensor**                            | **Amount** | **Price** | **Purchase Link** |
   |-----------------------------------------------|------------|-----------|-------------------|
   | Air Temperature and Humidity Sensor (DHT11)   | 1          | €5.90     | [Buy Here](https://www.amazon.it/POPESQ%C2%AE-Digitale-Sensore-temperatura-A1770/dp/B0CHBKYSM1/ref=sr_1_5?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=1SHO9O42M3OJL&dib=eyJ2IjoiMSJ9.R8AJd90bDoAIO-Gv3qrujhOJpfeV46XFFr0_pIum8p-5h7XImInBxFvbwBHMIzv5AEsMnnZOn_voq03MkgLeR_JGKArpORVb02DYEzQolNys4W5n11FBdgz9lxb7Y1xwDmiqhwwnj8BP8Qd_gHkn26HJYcjeNzNVhc9QUWeGfYmv7EmRn0hW12GhdhoEnQO6iB5t_bVeEMrxReeqZU-f4A8yw0TW7sQcx3NK9IActSk0RQG1gDgg-9WuGMNcV6jrgXGGWR2MBY3DSevrrDOyTyU3zdawMYcNCM3954VzCUY.LP8cfNoMxKprI_aaTJd7EGkVEbHfOe9W-A8hTNUsqhM&dib_tag=se&keywords=DHT11&qid=1716083273&sprefix=dht11%2Caps%2C81&sr=8-5)           |
   | Soil Humidity Sensor                          | 2          | €5.98     |[Buy Here](https://www.amazon.it/TecnoStore%C2%AE-SENSORE-UMIDITA-TERRENO-IGROMETRO/dp/B01N5W2ZAL/ref=sr_1_4?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2QHOWW55422L4&dib=eyJ2IjoiMSJ9.pWyu4H7I-ZjdH7o1Owv9a9xC--43BKiFZ21VJnhXsSBnAL9TNMBYXPslwVV3sRGJigjkgak5p8MqRvi54fOJFrrbQr-_Cbw9yZF_jBhBHHatwrH2a4BzvGHCwuzs-GEZ.-8FcxfdiVzeTXKupUDvS-wzIaucqQWCvgqoVFhEcan4&dib_tag=se&keywords=solid+humidity+sensor+arduino&qid=1716083308&sprefix=solid+humidity+sensor+arduino%2Caps%2C109&sr=8-4)           |
   | Relay                                         | 2          | €8.49     | [Buy Here](https://www.amazon.it/Hailege-Channel-Module-Isolation-Trigger/dp/B07XY2C5M5/ref=sr_1_5?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dib=eyJ2IjoiMSJ9.U57up9CjoN9NjiytJQNnSuCbIu5MqLVrUbFrf2hWUVgXZk-nJah4MV3JEHF1x-UxXCzfR9tZQmdSDpLrJe0iQEKMfm2gDpjv6Hm1iSVg2Ews_pOExa0eB3JrIwjrgKsns9zRxZwMK8-jDZU3XhW0khb8DOdDrpCT_rzjBDG3cBOBvnthvDRlMJXumbzo4Z_lj23LDEocEjCde2j12783hePP2brTS9r4FcwuMROYtyIlXB1fR1rBf1g_cPMZ28uR2MJrHEk22FS6BhbiH3-k4slI_Vtrdji_uxWPBdntuf4.Ki6tqBc9EHbeF17srPICqOs2jk4Lkz9lb3mO_6_MUQk&dib_tag=se&keywords=arduino+relay&qid=1716083818&sr=8-5)           |
   | ESP32 Camera                                  | 1          | €11.92     | [Buy Here](https://www.amazon.it/sspa/click?ie=UTF8&spc=MTo3MjEwMzc3MDkxNzg4NzY1OjE3MTYwODMzNDM6c3BfYXRmOjMwMDExODcxNzk0OTAzMjo6MDo6&url=%2FQutsvosh-Scheda-sviluppo-seriale-Bluetooth%2Fdp%2FB0CKHPQ9CZ%2Fref%3Dsr_1_4_sspa%3Fcrid%3D3M6SLF4EF8T0O%26dib%3DeyJ2IjoiMSJ9.eTrrQ47vy4morXspKFcnGjDu3yAzQT6tmhqpXzAOxUN2HltWoCH9CeBLVrDjFjUO66ty2XjxWJJxr5eZXqjquGGvwm04JVXfhOCXvdRTAtoEi2__5JS_H9hE7AbE476zUnKzgR2Qv5cmNEsiXgneb1xdxuSZRS1Q39f9Dsd5h5R_FLAFA1cF726ga1RsHqgl9QgiUBe1M57Mhhxr2NAwrOeW-DlLSQ0ISN6UmT6QyUOCyt5iYHEzliXJAg7_lC1AJE-dsslHbjSLKq3mXY_Ikp7X2MpGc2cspN_63akjmwY.ziStskmn_IgKGLQ--bZKRYU85Yz1o6C3yeV4XOmBDr0%26dib_tag%3Dse%26keywords%3Desp32%2Bcam%26qid%3D1716083343%26sprefix%3Desp32%2Bc%252Caps%252C90%26sr%3D8-4-spons%26sp_csd%3Dd2lkZ2V0TmFtZT1zcF9hdGY%26psc%3D1)           |
   | FTDI Programmer                               | 1          | €5.59     | [Buy Here](https://www.amazon.it/Sconosciuto-Programmer-Arduino-Adapter-Konverter/dp/B014JXX4BK/ref=sr_1_4?crid=2NA12Z0SQ4XU7&dib=eyJ2IjoiMSJ9.BHJNk1nwX5yZN8K1geZPqymko4KT-MiQkSZ-GtaGiF-YUO8hmTKiUzvOLNJk95lqSBK77iwrNyFwDRoRnDGZk-WA8qwjzTOXSk1wcStpfTlPKHEe44hS4_Cvwqv9QJNHlx1h-Uw85wWuaI8C9JLC9x2JuBu8ncKfx575iFpR45gnazCKt0Ub8QGbLDDlpkyh4SE6MAuLASNXgcKSad4OrDx68YMqksG6C_V2deJXwblQEmPh0-_dxfQqFcKJP9qM0m1Pf-VvHJk9S6_51c-sBQP7mbxtNWMerMUJiqa7bjE.lqxLtcg2KBIaXcyNMzAQG7sMUnRkfojay7-CA57oGJI&dib_tag=se&keywords=ftdi+programmer&qid=1716083360&sprefix=ftdi+progra%2Caps%2C88&sr=8-4)           |
   | Arduino Uno R3                                | 1          | €27.99     | [Buy Here](https://www.amazon.it/Arduino-Uno-Rev3-Scheda-microcontrollore/dp/B008GRTSV6/ref=sr_1_1?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=3VG31H539P80U&dib=eyJ2IjoiMSJ9.XbKE6b0NGJdls38suMJhLUaKq8dHEqab2Law8RJDb8rFXiWGlp7ggYetrds7qLlDrY2Nf9oE2vkjqMl23juho50h_Hqk3VRvPwbs5l7d37dNetm_a99AfCHxs9YklMkCURz2G4ipJDKVJlWBGcBi46pTYxxQBIwfnDNvwndAeoj_mFFwiBchDncjCOYgcF8tLGgsujGgbqzDQuhrXXiH8aFSDHzDs47CBzdop7FPBdxq9X4gE6DZdlYC0bkwcVCNtdP6SoJ1L1TVokEhoq1UY56iKXW_ZBkLwPLBiuZQVDc.X1ST8Qb85QOfCFRWLSG-kpKR3zGXo2xueh777EOskVk&dib_tag=se&keywords=arduino+uno+originale&qid=1716083411&sprefix=arduino+uno+orginale%2Caps%2C106&sr=8-1)           |
   | DC Motor                                      | 2          | €9.98     | [Buy Here](https://www.amazon.it/YUSHU-Giocattolo-Riduzione-Motoriduttore-Gearwheel/dp/B09ZXZMSXR/ref=sr_1_10?crid=24YO3K15YVHZK&dib=eyJ2IjoiMSJ9.KTsCz3mQTCj-X9gUNM-2WBX8ZcyrMSm3S0fsAaFJjjUOf9HE0VF4T3CvMGCTj8ac2elqvk-ivw-UKospGmjzCnMqx_nON-vG_TfEDy3TG8zuJHVk40HrviHmH8398WYDnw-HsddNpvccKSX4b3EqU8iq0KeftwLBGdy5gx-sikypi3PCadCrrXxIdBsJ46kbtuhKFIIUQbAOEkGeBiNT7H4qYTgJcWh2Dp50AQLjR_NuJIuwflLmszaHgTzSfo3DXhOb0b4xj-sfg5ArsY3TuCZDSvkOPN3ubMnVZf1L--k.kyTaWOM8nCp4wirIC_nc2e6fj5zuRAIdEBkcM2CIy9o&dib_tag=se&keywords=dc+motor+5v&qid=1716083460&sprefix=dc+motor+%2Caps%2C105&sr=8-10)            |
   | 9V Batteries                                  | 3          | €12.95 | [Buy Here](https://www.amazon.it/sspa/click?ie=UTF8&spc=MToyNjEzMDMzOTcxOTAzNzY1OjE3MTYwODM1MjA6c3BfYXRmOjIwMDE5NzIxOTAzMjAxOjowOjo&url=%2FAmazonBasics-Pile-alcaline-Volt-confezione%2Fdp%2FB00MH4QM1S%2Fref%3Dsr_1_1_ffob_sspa%3Fcrid%3D3ESFYJGDOIZ89%26dib%3DeyJ2IjoiMSJ9.fxpFZp4ElMZIcMjtWTuaAmyBbNGh_lI7jx180SUAbBN5y59NEVNPE3Zoa7tJPrneqpYR-a3w0_KHR2fw_WpHxTxJIFAW4NNIMYGFXWKr-AvmxB00oQlfdITGJa5zzLtlHfQsygLqeX3d-NBCI7efYD2lWeGsWY5DbxTV13oeIDe1mo7hgFAqzmprybocTELSJOTJWxsYLACAFWKyyW27NibCidQUmGNhzTCFa-QgTXWwlCXiUfVkk_NlpnPjKsvIABRoRmLOZDYHxcWvCNpLcvOGJMqEBwfRxNrjMSvaYCs.mMrdu5FZJvioUqCvzB52igU_z3pYCYlPKzwGy6ie5cc%26dib_tag%3Dse%26keywords%3D9v%2Bbatteria%26qid%3D1716083520%26sprefix%3D9v%2Bb%252Caps%252C90%26sr%3D8-1-spons%26sp_csd%3Dd2lkZ2V0TmFtZT1zcF9hdGY%26psc%3D1)           |
   | Light Sensor (BH1750)                         | 1          | €8.99     | [Buy Here](https://www.amazon.it/Aihasd-rilevatore-Illumination-Alimentazione-VCompatibile/dp/B01IBGDH7C/ref=sr_1_2?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=8U5L99U9TLTX&dib=eyJ2IjoiMSJ9.uTgUvJ4qEysmwXmJxPOqgZwJHN71oEQGV1tiYz7JiDtxisgeMAhST89gsZHpj5MBZXQOQ5kcW0FNdh4ZV7xRZXMf4W6Xm5FUf-vxlz3Sq8mqZk_YPoUKO6bcqIzX_QVkmNloU5fWuWX2HDR2gxWADddothnzCyGIo9ZOi5mADt7zjxNqDXBDwwj8XYi3ZDLgq1gn0KnM-AKU_4sf_wc8ogeAE4WxjcY5dYlgbCWyRSNH8ma_8OA370WBup6k1gv0bjEfII9xjq5c-E842_hdrzYGNuTWT0z8X2zV5VuszQ0.JowZsPP_DJKy_8250Uy6gXsVY0MJ7LPEhUYEdVS7Zgg&dib_tag=se&keywords=light+sensor+bh1750&qid=1716083542&sprefix=light+sensor+bh1750%2Caps%2C93&sr=8-2)           |
   | Jumper Wire Cables                            | 1          | €9.15     | [Buy Here](https://www.amazon.it/Cavetti-Maschio-Femmina-Arduino-Raspberry/dp/B07QYTK3Q4/ref=sr_1_2?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2KOP6Z2W4MT3Z&dib=eyJ2IjoiMSJ9.cQG_3mdtzbRVTRi0xuTt3ucZHvDNRiKOGbJK5OqouKGJuE9el702ocIB3VIXECMRGsrErd-jTLCBoSMLNKEr-U9PErdUU-Zj3HgZ8WsyV7aHF2flU7I-1slz1HxKftiCPw3QKzrlKWeUWpLmm-xDNDNCRKjjdHQ7YOtEaRtISvi02DbbkqtYuo2yS0laNSy1MXgpAgPrf7T-Ts3k8okbXdNckraHf2WrNudADdYxWk4y9CDhyhu4J1c_D8IBJKPf57cyWmgCUT_VA3IfzxTLgZEUeOYuvyS8V2-EE5tVwGU.qXmbVqreoT2WxjFNsxEN6P9TZEK7jX4YnEEoCEMuP5Y&dib_tag=se&keywords=jumper+wire+cables&qid=1716083566&sprefix=jumper+wire+cables%2Caps%2C91&sr=8-2)           |

   Total price is `€106.94`.Below is the sketch of the system setup. This diagram shows how each component is connected to the Arduino Uno and powered by the 9V batteries.

   <img width="812" alt="Sketch" src="https://github.com/AlexSteiner30/Vertical-Farm/assets/65250433/8d373b8c-aeeb-4022-b7b3-578016f7d1f2">

5. **Upload Code to ESP32 and Arduino**
   - Navigate to the `VerticalFarm/` directories to upload the necessary code to the Arduino.
   - Navigate to the  `CameraWebServer/` directories to upload the necessary code to the ESP32 Camera.

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

