import random
import numpy

inc0, dec0, inc1, dec1, inc2, dec2, inc3, dec3 = 0

sort_list = []

calib_bool = False
my_dict = {"0":[], "1":[], "2":[], "3":[]}

def inc_dec(index, dir):
    global inst1, inst2, inst3, inst0
    global inc0, inc1, inc2, inc3
    if (index == 0 and dir>0) and inc0<30 and inst0<250:
        inc0=inc0+1
        print("inc0", inc0)
        

    if (index == 1 and dir>0) and inc1<30 and inst1<250:
        inc1=inc1+1
        print("inc1", inc1)
        
    
    if (index == 2 and dir>0) and inc2<30 and inst2<250:
        inc2=inc2+1
        print("inc2", inc2)
    
    if (index == 3 and dir>0) and inc3<30 and inst3<250:
        inc3=inc3+1
        print("inc3", inc3)
    
    if (index == 0 and dir<0) and dec0>=0 and inst0<250:
        dec0=dec0+1
        print("dec0", dec0)    

    if (index == 1 and dir<0) and dec1>=0 and inst1<250:
        dec1=dec1+1
        print("dec1", dec1)
        
    
    if (index == 2 and dir<0) and dec2>=0 and inst2<250:
        dec2=dec2+1
        print("dec2", dec2)
    
    if (index == 3 and dir<0) and dec3>=0 and inst3<250:
        dec3=dec3+1
        print("dec3", dec3)

while calib_bool == False:
    inst0 = random.randint(100,499)
    inst1 = random.randint(100,499)
    inst2 = random.randint(100,499)
    inst3 = random.randint(100,499)
    inst0=inst0+inc0
    inst0=inst0-dec0
    inst1=inst1+inc1
    inst1=inst1-dec1
    inst2=inst2+inc2
    inst2=inst2-dec2
    inst3=inst3+inc3
    inst3=inst3-dec3
    sort_list.append(inst0)
    sort_list.append(inst1)
    sort_list.append(inst2)
    sort_list.append(inst3)
    sort_list = numpy.array(sort_list)
    sort_index = numpy.argsort(sort_list)
    sort_list = sort_list.tolist()
    sort_index = sort_index.tolist()
    sort_list.sort()
    sort_index.reverse()
    sort_list.reverse()
    print("diff", sort_list[0]-sort_list[1])
    print("before breaking", sort_list)
    if (sort_list[0]-sort_list[1]>25):
        index = sort_index[1]
        inc_dec(index, 1)
        #inc1=inc1+5
        sort_list = []
    elif (sort_list[0]-sort_list[2]>25):
        index = sort_index[2]
        inc_dec(index, 1)
        sort_list = []
    elif (sort_list[0]-sort_list[3]>25):
        index = sort_index[3]
        inc_dec(index, 1)
        sort_list = []
    elif (sort_list[0]-sort_list[1]<-25):
        index = sort_index[1]
        inc_dec(index, -1)
        #inc1=inc1+5
        sort_list = []
    elif (sort_list[0]-sort_list[2]<-25):
        index = sort_index[2]
        inc_dec(index, -1)
        sort_list = []
    elif (sort_list[0]-sort_list[3]<-25):
        index = sort_index[3]
        inc_dec(index, -1)
        sort_list = []
    else:
        calib_bool=True
        break
    print(sort_list)
    print(sort_index)


