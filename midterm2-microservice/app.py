from flask import Flask, request, jsonify
app = Flask(__name__)

# Dict storing room numbers (keys) to guest (guest name as str or False is unoccupied)
hotel = {
  300: False,
  301: False,
  302: False,
  303: False,
  304: False
}

# PUT /guest/<guest>
@app.route('/guest/<guest>', methods=["PUT"])
def put_guest(guest):
  for room_num in hotel:
    # If a room is free, check the guest in
    if hotel[room_num] is False:
      hotel[room_num] = guest
      return jsonify({ 'room': room_num, 'guest': guest }), 200
  
  # If no available room, return 404
  return 'No available room', 404


# DELETE /guest/<guest>
@app.route('/guest/<guest>', methods=["DELETE"])
def delete_guest(guest):
  for room_num in hotel:
    # If guest is in hotel, remove them
    if guest == hotel[room_num]:
      hotel[room_num] = False
      return jsonify({ 'room': room_num, 'guest': guest }), 200
  
  return "Guest not found", 404


# GET /guest/<guest>
@app.route('/guest/<guest>', methods=["GET"])
def get_guest(guest):
  for room_num in hotel:
    # If guest is in hotel, remove them
    if guest == hotel[room_num]:
      return jsonify({ 'room': room_num, 'guest': guest }), 200

  return "Guest not found", 404
