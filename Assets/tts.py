import sys
from gtts import gTTS

if len(sys.argv) < 3:
    print("Usage: tts.py <text> <outputfile>")
    exit(1)

text = sys.argv[1]
out = sys.argv[2]

tts = gTTS(text)
tts.save(out)
