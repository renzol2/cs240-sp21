from flask import Flask, send_file, jsonify
import requests

import io
app = Flask(__name__)

from artwork import artwork_server
app.register_blueprint(artwork_server)

from mp8 import mp8_server
app.register_blueprint(mp8_server)
