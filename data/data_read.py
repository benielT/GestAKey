# Created by Yilei
# On 4 April 2017 for GestKEY applicaiton
# Copyright reserved

import serial
import re
import math
from openpyxl import *

# To get the relative data from the data


def get_data(getdata_data,baseline):
    getdata_result = [0] * 9
    for i in range(9):
        getdata_result[i] = abs(float(getdata_data[i]) - float(baseline[i])) / float(baseline[i])
    return getdata_result

#  To know whether the pad is touched


def whether_touched(touched_data):

    touched_result = False
    for i in range(len(touched_data)):
        if touched_data[i] > 0.02:
            touched_result = True
            break
    return touched_result

# To get the position of the touch point


def get_position(rawdata, baseline):

    #  Get the initial data
    data = [0] * 9
    for i in range(9):
        data[i] = abs(float(rawdata[i]) - float(baseline[i])) / float(baseline[i])

    '''
    Get the position of the touch
    If it is not touched, give it [11,11]
    If it is touched by two finger, give it [0,0]
    '''
    position = [11, 11]

    if whether_touched(data) is False:
        return position

    costx = 0
    costy = 0
    mincostx = 10000
    mincosty = 10000
    x = 0.0
    y = 0.0

    while x <= 10:
          costx = data[8] * pow(0.0 - x, 2) + \
                data[7] * pow(3.3 - x, 2) + \
                data[6] * pow(6.6 - x, 2) + \
                data[5] * pow(10.0 - x, 2)

          if costx < mincostx:
              mincostx = costx
              position[0] = x
          x = x + 0.02

    while y <= 10:
        costy = data[4] * pow(0.0 - y, 2) + \
                data[3] * pow(2.5 - y, 2) + \
                data[2] * pow(5.0 - y, 2) + \
                data[1] * pow(7.5 - y, 2) + \
                data[0] * pow(10.0 - y, 2)
        if costy < mincosty:
            mincosty = costy
            position[1] = y
        y = y + 0.02

    return position

#  Gesture Recognition


def gesture_recognition(gesturerecognition_data):
    xMax = 0
    xMin = 10
    yMax = 0
    yMin = 10
    for i in range(len(gesturerecognition_data)):
        xMax = max(xMax, gesturerecognition_data[i][0])
        xMin = min(xMin, gesturerecognition_data[i][0])
        yMax = max(yMax, gesturerecognition_data[i][1])
        yMin = min(yMin, gesturerecognition_data[i][1])
    xDis = xMax - xMin
    yDis = yMax - yMin

    '''
    Gesture protocal
    1---up
    2---down
    3---left
    4---right
    5---up-right
    6---down-right
    7---up-down
    8---down-up
    9---circle
    '''
    vertical_sum1 = 0
    vertical_sum2 = 0
    vertical_sum3 = 0
    vertical_sum4 = 0
    vertical_sum5 = 0

    horizontal_sum1 = 0
    horizontal_sum2 = 0
    horizontal_sum3 = 0
    horizontal_sum4 = 0
    horizontal_sum5 = 0

    up_right_sum = 0
    down_right_sum = 0
    circle_sum = 0

    for single_position in gesturerecognition_data:

        vertical_sum1 = vertical_sum1 + pow(single_position[0] - 1, 2)
        vertical_sum2 = vertical_sum2 + pow(single_position[0] - 3, 2)
        vertical_sum3 = vertical_sum3 + pow(single_position[0] - 5, 2)
        vertical_sum4 = vertical_sum4 + pow(single_position[0] - 7, 2)
        vertical_sum5 = vertical_sum5 + pow(single_position[0] - 9, 2)

        horizontal_sum1 = horizontal_sum1 + pow(single_position[1] - 1, 2)
        horizontal_sum2 = horizontal_sum2 + pow(single_position[1] - 3, 2)
        horizontal_sum3 = horizontal_sum3 + pow(single_position[1] - 5, 2)
        horizontal_sum4 = horizontal_sum4 + pow(single_position[1] - 7, 2)
        horizontal_sum5 = horizontal_sum5 + pow(single_position[1] - 9, 2)

        up_right_sum = up_right_sum + pow(single_position[0] - single_position[1], 2) / 2
        down_right_sum = down_right_sum + pow(single_position[0] + single_position[1] - 10, 2) / 2

        circle_sum = circle_sum + pow(math.sqrt(pow(single_position[0] - 5, 2)
                                                + pow(single_position[1] - 5, 2)) - 2.5, 2)

    threshold = 5

    if xDis < threshold and yDis < threshold:
        return 0

    if xDis < threshold or yDis < threshold:

        min_sum = min(vertical_sum1, vertical_sum2, vertical_sum3, vertical_sum4, vertical_sum5,
                  horizontal_sum1, horizontal_sum2, horizontal_sum3, horizontal_sum4, horizontal_sum5)

        if min_sum == vertical_sum1 or min_sum == vertical_sum2 or min_sum == vertical_sum3 or \
                        min_sum == vertical_sum4 or min_sum == vertical_sum5:

            if gesturerecognition_data[0][1] < threshold and gesturerecognition_data[-1][1] < threshold and \
                    gesturerecognition_data[int(len(gesturerecognition_data)/2)][1] > 7:
                return 7
            elif gesturerecognition_data[0][1] > threshold and gesturerecognition_data[-1][1] > threshold and \
                    gesturerecognition_data[int(len(gesturerecognition_data)/2)][1] < 3:
                return 8
            elif gesturerecognition_data[0][1] < gesturerecognition_data[-1][1]:
                return 1
            elif gesturerecognition_data[0][1] > gesturerecognition_data[-1][1]:
                return 2

        if min_sum == horizontal_sum1 or min_sum == horizontal_sum2 or min_sum == horizontal_sum3 or \
                        min_sum == horizontal_sum4 or min_sum == horizontal_sum5:
            if gesturerecognition_data[0][0] < gesturerecognition_data[-1][0]:
                return 4
            if gesturerecognition_data[0][0] > gesturerecognition_data[-1][0]:
                return 3
    else:
        min_sum = min(up_right_sum, down_right_sum, circle_sum)
        if min_sum == up_right_sum:
            if gesturerecognition_data[0][0] < gesturerecognition_data[-1][0]:
                return 5
        if min_sum == down_right_sum:
            if gesturerecognition_data[0][0] < gesturerecognition_data[-1][0]:
                return 6
        if min_sum == circle_sum:
            return 9

    return 0
    '''
    Gesture recognition algorithm with the data feature
    '''
    # if abs(gesturerecognition_data[0][0] - gesturerecognition_data[-1][0]) < 5:
    #     if abs(gesturerecognition_data[0][1] - gesturerecognition_data[-1][1]) < 5:
    #         if xDis > 5 and yDis > 5:
    #             return 9
    #
    # if gesturerecognition_data[0][1] < 5 and gesturerecognition_data[-1][1] < 5:
    #     if gesturerecognition_data[int(len(gesturerecognition_data)/2)][1] - gesturerecognition_data[0][1] > 5:
    #         return 7
    #
    # if gesturerecognition_data[0][1] > 5 and gesturerecognition_data[-1][1] > 5:
    #     if gesturerecognition_data[0][1] - gesturerecognition_data[int(len(gesturerecognition_data)/2)][1] > 5:
    #         return 8
    #
    # if xDis < 5:
    #     if yDis > 5:
    #         if gesturerecognition_data[-1][1] > gesturerecognition_data[0][1]:
    #             return 1
    #         if gesturerecognition_data[-1][1] < gesturerecognition_data[0][1]:
    #             return 2
    # if xDis > 5:
    #     if yDis < 5:
    #         if gesturerecognition_data[-1][0] > gesturerecognition_data[0][0]:
    #             return 4
    #         if gesturerecognition_data[-1][0] < gesturerecognition_data[0][0]:
    #             return 3
    #     if yDis > 5:
    #         if gesturerecognition_data[-1][1] > gesturerecognition_data[0][1]:
    #             return 5
    #         if gesturerecognition_data[-1][1] < gesturerecognition_data[0][1]:
    #             return 6
    # return 0

