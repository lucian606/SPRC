from flask import Flask, request, Response
from flask.json import jsonify

app = Flask(__name__)

@app.route('/')
def home():
    return 'Home Page'

@app.route('/greet')
def greetings():
    return "Greeting..."

@app.route('/ruta1', methods=['GET'])
def hello():
    return "Hello Lab 03!"

if __name__ == '__main__':
    app.run('0.0.0.0', port=7020, debug=True)
