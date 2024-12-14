import boto3
import time
import paho.mqtt.client as mqtt
import json

alias = 'factory/Weight_iot/Weight'

# AWS Credentials
ACCESS_KEY = 'Your ACCESS_KEY'
SECRET_KEY = 'Your SECRET_KEY'

# MQTT Broker
MQTT_BROKER = "Your MQTT Broker IP"  
MQTT_PORT = 1883
MQTT_TOPIC = "sensor/weight"

client_aws = boto3.client('iotsitewise', region_name='us-east-2', 
                          aws_access_key_id=ACCESS_KEY, 
                          aws_secret_access_key=SECRET_KEY)

def on_message(client, userdata, msg):
    try:
       
        payload = msg.payload.decode('utf-8').strip()
        print(f"Received MQTT message on {msg.topic}: {payload}")

        
        weight = float(payload)

        
        epoch = int(time.time())

        
        aws_payload = {
            "entries": [
                {
                    "entryId": str(epoch),
                    "propertyAlias": alias,
                    "propertyValues": [
                        {
                            "value": {
                                "doubleValue": weight
                            },
                            "timestamp": {
                                "timeInSeconds": epoch
                            },
                            "quality": "GOOD"
                        }
                    ]
                }
            ]
        }

        
        response = client_aws.batch_put_asset_property_value(entries=aws_payload['entries'])
        print(f"Data sent to AWS successfully at {time.ctime(epoch)}: {aws_payload}")
    except Exception as e:
        print(f"Error processing MQTT message: {e}")

mqtt_client = mqtt.Client()

mqtt_client.on_message = on_message

try:
    mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)
    print(f"Connected to MQTT Broker at {MQTT_BROKER}:{MQTT_PORT}")
except Exception as e:
    print(f"Failed to connect to MQTT Broker: {e}")
    exit(1)

mqtt_client.subscribe(MQTT_TOPIC)
print(f"Subscribed to MQTT topic: {MQTT_TOPIC}")

try:
    print("Starting MQTT loop...")
    mqtt_client.loop_forever()
except KeyboardInterrupt:
    print("MQTT loop stopped.")
    mqtt_client.disconnect()
