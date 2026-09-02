from flask import Flask, jsonify
import os
import socket

from db import get_connection

app = Flask(__name__)

APP_VERSION = os.getenv("APP_VERSION", "v1")
POD_NAME = socket.gethostname()

@app.get("/health")
def health():
    try:
        conn = get_connection()
        conn.close()
        return jsonify({
        "status": "ok",
        "database": "ok"
        }), 200

    except Exception:
        return jsonify({
        "status": "error",
        "database": "unavailable"        
        }), 503



if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=False)