from flask import Blueprint, send_file, request, jsonify
from PIL import Image, ImageEnhance
import io
import requests

artwork_server = Blueprint("artwork_server", __name__)

# POST
@artwork_server.route('/<key>', methods=["POST"])
def post_image(key):
  img = Image.open(io.BytesIO(request.data))

  # Enhance image
  processed = ImageEnhance.Sharpness(img).enhance(20000)
  processed.save('file.png')

  # Send image to state
  b = io.BytesIO()
  processed.save(b, format='png')
  contents = b.getvalue()
  r = requests.put('http://localhost:5000/' + key, data=contents)
  print(r.content)

  return send_file('file.png'), 200

