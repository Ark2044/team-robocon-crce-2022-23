from flask import Flask, Response, render_template, url_for
from datetime import datetime
import time
import serial
import read_arduino

app = Flask(__name__)
start = False
print("lololol")



#use_reloader=False

#time.sleep(2)

@app.route('/')
def index():
    global start
    print("hello")
    print(start)
    print("later", start)
    dataa = ser.read()
    tempData = {
        'ultrasonic' : dataa 
    }
    #timex = datetime.now().strftime("%Y.%m.%d|%H:%M:%S")
    #print(timex)

    return render_template('./index.html', **tempData)

def counterr():
        #ard = serial.Serial('COM5', 9600)
    print("hello")   
    dataa = ser.read()
    tempData = {
        'ultrasonic' : dataa 
    }
    #data=ser.read()
    #func_data = read_arduino()
    #print(data)


@app.route('/time_feed')
def time_feed():
    def generate():
        #timex = datetime.now().strftime("%Y.%m.%d|%H:%M:%S")
        #print(timex)
        yield counterr()  # return also will work
    return Response(generate(), mimetype='text') 

if __name__ == '__main__':
    ser = serial.Serial('COM9', 9600)
    app.run(debug=False, threaded=True, use_reloader=False)