from flask import Flask, render_template, request, jsonify
import requests
app = Flask(__name__)


from pymongo import MongoClient
mongo = client = MongoClient('localhost', 27017)
db = mongo["state-server"]

# PUT /<key> – Adds a versioned object
@app.route('/<key>', methods=['PUT'])
def put_key(key):
  # Get value
  value = request.data.decode('utf-8')
  
  # Insert value into state
  version = 1
  key_collection = db[key]

  if key in db.list_collection_names():
    version = key_collection.count() + 1

  document = {
    'version': version,
    'key': key,
    'value': value
  }

  key_collection.insert_one(document)
    
  # Send response
  return f'Added {key}={value} as version {version}', 200

# GET /<key> – Retrieves the latest version of a key
@app.route('/<key>', methods=['GET'])
def get_key(key):
  if key in db.list_collection_names():
    key_collection = db[key]
    version = key_collection.count()
    doc = key_collection.find_one({ 'version': version })
    return jsonify({ 'value': doc['value'], 'version': doc['version'] }), 200
  else:
    return 'Key not found', 404

# GET /<key>/<version> – Retrieves a specific version of a key
@app.route('/<key>/<version>', methods=['GET'])
def get_key_version(key, version):
  if not version.isnumeric():
    return 'Version not valid', 404
  
  if key not in db.list_collection_names():
    return 'Key not in state', 404
  
  key_collection = db[key]
  
  v = int(version)
  if v > key_collection.count() or v < 1:
    return 'Version not in range', 404
    
  return jsonify({ 'value': key_collection.find_one({ 'version': v })['value'], 'version': v }), 200
  
# DELETE /<key> – Completely deletes a key
@app.route('/<key>', methods=['DELETE'])
def delete_key(key):
  db[key].drop()
  return f'{key} deleted successfully', 200
