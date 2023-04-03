from flask import Flask, request, jsonify
import os

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
    if request.form.keys() != {'id', 'row'}:
        return jsonify({'msg':'invalid form-data'}), 400
    
    id = int(request.form['id'])
    if id == -1:
        id = id_counter
        id_counter += 1
    elif id > id_counter:
        id_counter = id + 1

    write_to_file(f'time_series/{id}', request.form['row'])

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

    # if file does not exist create it
    if not os.path.exists('data/summary.csv'):
        with open('data/summary.csv', 'w') as f:
            f.write('task_id;task_type;time_total;time_holding;num_errors;age;gender;visual_evaluation\n')
    else:
        with open('data/summary.csv', 'r') as f:
            lines = f.readlines()
            if len(lines) > 1:
                id_counter = int(lines[-1].split(';')[0]) + 1

    app.run(debug=True, use_reloader=False, host="0.0.0.0")


if __name__ == '__main__':
    main()