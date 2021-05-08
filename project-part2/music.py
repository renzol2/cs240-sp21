# CS 240 - Final Project (Part 2)
# See: https://courses.engr.illinois.edu/cs240/sp2021/project/week2/

import argparse
import requests
from music21 import *
import json

parser = argparse.ArgumentParser(description='CS 240 Final Project Part 2')
parser.add_argument('--server', help='Server to connect to', dest='server')
parser.add_argument('--name', help='Name of song', dest='name')

args = parser.parse_args()
server = args.server
name = args.name

URL = f'http://{server}/{name}'

initial_response = requests.get(url = URL)
song_length = initial_response.json()['version']

notes = []
current_offset = 0
for v in range(1, song_length + 1):
  r = requests.get(url = f'{URL}/{v}')
  value = json.loads(r.json()['value'])

  if value.get('tempo') is not None:
    metronome_mark = tempo.MetronomeMark(number=value['tempo'])  # Create a new MetronomeMark
    metronome_mark.offset = current_offset   # Set the offset of the note
    notes.append(metronome_mark)   # Add the note to the list of notes

  new_note = note.Note(value['note'])
  dur = value['duration']

  new_note.duration = duration.Duration(dur)

  if value.get('offset') is not None:
    new_note.offset = value['offset']
  else:
    new_note.offset = current_offset
    current_offset += dur

  notes.append(new_note)

print(notes)
midi_stream = stream.Stream(notes)
midi_stream.write("midi", fp=f"{name}.mid")