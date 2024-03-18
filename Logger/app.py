from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/log', methods=['POST'])
def log_data():
    data = request.get_json()
    action = data.get('action')
    value = data.get('value')

    if action == 'ammo_change':
        log_message = f"Ammo changed to: {value}"
    elif action == 'health_change':
        log_message = f"Health changed to: {value}"
    else:
        log_message = "Unknown action"

    # Log the message or perform any desired logging operations
    print(log_message)

    return jsonify({'message': 'Data logged successfully'})

if __name__ == '__main__':
    app.run(port=5000)