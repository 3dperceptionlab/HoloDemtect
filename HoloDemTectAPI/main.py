from flask import Flask, request, jsonify
import os
from PIL import Image, UnidentifiedImageError
from io import BytesIO
import base64

id_counter = 0

def write_to_file(file_name, row):
    # print row to file in data directory with id as name
    with open(f'data/{file_name}.csv', 'a') as f:
        # check if row has a \n at the end, if not add it        
        if row[-1] != '\n':
            row += '\n'
        f.write(row)

app = Flask(__name__)
@app.route('/', methods=["GET"])
def home():
    # return a summary of the methods time_series and update inside <p> tags
    return "<p>POST time series on /time_series</p> <p> Body (form-data): 'id' (string, use -1 in the first run, if the id already exists it will be OVERWRITTEN) and 'row' (string, csv row) </p> \
            <p> ------------------ </p> \
            <p>POST summary on /summary</p> <p> Body (form-data): 'id' (string) and 'row' (string, csv row) </p> \
            <p> ------------------ </p> \
            <p> RESPONSE 200: 'msg' (string -> sucess) and 'id' (string) </p> \
            <p> RESPONSE 400: 'msg' (string -> invalid form-data). The parameters are not correct or the id is not valid (in summary in can't be -1 or lower than the current counter)</p>"

@app.route('/time_series', methods=["POST"])
def time_series():
    global id_counter
    #Check if request.form.keys() dont include 'id', 'row'
    if not 'id' in request.form.keys() or not 'row' in request.form.keys():
        return jsonify({'msg':'invalid form-data'}), 400
    
    id = int(request.form['id'])
    if id == -1:
        id = id_counter
        id_counter += 1
        write_to_file(f'time_series_low_performance/{id}', 'Time;RightHandObject;LeftHandObject;RightHandPos;RightHandRot;LeftHandPos;LeftHandRot;HeadPos;HeadRot;ObjectPut;TotalErrors;EyeTrackerPos;EyeTrackerDir')

    write_to_file(f'time_series_low_performance/{id}', request.form['row'])

    if 'img' in request.form.keys():
        print('image length:', len(request.form['img']))
        try:
            im = Image.open(BytesIO(base64.b64decode(request.form['img']))).convert(mode='RGB')
            # to get the date: first get the request.form['row'] and split it by ';' then get the first element and change every ':' to '-'
            date = request.form['row'].split(';')[0].replace(':', '-')
            # check if the the path exists, if not create it
            if not os.path.exists(f'data/images/{id}'):
                os.makedirs(f'data/images/{id}')
            im.save(f'data/images/{id}/{date}.png')
        except UnidentifiedImageError:
            print('Controlled ERROR: UnidentifiedImageError')
     

    # return success message and id with code 200
    return jsonify({'msg':'success', 'id':id}), 200

@app.route('/summary', methods=["POST"])
def summary():
    # do the same as the function time_series but with the file data_summary.csv
    if request.form.keys() != {'id', 'row'}:
        return jsonify({'msg':'invalid form-data'}), 400
    
    id = int(request.form['id'])
    if id == -1:
        return jsonify({'msg':'id cannot be -1 in summary post'}), 400
    elif id > id_counter:
        return jsonify({'msg':'id is too high'}), 400

    write_to_file('summary', request.form['row'])

    # return success message and id with code 200
    return jsonify({'msg':'success', 'id':id}), 200    

def main():
    global id_counter
    # if directory does not exist create it
    if not os.path.exists('data'):
        os.makedirs('data')

    if not os.path.exists('data/time_series'):
        os.makedirs('data/time_series')

    if not os.path.exists('data/images'):
        os.makedirs('data/images')

    # if file does not exist create it
    if not os.path.exists('data/summary.csv'):
        with open('data/summary.csv', 'w') as f:
            f.write('task_id;task_type;total_time;time_holding;num_errors;total_objects;visual_evaluation;eval_id\n')
    else:
        with open('data/summary.csv', 'r') as f:
            lines = f.readlines()
            if len(lines) > 1:
                id_counter = int(lines[-1].split(';')[0]) + 1

    app.run(debug=True, use_reloader=False, host="0.0.0.0")


if __name__ == '__main__':
    main()