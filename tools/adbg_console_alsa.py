#
#  Copyright (C) 2012, Ulrich Hecht <ulrich.hecht@gmail.com>
# 
#  This software is provided 'as-is', without any express or implied
#  warranty.  In no event will the authors be held liable for any damages
#  arising from the use of this software.
# 
#  Permission is granted to anyone to use this software for any purpose,
#  including commercial applications, and to alter it and redistribute it
#  freely, subject to the following restrictions:
# 
#  1. The origin of this software must not be misrepresented; you must not
#     claim that you wrote the original software. If you use this software
#     in a product, an acknowledgment in the product documentation would be
#     appreciated but is not required.
#  2. Altered source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.
#  3. This notice may not be removed or altered from any source distribution.
#

import struct
#import pyaudio
import alsaaudio
import sys

sample_rate = 48000
pulse_width = 5

sample_buf_size = 256

def get_audio_devices(dontAsk=True):
    global card_id
    p = pyaudio.PyAudio()
    for i in range(p.get_default_host_api_info()["deviceCount"]):
        di = p.get_device_info_by_index(i)
        if di["maxInputChannels"] > 0:
            card_name = di["name"]
            card_index = di["index"]
            print card_index, card_name
            if card_id == -1:
                card_id = card_index

#card_id = -1
#get_audio_devices()
#print "Using device",card_id

def sample2bit(bit):
    if bit > 8000:
        return False
    elif bit < -8000:
        return True
    else:
        return True

state = 0	# idle
pos = 0		# position in sample stream
byte = 0	# byte transferred
start_of_next_bit = 0	# sample stream position of next bit

def process_sample(prev, bit, next):
    global state, pos, byte, start_of_next_bit
    
    # convert samples to booleans
    bit = sample2bit(bit)
    prev = sample2bit(prev)
    next = sample2bit(next)
    
    # iron out spikes
    if bit != prev and bit != next:
        bit = prev

    if state == 0 and prev == True and bit == False:
        # start bits
        state = 1	# data bit 0
        start_of_next_bit = pos + pulse_width * 2 + pulse_width / 2 - 1
        byte = 0
    elif state >=1 and state < 9 and pos == start_of_next_bit:
        # data bits
        byte |= int(bit) << state - 1
        start_of_next_bit += pulse_width
        state += 1
    elif state == 9 and pos == start_of_next_bit:
        # stop bit
        if bit == True:		# stop bit valid?
            sys.stdout.write(chr(byte))
            #if (byte < 32 or byte > 127) and byte != 10:
            #    sys.exit(1)
        state = 0	# idle
    pos += 1

#file = open("sample.raw", "w")
sound = alsaaudio.PCM(alsaaudio.PCM_CAPTURE, alsaaudio.PCM_NONBLOCK, 'default')
# Set attributes: Mono, 44100 Hz, 16 bit little endian samples
sound.setchannels(1)
sound.setrate(48000)
sound.setformat(alsaaudio.PCM_FORMAT_S16_LE)
sound.setperiodsize(sample_buf_size)

prev = -32768
pprev = -32768

try:

    while True:
        try:
            l, sample_stream = sound.read()
            #print len(sample_stream)
        except IOError, e:
            print 'dropped', e

        if l == 0: continue
        
        #file.write(sample_stream)

        sample_values = struct.unpack(str(len(sample_stream)/2) + 'h', sample_stream)
        for i in sample_values:
            process_sample(pprev, prev, i)
            pprev = prev
            prev = i

except KeyboardInterrupt:
    sound.close()
