# Smart Factory Quality Monitoring System
This project is designed to measure weight using an ESP-32 microcontroller, load cell, and MQTT for data transmission. The system uploads the data to an AWS database for further analysis and prompts for regular updates of the measured object.

---
## Background
## Features

---

# System architecture
![插入圖片](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/System%20Architecture.png)

---

# Hardware – Arduino
## Required Components
- **NodeMCU ESP-32** x1  
- **HX711** x1  
- **LCD module**  
- **Load cell**  
*(For a detailed list of components, refer to the BOM file.)*
---

# Software – AWS Cloud

## Local machine

1. Program File:
   - Create a SendData.py on local to send data to AWS Cloud's IoT SiteWise.

2. Install Necessary Packages:
   - Ensure you have installed AWS SDK for Python (boto3) and other required Python packages like pandas.
   - Installation command: `pip install boto3`

3. Configure AWS IAM Credentials:
   - In the SendData.py, enter the Access_key and Secret_key provided by AWS Cloud to establish a secure connection with AWS.

4. Sending Data:
   - In the SendData.py, set the data format to be sent to IoT SiteWise, such as measurements (e.g., weight).
   - The SendData.py will send data to AWS at specified intervals.

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

   - After completion, verify the asset page. You can execute SendData.py locally to check if the transmitted data appears in the Latest value and Latest value timestamp fields.
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

## Process Completion and Verification

1. Run the local SendData.py script to send measurement data to AWS Cloud.
2. Log in to AWS SiteWise and confirm that the Measurements field in the Weight1 asset displays the latest data and timestamp.
3. Log in to Grafana and confirm that the dashboard displays the transmitted data in real-time.
   ![Figure 12](https://github.com/iiot-4/Smart-Factory-Quality-Monitoring-System/blob/main/AWS%20Cloud/Fig%2012.png)
