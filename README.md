# Smart Factory Quality Monitoring System
This project is designed to measure weight using an ESP-32 microcontroller, load cell, and MQTT for data transmission. The system uploads the data to an AWS Cloud for further analysis and prompts for regular updates of the measured object.
## Features
- Real-Time Detection of Irregularities 
- Ensuring Consistent Product Quality 
- Enhanced Data Transparency and Management Efficiency 
- Building Long-Term Data Assets
---
# System architecture
![插入圖片](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/System%20Architecture.png)
---
# Hardware – Arduino
## Required Components
- **NodeMCU ESP-32** 
- **HX711** 
- **LCD module**  
- **Load cell**  
*(For a detailed list of components, refer to the BOM file.)*
## Setup Instructions
### 1. Hardware Connections
Connect the components as shown in the diagram below:  
![Hardware Connections](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/Aruduino%20%26%20MQTT/Fig1_v2.jpg)
### 2. Arduino Environment Setup
1. **Download and install Arduino IDE**  
   Visit the [Arduino IDE download page](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE) and install it based on your operating system.  
2. **Install the CH340 driver**  
   Download and install the [CH340 driver](https://www.arduined.eu/ch340-windows-10-driver-download/) to enable communication with the ESP-32.  
3. **Add the ESP32 board package**  
   - Open the Arduino IDE and navigate to:  
     ```
     File -> Preferences -> Additional Boards Manager URLs
     ```
   - Add the following URL:  
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
4. **Install the ESP32 Board**  
   - Go to **Tools -> Board -> Boards Manager** and search for **ESP32**.  
   - Install the board package (refer to the diagram below):  
     ![Boards Manager](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/Aruduino%20%26%20MQTT/Fig%202.png)  
5. **Install required libraries**  
   - Open the **Library Manager**:  
     ```
     Tools -> Manage Libraries
     ```
   - Install the following libraries:  
     - **HX711**  
     - **PubSubClient**  
     - **LiquidCrystal_PCF8574**  
     ![Library Manager](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/Aruduino%20%26%20MQTT/Fig%203.png)
### 3. MQTT Environment Setup
1. **Download MQTT software**  
   Install [Mosquitto MQTT software](https://mosquitto.org/download/), and use the default installation path: "C:\Program Files\mosquitto\"
2. **Modify `mosquitto.conf`**  
Open the `mosquitto.conf` file in the installation directory and add the following lines:  
- **listener 1883**
- **allow_anonymous true**
3. **Start Mosquitto Broker service**  
- Open **Windows Services**, locate **Mosquitto Broker**, and double-click it.  
- ![Mosquitto Service](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/Aruduino%20%26%20MQTT/Fig%204.png)  
- Start the **Mosquitto Broker** service.  
  ![Start Service](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/Aruduino%20%26%20MQTT/Fig%205.png)
4. **Verify MQTT broker functionality**  
- Open a command prompt (cmd) and navigate to the Mosquitto directory:  
  ```bash
  cd "C:\Program Files\mosquitto\"
  ```
- Run the broker using:  
  ```bash
  mosquitto -c "C:\Program Files\mosquitto\mosquitto.conf" -v
  ```
- Open another command prompt, find your IPv4 address:  
  ```bash
  ipconfig
  ```
  *Note the IPv4 address.*  
5. **Test MQTT subscriptions**  
- In the command prompt, subscribe to a topic using:  
  ```bash
  mosquitto_sub -h 192.168.XXX.XXX -t sensor/weight
  ```
  Replace `192.168.XXX.XXX` with your IPv4 address and `sensor/weight` with your desired topic.  
---
# Software – AWS Cloud
## Local machine
1. Program File:
   - Create a connect_AWS.py on local to send data to AWS Cloud's IoT SiteWise.
2. Install Necessary Packages:
   - Ensure you have installed AWS SDK for Python (boto3) and other required Python packages like pandas.
   - Installation command: `pip install boto3`
3. Configure AWS IAM Credentials:
   - In the connect_AWS.py, enter the Access_key and Secret_key provided by AWS Cloud to establish a secure connection with AWS.
4. Sending Data:
   - In the connect_AWS.py, set the data format to be sent to IoT SiteWise, such as measurements (e.g., weight).
   - The connect_AWS.py will send data to AWS at specified intervals.
## AWS Cloud
1. IAM User Setup:
   - Log in to the AWS Management Console and go to the IAM service.
   - Create a new IAM user.
   - Generate Access Key and Secret Key for the user, which will be used for the local script’s connection configuration.
   - In the Set permissions step, select Attach policies directly and grant full access to AWS SiteWise operations and AWS GRAFANA's write and read permissions.
    ![Figure 1](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%201.png)
2. IoT SiteWise: Creating Asset Models and Assets:
   - Go to the IoT SiteWise service, click Models, and create a new model:
     - Name it WeightModel.
     - Add the following two types of definitions to the model:
       - Attribute Definitions: Used to record static properties (such as device name or specifications).
       - Measurement Definitions: Used to receive measurement data (such as weight data).
      ![Figure 2](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%202.png)
      ![Figure 3](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%203.png)
   - Based on the WeightModel just created, add an asset:
     - Asset name: Weight1.
     - During the addition process, specify the Measurement alias (`/factory/Weight_test/Weight`) for data transmission and retrieval.
       ![Figure 4](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%204.png)
       ![Figure 5](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%205.png)

   - After completion, verify the asset page. You can execute connect_AWS.py locally to check if the transmitted data appears in the Latest value and Latest value timestamp fields.
     ![Figure 6](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%206.png)
## Amazon Managed Grafana
1. Create Grafana Workspace:
   - Go to Amazon Managed Grafana and click Create workspace.
   - During the creation process:
     - Name it foodfactoryDemo.
     - Permission type: Select Service managed.
     - Choose AWS IAM Identity Center for identity management.
     - Select AWS IoT SiteWise as the data source.
     
   - After creation, add the users who need to use Grafana to the Workspace user list.
   - Click the generated Grafana URL to log in to the Grafana Workspace.
     ![Figure 7](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%207.png)
     ![Figure 8](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%208.png)
2. Create Dashboard:
   - In the Grafana Workspace, add a data source and select AWS IoT SiteWise.
   - Create a new dashboard, selecting IoT SiteWise as the data source.
   - Select the asset Weight1 previously created in SiteWise and choose its Measurement data to display as Time Series data on the dashboard.
   - Configure the dashboard style, such as line charts.
     ![Figure 9](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%209.png)
     ![Figure 10](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%2010.png)
     ![Figure 11](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%2011.png)

# Calibration and Program Execution
1. **Connect the device**  
Plug the ESP-32 into your computer via USB.
2. **Select board and port**  
- In the Arduino IDE, select the board type:  
  ```
  Tools -> Board -> esp32 -> NodeMCU-32S
  ```
- Select the correct port (viewable in **Device Manager**).
3. **Run calibration**  
- Open and upload the **HX711_Calibration** program.  
- Prepare a test object with a known weight (e.g., a calibration weight).  
- Modify the `sample_weight` variable to match the object's actual weight.  
- Follow the on-screen instructions to calibrate and note the calibration value displayed.  
4. **Run the MQTT script**  
- Open the `connect_AWS.py` script and modify the following variables:  
  - `MQTT_BROKER`: Set it to your MQTT broker's IPv4 address.  
  - `MQTT_TOPIC`: Set it to your desired MQTT topic.  
- Run the script.
5. **Run the main program**  
- Open the `Final_code` program in the Arduino IDE and modify:  
  - `scale_factor`: Replace with the calibration value obtained earlier.  
  - `ssid`: Set your WiFi network ID.  
  - `Password`: Set your WiFi password.  
  - `mqttServer`: Set the broker's IPv4 address.  
  - `mqttTopic`: Set your desired MQTT topic.  
- Upload the program to the ESP-32.
6. Log in to AWS SiteWise and confirm that the Measurements field in the Weight1 asset displays the latest data and timestamp.
7. Log in to Grafana and confirm that the dashboard displays the transmitted data in real-time.
   ![Figure 12](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%2012.png)
## System Functionality
- The system transmits weight data via MQTT every **10 seconds** to an AWS database for analysis.  
- Every **15 seconds**, the system prompts the user to replace the measured object for continuous monitoring.
