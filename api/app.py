from flask import Flask, jsonify
import os
import socket

app = Flask(__name__)

APP_VERSION = os.getenv("APP_VERSION", "v1")
POD_NAME = socket.gethostname()

@app.get("/health")
def health():
    return jsonify({
        "status": "ok",
    }), 200



if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=False)