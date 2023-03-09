import random
import numpy
import csv

filename = "duty_cycle.csv"
file = open(filename, "a")
fields = ['Duty_Motor1', 'Duty_Motor2', 'Duty_Motor3', 'Duty_Motor4']
with open(filename, 'a', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(fields)
file.close()

freq1 = freq2 = freq3 = freq4 = 5000
duty1 = duty2 = duty3 = duty4 = 50
inc0 = dec0 = inc1 = dec1 = inc2 = dec2 = inc3 = dec3 = 0

sort_list = []

calib_bool = False
my_dict = {"0": [], "1": [], "2": [], "3": []}


def inc_dec(index, dir):
    global inc0, inc1, inc2, inc3, dec0, dec1, dec2, dec3, duty1, duty2, duty3, duty4
    if (index == 0 and dir > 0) and inc0 < 30 and count1 < 250:
        inc0 = inc0+1
        duty1 += inc0
        p1.ChangeDutyCycle(duty1)
        # print("inc0", inc0)

    if (index == 1 and dir > 0) and inc1 < 30 and count2 < 250:
        inc1 = inc1+1
        duty2 += inc1
        p2.ChangeDutyCycle(duty2)
        # print("inc1", inc1)

    if (index == 2 and dir > 0) and inc2 < 30 and count3 < 250:
        inc2 = inc2+1
        duty3 += inc2
        p3.ChangeDutyCycle(duty3)
        # print("inc2", inc2)

    if (index == 3 and dir > 0) and inc3 < 30 and count4 < 250:
        inc3 = inc3+1
        duty4 += inc3
        p4.ChangeDutyCycle(duty4)
        # print("inc3", inc3)

    if (index == 0 and dir < 0) and dec0 >= 0 and count1 < 250:
        dec0 = dec0+1
        duty1 -= dec0
        p1.ChangeDutyCycle(duty1)
        # print("dec0", dec0)

    if (index == 1 and dir < 0) and dec1 >= 0 and count2 < 250:
        dec1 = dec1+1
        duty2 -= dec1
        p2.ChangeDutyCycle(duty2)
        # print("dec1", dec1)

    if (index == 2 and dir < 0) and dec2 >= 0 and count3 < 250:
        dec2 = dec2+1
        duty3 -= dec2
        p3.ChangeDutyCycle(duty3)
        # print("dec2", dec2)

    if (index == 3 and dir < 0) and dec3 >= 0 and count4 < 250:
        dec3 = dec3+1
        duty4 -= dec3
        p4.ChangeDutyCycle(duty4)
        # print("dec3", dec3)


def calibrate():
    while (count1 != 0 or count2 != 0 or count3 != 0 or count4 != 0):
        global sort_list, calib_bool, file
        global inc0, inc1, inc2, inc3,duty1,duty2,duty3,duty4
        sort_list.append(count1)
        sort_list.append(count2)
        sort_list.append(count3)
        sort_list.append(count4)
        sort_list = numpy.array(sort_list)
        sort_index = numpy.argsort(sort_list)
        sort_list = sort_list.tolist()
        sort_index = sort_index.tolist()
        sort_list.sort()
        sort_index.reverse()
        sort_list.reverse()
        # print("diff", sort_list[0]-sort_list[1])
        # print("before breaking", sort_list)
        if (sort_list[0]-sort_list[1] > 5):
            index = sort_index[1]
            inc_dec(index, 1)
            # inc1=inc1+5
            sort_list = []
        elif (sort_list[0]-sort_list[2] > 5):
            index = sort_index[2]
            inc_dec(index, 1)
            sort_list = []
        elif (sort_list[0]-sort_list[3] > 5):
            index = sort_index[3]
            inc_dec(index, 1)
            sort_list = []
        elif (sort_list[0]-sort_list[1] < -5):
            index = sort_index[1]
            inc_dec(index, -1)
            # inc1=inc1+5
            sort_list = []
        elif (sort_list[0]-sort_list[2] < -5):
            index = sort_index[2]
            inc_dec(index, -1)
            sort_list = []
        elif (sort_list[0]-sort_list[3] < -5):
            index = sort_index[3]
            inc_dec(index, -1)
            sort_list = []
        else:
            calib_bool = True
            #print('calibrated')
            data = [[str(duty1),str(duty2),str(duty3),str(duty4)]]
            # create the CSV
            with open(filename, 'a', newline='') as f:
                writer = csv.writer(f)
                writer.writerows(data)
            file.close()
            #stp()
            break
        # print(sort_list)
        # print(sort_index)