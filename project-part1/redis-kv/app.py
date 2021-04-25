from flask import Flask, render_template, request, jsonify
import requests
import redis
app = Flask(__name__)

r = redis.Redis(host='localhost', port=6379, db=0)

# PUT /<key> – Adds a versioned object
@app.route('/<key>', methods=['PUT'])
def put_key(key):
  # Get value
  value = request.data.decode('utf-8')
  
  # TODO: Insert value at key, checking version too
  # Insert value into state
  version = 1

  if r.exists(key) == 1:
    # Add key/value with incremented version
    version = r.hlen(key) + 1
    r.hset(key, version, value)
  else:
    # Add key/value at version 1
    r.hset(key, version, value)

  # Send response
  return f'Added {key}={value} as version {version}', 200

# GET /<key> – Retrieves the latest version of a key
@app.route('/<key>', methods=['GET'])
def get_key(key):
  print(r.keys())
  if r.exists(key) == 1:
    version = r.hlen(key)
    value = r.hget(key, version).decode('utf-8')
    return jsonify({ 'value': value, 'version': version }), 200
  else:
    return 'Key not found', 404

# GET /<key>/<version> – Retrieves a specific version of a key
@app.route('/<key>/<version>', methods=['GET'])
def get_key_version(key, version):
  if not version.isnumeric():
    return 'Version not valid', 404
  
  if r.exists(key) == 0:
    return 'Key not in state', 404

  v = int(version)
  if v > r.hlen(key) or v < 1:
    return 'Version not in range', 404
    
  return jsonify({ 'value': r.hget(key, v).decode('utf-8'), 'version': v }), 200
  
# DELETE /<key> – Completely deletes a key
@app.route('/<key>', methods=['DELETE'])
def delete_key(key):
  r.delete(key)
  return f'{key} deleted successfully', 200