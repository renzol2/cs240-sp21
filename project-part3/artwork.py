from flask import Blueprint, send_file, request, jsonify
from PIL import Image, ImageEnhance
import io
import requests

artwork_server = Blueprint("artwork_server", __name__)

# POST
@artwork_server.route('/<key>', methods=["POST"])
def post_image(key):
  # citation: https://pillow.readthedocs.io/en/stable/handbook/tutorial.html#reading-from-binary-data
  img = Image.open(io.BytesIO(request.data))

  # citation: https://pillow.readthedocs.io/en/stable/handbook/tutorial.html#enhancing-images
  processed = ImageEnhance.Sharpness(img).enhance(20000)
  processed.save('file.png')

  # TODO: save the file in the state server
  b = io.BytesIO()
  img.save(b, format='png')
  contents = b.getvalue()
  r = requests.put('http://localhost:5000/' + key, data=str(contents))
  print(r)

  return send_file('file.png'), 200

def manual_put(store, key, image_data):
  if key not in store:
    store[key] = []
    version = 1
  else:
    version = len(store[key]) + 1

  store[key].append({
    "version": version,
    "value": image_data  # insert image; does it work?
  })
