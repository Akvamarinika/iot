import time
import paho.mqtt.client as mqtt_client
import random

broker = "broker.emqx.io"
flag = 'off'
dist = 0
sensor_front = 0
sensor_back = 0
values_dist = []
values_front = []


def my_map(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min


def on_message(client, userdata, message):
    global dist
    global sensor_front
    global sensor_back
    global values_dist


    data = str(message.payload.decode("utf-8"))
    print("received message =", data)
    # print(client)
    # print(userdata)
    print("message topic=", message.topic)

    if dist < sensor_front and data.isdigit():
        values_dist.append(int(data))
        sum_val = 0
        for val in values_dist:
            sum_val += val
            dist = sum_val // len(values_dist)
        #dist = int(data)

    if message.topic == "lab/leds/sensor/sonar_front" and data.isdigit() and int(data) != 0:
        sensor_front = int(data)

    if message.topic == "lab/leds/sensor/sonar_back" and data.isdigit() and int(data) != 0:
        sensor_back = int(data)

    if (sensor_front != 0) and (sensor_back != 0) and (dist != 0):
        topic_name = get_topic_for_send()

        if topic_name is not None:
            print(f"topic_name ON: {topic_name}")
            client.publish(topic_name, "on")
            send_off_other_topics(topic_name)


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print("Failed to connect, return code %d\n", rc)


def get_topic_for_send():
    global flag
    stair_length = sensor_front + sensor_back
    print(f"LENGTH: {stair_length}")
    print(f"DIST: {dist}")

    if stair_length-5 <= dist:  # stair_length
        number_strip = my_map(sensor_front, 0, stair_length, 0, 8)
        print(f"NUM STRIP: {number_strip}")
        topic = "lab/leds/strip/set_leds_bytes/" + str(number_strip)
        flag = 'on'
        return topic
    elif flag == 'on':
        for i in range(1, 9):
            topic_name_off = "lab/leds/strip/set_leds_bytes/" + str(i)
            client.publish(topic_name_off, "off")


# def get_topic_for_send():
#     stair_length = sensor_front + sensor_back
#     print(f"LENGTH: {stair_length}")
#     print(f"DIST: {dist}")
#
#     if stair_length - 3 <= dist:
#         width = dist // 8
#         print(f"WIDTH: {width}")
#
#         if 0 < sensor_front <= width * 1:
#             return "lab/leds/strip/set_leds_bytes/1"
#
#         for i in range(2, 9):
#             topic = "lab/leds/strip/set_leds_bytes/" + str(i)
#             if (width * (i - 1)) < sensor_front <= (width * i):
#                 return topic
#     else:
#         for i in range(1, 9):
#             topic_name_off = "lab/leds/strip/set_leds_bytes/" + str(i)
#             client.publish(topic_name_off, "off")


def send_off_other_topics(topic_name_on):
    topic_base_led = "lab/leds/strip/set_leds_bytes/"
    topic_number = topic_name_on[-1]

    for i in range(1, 9):
        if i != int(topic_number):
            topic_name_off = topic_base_led + str(i)
            client.publish(topic_name_off, "off")
            #  print(f"topic_name OFF: {topic_name_off}")


if __name__ == '__main__':
    client = mqtt_client.Client(f'lab_{random.randint(10000, 99999)}')  # id
    client.on_message = on_message  #
    client.on_connect = on_connect  #
    client.connect(broker)
    client.loop_start()

    client.subscribe("lab/leds/sensor/#")
    # client.subscribe("lab/leds/sensor/sonar_front")
    # client.subscribe("lab/leds/sensor/sonar_back")
    # for _ in range(10):
    #  client.publish("lab/leds/strip/set_leds_bytes/1", "on")
    time.sleep(2500)

    client.disconnect()
    client.loop_stop()