# Communication info
SERIAL_PORT = '/dev/cu.usbmodem1421'
COMMUNICATION_RATE = 115200
ser = serial.Serial(SERIAL_PORT, COMMUNICATION_RATE)

import socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
ip = '127.0.0.1'
port = 33335


sock_receive = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_receive.setblocking(0)
port_receive = 33336
sock_receive.bind((ip, port_receive))


# find the baseline for all the keys
key_dic = {'a': 0, 'b': 11, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'h': 7, 'i': 8, 'j': 9,
           'k': 10, 'l': 11, 'm': 12, 'n': 13, 'o': 14, 'p': 15, 'q': 16, 'r': 17, 's': 18,
           't': 19, 'u': 20, 'v': 21, 'w': 22, 'x': 23, 'y': 24, 'z': 25, 'delete': 51, 'fn': 63}

all_key = ['0', '11', '8', '2', '14', '3', '5', '4', '34', '38', '40', '37', '46',
           '45', '31', '35', '12', '15', '1', '17', '32', '9', '13', '7', '16', '6', '51', '63']

# all_key = ['12','0','6']

base_array = []

# for scan_key in all_key:
#     ser.write(scan_key.encode('UTF-8'))
#     for i in range(10):
#         ser.readline()
#     baseline = re.findall(r"\d+", str(ser.readline(), encoding="utf-8"))
#     base_array.append(baseline)
#
# print("Baseline reading finished, data is as following:")
# for base_data in base_array:
#     print(base_data)

touched = False
previous_touched = False
read = True
data_list = []
gesture = 0
keyData = ''
baseline = [380, 360, 320, 370, 400, 405, 410, 420, 413]
status = False
previous_status = False
last_key = "NULL"
pressed = False
# Start the the data reading part
print("Please do gesture")

while True:

    # Get the original data
    try:
        raw_data_str = str(ser.readline(), encoding="utf-8")
        raw_data_array = re.findall(r"\d+", raw_data_str)
        # Get the touch position
        data = get_data(raw_data_array, baseline)
        position = get_position(raw_data_array, baseline)

        # Check whether the data should be added
        if pressed:
            if position != [11, 11]:
                data_list.append(position)
    except:
        continue

    try:
        if pressed == False:
            data_list = []

        keyData = sock_receive.recvfrom(4096)
        key = keyData[0].decode('UTF-8')

        if key == "-1":
            continue

        print(key)

        if key == "STOP":
            read = False
            continue
        elif key == "START":
            read = True
            continue

        if read is False:
            continue

        bkey = key.encode('UTF-8')
        # ser.flushInput()
        # ser.flushOutput()
        # print(key)
        # print(last_key)
        if last_key == "NULL":
            pressed = True
            ser.write(bkey)
            last_key = key

        elif last_key == key:
            last_key = "NULL"
            # print("pressed")
            pressed = False
            if len(data_list) > 2:
                print("--------------NEW GESTURE---------------")
                print("KEY NUM ---"+key)
                del data_list[0]
                del data_list[0]
                del data_list[0]
                del data_list[0]
                gesture = gesture_recognition(data_list)
                print("GESTURE ---" + str(gesture))
                try:
                    sock.sendto(bytes(int(gesture)), (ip, port))
                except:
                    print("error")
            # print(data_list)
            data_list = []
    except socket.error:
        continue
