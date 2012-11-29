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
import pyaudio
import sys
from optparse import OptionParser

sample_rate = 48000
pulse_width = 5
sample_buf_size = 1024

parser = OptionParser()
parser.add_option('-i', '--invert', action = 'store_true', dest = 'invert', help = 'Invert signal before processing.')
parser.add_option('-a', '--audio-interface', dest = 'audio_system', default = 'alsa', help = 'Audio interface used ("alsa" or "portaudio")')
parser.add_option('-d', '--device', dest = 'audio_device', default = None, help = 'Audio input device')
parser.add_option('-l', '--list-devices', action = 'store_true', dest = 'list_devices', help = 'List available audio devices')
parser.add_option('--vu-meter', action = 'store_true', dest = 'vu_meter', help = 'Show signal levels on input device')
(options, args) = parser.parse_args()

class AlsaSound:
    def __init__(self, device = 'default'):
        import alsaaudio
        self.sound = alsaaudio.PCM(alsaaudio.PCM_CAPTURE, alsaaudio.PCM_NORMAL, 'default')
        self.sound.setchannels(1)
        self.sound.setrate(sample_rate)
        self.sound.setformat(alsaaudio.PCM_FORMAT_S16_LE)
        self.sound.setperiodsize(sample_buf_size)
    def read(self):
        return self.sound.read()[1]
    def close(self):
        self.sound.close()
    def device_list(self):
        return 'unimplemented'

class PortSound:
    def __init__(self, device = 0):
        import pyaudio
        self.pa = pyaudio.PyAudio()
        self.sound = self.pa.open(format = pyaudio.paInt16, channels = 1, rate = sample_rate,
            input_device_index = device, input = True, output = False)
    def read(self):
        return self.sound.read(sample_buf_size)
    def close(self):
        self.sound.close()
    def device_list(self):
        ret = ''
        for i in range(self.pa.get_default_host_api_info()["deviceCount"]):
            di = self.pa.get_device_info_by_index(i)
            if di["maxInputChannels"] > 0:
                card_name = di["name"]
                card_index = di["index"]
                ret += str(card_index) + ' ' + card_name + '\n'
        return ret

def sample2bit(bit):
    if bit < -15000:
        return False
    elif bit > 15000:
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
            if (byte >= 32 or byte == 10 or byte == 13) and byte != 255:
                sys.stdout.write(chr(byte))
            else:
                sys.stdout.write('<' + str(byte) + '>')
        state = 0	# idle
    pos += 1


if options.audio_system == 'alsa':
    if options.audio_device:
        sound = AlsaSound(options.audio_device)
    else:
        sound = AlsaSound()
elif options.audio_system == 'portaudio':
    if options.audio_device:
        sound = PortSound(int(options.audio_device))
    else:
        sound = PortSound()
else:
    sys.stderr.write('invalid audio interface "' + options.audio_system + '"\n')
    sys.exit(1)

if options.list_devices:
    print '\nAvailable audio devices:'
    print sound.device_list()
    sys.exit(0)

prev = 32768
pprev = 32768

file = open("sample.raw", "w")
try:

    while True:
        try:
            sample_stream = sound.read()
        except IOError, e:
            print 'dropped', e

        file.write(sample_stream)

        sample_values = struct.unpack(str(sample_buf_size) + 'h', sample_stream)
        if options.vu_meter:
            width = 75
            max = 0
            min = 0
            for i in sample_values:
                if i > max:
                    max = i
                if i < min:
                    min = i
            len = width * (max - min) / 65536
            sys.stderr.write('|' + '*' * len + ' ' * (width - len) + '|\r')
        else:
            for i in sample_values:
                if not options.invert:
                    process_sample(pprev, prev, i)
                else:
                    process_sample(-pprev, -prev, -i)
                pprev = prev
                prev = i

except KeyboardInterrupt:
    sound.close()
