from flask import Flask, render_template, request, jsonify
import requests
app = Flask(__name__)

# State of the server. Stores a key (string)
# and value (array storing versions of value)
# Version 1 of a key is at its value[0]
state = {}

# PUT /<key> – Adds a versioned object
@app.route('/<key>', methods=['PUT'])
def put_key(key):
  print('State before PUT ' + str(key))
  print(state)

  # Get value
  value = request.data.decode('utf-8')
  
  # TODO: Insert value at key, checking version too

# GET /<key> – Retrieves the latest version of a key
@app.route('/<key>', methods=['GET'])
def get_key(key):
  # TODO: Get value

# GET /<key>/<version> – Retrieves a specific version of a key
@app.route('/<key>/<version>', methods=['GET'])
def get_key_version(key, version):
  # TODO: Get value at specific key/version
  
# DELETE /<key> – Completely deletes a key
@app.route('/<key>', methods=['DELETE'])
def delete_key(key):
  # TODO: Delete key and data
  return f'{key} deleted successfully', 200