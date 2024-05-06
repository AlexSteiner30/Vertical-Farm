from flask import Flask

app = Flask(__name__)

@app.route('/')
def index(method="POST"):
    return [600,30,24,204, 459, 33]

if __name__ == '__main__':
    app.run(debug=True, port=5001, host='0.0.0.0')