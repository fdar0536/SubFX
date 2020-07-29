# This file is part of SubFX,
# Copyright (c) 2020 fdar0536
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys
from SubFX_AssPy import *

ass_buf = []
text_start = "\\r" + an(5)

def lineEffect(dialog, line):
    """
    Here just simplely fade in and fade out
    """
    start = line.start_time
    end = line.end_time
    effect = text_start
    effect += fad(150, 150)
    effect += pos(line.center, line.middle)
    ass_main(ass_buf, \
             SubL(start, end, 0, "default"), \
             effect, line.text_stripped)

def sylEffect(line, syl):
    """
    Here just simplely fade in and fade out per syllable or character
    """
    if not is_syl_blank(syl):
        start = line.start_time - 300 + 50 * syl.i
        end = line.start_time + syl.start_time
        effect = text_start
        effect += fad(150, 0)
        effect += pos(syl.center, syl.middle)
        ass_main(ass_buf, \
                 SubL(start, end, 0, "default"), \
                 effect, syl.text)
        
        start = line.start_time + syl.start_time
        end = line.start_time + syl.end_time
        effect = text_start
        effect += pos(syl.center, syl.middle)
        ass_main(ass_buf, \
                 SubL(start, end, 0, "default"), \
                 effect, syl.text)
        
        start = line.start_time + syl.end_time
        end = line.end_time - 300 + 50 * syl.i
        effect = text_start
        effect += fad(0, 150)
        effect += pos(syl.center, syl.middle)
        ass_main(ass_buf, \
                 SubL(start, end, 0, "default"), \
                 effect, syl.text)

def main():
    print("parsering ass...", end="")

    # Yutils is defined in SubFX_AssPy(the source code is asspy.py)
    try:
        parser = Yutils.AssParser.create("your_own_input.ass")
    except:
        print("Fail to parse ass file", file=sys.stderr)
        sys.exit(1)

    print("done")

    print("Extending dialogs...", end="")
    try:
        parser.extendDialogs()
    except:
        print("Fail to extend dialogs", file=sys.stderr)
        sys.exit(1)
    
    print("done")
    dialogs = parser.dialogs()
    total = len(dialogs)
    for i in range(0, total):
        dialog = dialogs[i]
        if i % 3 == 0:
            # line
            lineEffect(dialog, dialog)
        elif (i % 3 == 1):
            # syl
            syls = dialog.syls
            for si in range(0, len(syls)):
                sylEffect(dialog, syls[si])
        else:
            # char
            chars = dialog.chars
            for ci in range(0, len(chars)):
                sylEffect(dialog, chars[ci])
        print("progress: {0:.2f}%".format((i + 1) / total * 100), \
              end = "\r", flush = True)
    
    print("\nWriting new ass...", end = "")
    try:
        Yutils.AssWriter.write("your_own_output.ass", \
                               parser.meta(), \
                               parser.styles(), ass_buf)
    except:
        print("Fail to write ass file", file=sys.stderr)
        sys.exit(1)
    
    print("done")
    sys.exit(0)

if __name__ == "__main__":
    main()