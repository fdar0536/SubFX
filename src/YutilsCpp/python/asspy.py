# This file is based on tcaxPy.
# Copyright (c) 2008-2012 milkyjing (milkyjing@gmail.com).
# Copyright (c) 2019-2020 fdar0536
#
# SubFX is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# SubFX is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with SubFX. If not, see <http://www.gnu.org/licenses/>.

######################################### Modules ###########################################################

import sys

if (sys.platform == "win32"):
    info = sys.version_info
    if (info.major >= 3 and info.minor >= 8): # python >= 3.8
        import os
        os.add_dll_directory(os.path.join(os.environ["SUBFX_HOME"], "bin"))
        os.add_dll_directory(os.path.join(os.environ["ICU_HOME"], "bin"))

        try:
            import SubFX_YutilsPy as Yutils
        except ImportError:
            raise ImportError("Cannot import SubFX_YutilsPy")
    else:
        import SubFX_YutilsPy as Yutils
else:
    import SubFX_YutilsPy as Yutils

import math
import random

########################################## Main FX Function #################################################
def SubL(Start = 0, End = 0, Layer = 0, Style = "TCMS"):
    return "Dialogue: {0},{1},{2},{3},NTP,0000,0000,0000,,".format(int(Layer), FmtTime(Start), FmtTime(End), Style)

def ass_main(ASS_BUF, SubDlg = "", Event = "", Text = ""):
    if Event == "":
        ASS_BUF.append("{0}{1}{2}".format(SubDlg, Event, Text))
    else:
        ASS_BUF.append("{0}{{{1}}}{2}".format(SubDlg, Event, Text))

#############################################################################################################

######################################### Useful Funtions ###################################################

#--------------------------------------------- Format Function ----------------------------------------------#

# 格式化相关的函数

def FmtHex(n):                                   # dec to hex FormatHex
    if n <= 0:
        return "00"
    elif n >= 255:
        return "FF"
    else:
        return "{0:02X}".format(int(n))

def HexToDec(a):                                 # hex to dec
    return int(a, 16)

def FmtFloat(f):
    if f == int(f):
        F = str(int(f))
    else:
        F = str(format(f, ".2f"))
    return F

def FmtRGB(r, g, b):                             # return a formated RGB string
    return FmtHex(b) + FmtHex(g) + FmtHex(r)

def DeFmtRGB(CLR):                               # convert RGB string to RGB tuple
    CLR_LIS_3 = HexToDec(CLR[0:2])
    CLR_LIS_2 = HexToDec(CLR[2:4])
    CLR_LIS_1 = HexToDec(CLR[4:6])
    return (CLR_LIS_1, CLR_LIS_2, CLR_LIS_3)

def DecRGB(RGB):                                 # convert RGB string to RGB dec value
    return int("0x" + RGB, 16)

def clip_0_255(a):
    if a < 0:
        return 0
    elif a > 255:
        return 255
    else:
        return int(a)

def MakeRGB(r, g, b):
    return clip_0_255(r) + clip_0_255(g) * 256 + clip_0_255(b) * 65536

def MakeRGBA(r, g, b, a):
    return clip_0_255(r) + clip_0_255(g) * 256 + clip_0_255(b) * 65536 + clip_0_255(a) * 16777216

#-------------------------------- Ass tag functions -------------------------------#

def an(a):
    return "\\an{0}".format(int(a))

def K(a):
    return "\\K{0}".format(int(a))

def k(a):
    return "\\k{0}".format(int(a))

def ko(a):
    return "\\ko{0}".format(int(a))

def kf(a):
    return "\\kf{0}".format(int(a))

def t(a1, a2 = None, a3 = None, a4 = None):
    if a2 == None:
        return "\\t({code})".format(code = a1)
    elif a3 == None:
        if a1 == int(a1):
            A = str(int(a1))
        else:
            A = str(format(a1, ".2f"))
        return "\\t({a},{code})".format(a = A, code = a2)
    elif a4 == None:
        return "\\t({t1},{t2},{code})".format(t1 = int(a1), t2 = int(a2), code = a3)
    else:
        if a3 == int(a3):
            A = str(int(a3))
        else:
            A = str(format(a3, ".2f"))
        return "\\t({t1},{t2},{a},{code})".format(t1 = int(a1), t2 = int(a2), a = A, code = a4)

def fscx(x):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    return "\\fscx{0}".format(X)

def fscy(y):
    if y == int(y):
        Y = str(int(y))
    else:
        Y = str(format(y, ".2f"))
    return "\\fscy{0}".format(Y)

def fsc(x, y):
    if x == int(x) and y == int(y):
        X = str(int(x))
        Y = str(int(y))
    else:
        X = str(format(x, ".2f"))
        Y = str(format(y, ".2f"))
    return "\\fscx{0}\\fscy{1}".format(X, Y)

def fn(a):
    return "\\fn{0}".format(a)

def fs(x):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    return "\\fs{0}".format(X)

def fad(t1, t2):
    return "\\fad({0},{1})".format(int(t1), int(t2))

def fade(a1, a2, a3, t1, t2, t3, t4):
    return "\\fade({0},{1},{2},{3},{4},{5},{6})".format(clip_0_255(a1), clip_0_255(a2), clip_0_255(a3), int(t1), int(t2), int(t3), int(t4))

def bord(x):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    return "\\bord{0}".format(X)

def shad(x):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    return "\\shad{0}".format(X)

def blur(x):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    return "\\blur{0}".format(X)

def p(a):
    return "\\p{0}".format(int(a))

def b(a):
    return "\\b{0}".format(int(a))

def be(x):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    return "\\be{0}".format(X)

def xbord(x):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    return "\\xbord{0}".format(X)

def ybord(y):
    if y == int(y):
        Y = str(int(y))
    else:
        Y = str(format(y, ".2f"))
    return "\\ybord{0}".format(Y)

def xshad(x):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    return "\\xshad{0}".format(X)

def yshad(y):
    if y == int(y):
        Y = str(int(y))
    else:
        Y = str(format(y, ".2f"))
    return "\\yshad{0}".format(Y)

def fax(x):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    return "\\fax{0}".format(X)

def fay(y):
    if y == int(y):
        Y = str(int(y))
    else:
        Y = str(format(y, ".2f"))
    return "\\fay{0}".format(Y)

def frx(a):
    if a == int(a):
        A = str(int(a))
    else:
        A = str(format(a, ".2f"))
    return "\\frx{0}".format(A)

def fry(a):
    if a == int(a):
        A = str(int(a))
    else:
        A = str(format(a, ".2f"))
    return "\\fry{0}".format(A)

def frz(a):
    if a == int(a):
        A = str(int(a))
    else:
        A = str(format(a, ".2f"))
    return "\\frz{0}".format(A)

def fr(a):
    if a == int(a):
        A = str(int(a))
    else:
        A = str(format(a, ".2f"))
    return "\\fr{0}".format(A)

def pos(x, y):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    if y == int(y):
        Y = str(int(y))
    else:
        Y = str(format(y, ".2f"))
    return "\\pos({0},{1})".format(X, Y)

def org(x, y):
    if x == int(x):
        X = str(int(x))
    else:
        X = str(format(x, ".2f"))
    if y == int(y):
        Y = str(int(y))
    else:
        Y = str(format(y, ".2f"))
    return "\\org({0},{1})".format(X, Y)

def alpha(a):                                    # note that a is a dec value
    return "\\alpha&H{0}&".format(FmtHex(a))

def alpha1(a):
    return "\\1a&H{0}&".format(FmtHex(a))

def alpha2(a):
    return "\\2a&H{0}&".format(FmtHex(a))

def alpha3(a):
    return "\\3a&H{0}&".format(FmtHex(a))

def alpha4(a):
    return "\\4a&H{0}&".format(FmtHex(a))

def color(c):                                    # note that c is a RGB string
    return "\\c&H{0}&".format(c)

def color1(c):
    return "\\1c&H{0}&".format(c)

def color2(c):
    return "\\2c&H{0}&".format(c)

def color3(c):
    return "\\3c&H{0}&".format(c)

def color4(c):
    return "\\4c&H{0}&".format(c)

def move(x1, y1, x2, y2, t1 = None, t2 = None):
    if x1 == int(x1) and y1 == int(y1) and x2 == int(x2) and y2 == int(y2):
        X1 = str(int(x1))
        Y1 = str(int(y1))
        X2 = str(int(x2))
        Y2 = str(int(y2))
    else:
        X1 = str(format(x1, ".2f"))
        Y1 = str(format(y1, ".2f"))
        X2 = str(format(x2, ".2f"))
        Y2 = str(format(y2, ".2f"))
    if t2 == None:
        return "\\move({0},{1},{2},{3})".format(X1, Y1, X2, Y2)
    else:
        return "\\move({0},{1},{2},{3},{4},{5})".format(X1, Y1, X2, Y2, int(t1), int(t2))

def clip(a1, a2 = None, a3 = None, a4 = None):
    if a2 == None:
        return "\\clip({draw})".format(draw = a1)
    elif a3 == None:
        return "\\clip({scale},{draw})".format(scale = a1, draw = a2)
    else:
        return "\\clip({x1},{y1},{x2},{y2})".format(x1 = int(a1), y1 = int(a2), x2 = int(a3), y2 = int(a4))

def iclip(a1, a2 = None, a3 = None, a4 = None):
    if a2 == None:
        return "\\iclip({draw})".format(draw = a1)
    elif a3 == None:
        return "\\iclip({scale},{draw})".format(scale = a1, draw = a2)
    else:
        return "\\iclip({x1},{y1},{x2},{y2})".format(x1 = int(a1), y1 = int(a2), x2 = int(a3), y2 = int(a4))

#--------------------------------------------- Utility Function -------------------------------------------#

def GetVersion():
    print("SubFX version: " + Yutils.version())

def Pause():
    print("Press any key to continue...")
    sys.stdin.readline()

def GetRootDir():
    if sys.platform == "win32":
        return __file__.rsplit("\\", 1)[0]
    else:
        return "/"

def GetWorkingDir():
    return sys.path[0]

def abspath(filename):
    return sys.path[0] + "\\" + filename

def MakePath(FolderIndex = 0, ImageIndex = 0, MainFolder = "src",
             SubFolder = "list", ImageName = "img", ImageType = ".png", PathType = "pi"):

    if (sys.platform == "win32"):
        if PathType == "pi":
            img_path = r"%s\%s%d\%s%04d%s" % (MainFolder, SubFolder, FolderIndex, ImageName, ImageIndex, ImageType)
        elif PathType == "sys":
            img_path = r"%s\%s%d\%s (%d)%s" % (MainFolder, SubFolder, FolderIndex, ImageName, ImageIndex, ImageType)
        else:
            img_path = r"%s\%s%d\%s%04d%s" % (MainFolder, SubFolder, FolderIndex, ImageName, ImageIndex, ImageType)
    else:
        if PathType == "pi":
            img_path = "%s/%s%d/%s%04d%s" % (MainFolder, SubFolder, FolderIndex, ImageName, ImageIndex, ImageType)
        elif PathType == "sys":
            img_path = "%s/%s%d/%s (%d)%s" % (MainFolder, SubFolder, FolderIndex, ImageName, ImageIndex, ImageType)
        else:
            img_path = "%s/%s%d/%s%04d%s" % (MainFolder, SubFolder, FolderIndex, ImageName, ImageIndex, ImageType)

    return img_path

def is_syl_blank(syl):
    if syl.duration <= 0:
        return True
    
    if len(syl.text) <= 0:
        return True
    
    tmp_str = syl.text
    tmp_str = tmp_str.strip()
    tmp_str = tmp_str.replace(" ", "")
    tmp_str = tmp_str.replace("　", "")
    return (len(tmp_str) <= 0)

#--------------------------------------------- Advanced Function -------------------------------------------#

def Sum(LIS, I, diff = 0):       # 高级加法函数 在数列LIS里 从第一项累加到第I项
    SUM = 0
    for i in range(I):
        SUM += LIS[i] + diff
    return SUM

def AdvInt(a):       # 高级取整函数 AdvancedIntegrate
    if a >= 0 and a - int(a) < 0.5:
        return int(a)
    elif a >= 0 and a - int(a) >= 0.5:
        return int(a) + 1
    elif a < 0 and a - int(a) <= -0.5:
        return int(a) - 1
    else:                # elif a < 0 and a - int(a) > -0.5:
        return int(a)

def DeFmtTime(TIME):       # 重新数字化已被格式化的时间 TIME = "0:00:00.00"
    return Yutils.Ass.stringToMs(TIME)

def FmtTime(t):       # 格式化时间
    return Yutils.Ass.msToString(t)

def PixPt():       # 返回一个像素点 PixelPoint, deprecated, use DrawPoint instead
    return "{\\p1}m 0 0 l 1 0 1 1 0 1{\\p0}"

def DrawPoint():
    return "{\\p1}m 0 0 l 1 0 1 1 0 1{\\p0}"

def DrawLight(l):
    light = "{\\p4}"
    l *= 8
    light += "m 0 0 l -{length} 0 0 8 c".format(length = int(l))
    light += "{\\p0}"
    return light

def MovPxl():       # 移动单位个像素 MovePixel
    return Yutils.Math.randomway()

def GetDistance(x1, y1, x2, y2):
    return Yutils.Math.distance((x1 - x2), (y1 - y2))

def GetAngle(x, y, ox, oy):
    r = GetDistance(x, y, ox, oy)
    if r == 0:
        return 0
    a = math.acos((x - ox) / r)
    if y > oy:
        a = math.pi * 2 - a
    return a

def RandSign():
    if random.randint(0, 1) == 1:
        return 1
    else:
        return -1

def RandomGauss(l, h, g):
    sum = 0
    for i in range(g):
        sum += l + (h - l) * random.random()
    return sum / g

def RandomDouble(l, h):
    return l + (h - l) * random.random()

def RandGauss(l, h):
    sum = 0
    for i in range(6):
        sum += l + (h - l) * random.random()
    return sum / 6

def RandRGB():       # 返回一个随机的RGB值 RandomRGB
    return FmtRGB(random.randint(0, 255), \
                  random.randint(0, 255), \
                  random.randint(0, 255))

def RandA():       # 返回一个随机的Alpha值 RandomAlpha
    return FmtHex(random.randint(0, 255))

def RandImg(n, IMG_WD, IMG_HT):       # 随机图形 RandomImage
    STR_RAND_IMG = "m " + str(random.randint(0, IMG_WD)) + \
                   " " + str(random.randint(0, IMG_HT)) + " b "
    for i in range(6 * n):
        if i % 2 == 0:
            STR_RAND_IMG += str(random.randint(0, IMG_WD)) + " "
        else:
            STR_RAND_IMG += str(random.randint(0, IMG_HT)) + " "
    return STR_RAND_IMG + "c"

def RandCir(a, b, r):       # 随机圆分布函数 RandomCircle1
    R = random.randint(0, r)
    theta = random.randint(0, 2 * 314) / 100.0
    x = AdvInt(R * math.cos(theta) + a)
    y = AdvInt(R * math.sin(theta) + b)
    return (x, y)

def RandCir2(a, b, r1, r2):      # 随机环分布函数 RandomCircle2
    R = random.randint(r1, r2)
    theta = random.randint(0, 2 * 314) / 100.0
    x = AdvInt(R * math.cos(theta) + a)
    y = AdvInt(R * math.sin(theta) + b)
    return (x, y)

def RandCir3(a, b, r1, r2, theta1, theta2):      # 带缺口的随机环分布函数 RandomCircle3
    R = random.randint(r1, r2)
    Tmp = min(theta1, theta2)
    theta2 = max(theta1, theta2)
    theta1 = Tmp
    theta = random.randint(AdvInt(314 * theta1 / 180.0), \
                           AdvInt(314 * theta2 / 180.0)) / 100.0
    x = AdvInt(R * math.cos(theta) + a)
    y = AdvInt(R * math.sin(theta) + b)
    return (x, y)

def Cir(n, a, b, r):       # 圆函数
    CIR = []
    for i in range(n):
        theta = (n - i - 1) * 2 * math.pi / n
        x = AdvInt(r * math.cos(theta) + a)
        y = AdvInt(r * math.sin(theta) + b)
        CIR.append((x, y))
    return CIR

def Circle(n, a, b, r):       # 圆函数
    CIR = []
    for i in range(n):
        theta = (n - i - 1) * 2 * math.pi / n
        x = r * math.cos(theta) + a
        y = r * math.sin(theta) + b
        CIR.append((x, y))
    return CIR

def RandPolygon(r1, r2, v):
    s = "{\\p4}"
    vertex = []
    r1 *= 8
    r2 *= 8
    num = int(v)
    for i in range(num):
        vertex.append(RandomDouble(0, 2 * math.pi))
    vertex.sort()
    x = math.cos(vertex[0]) * r1
    y = math.sin(vertex[0]) * r2
    s += "m {0} {1} ".format(AdvInt(x), AdvInt(y))
    for i in range(1, num):
        x = math.cos(vertex[i]) * r1
        y = math.sin(vertex[i]) * r2
        s += "l {0} {1} ".format(AdvInt(x), AdvInt(y))
    s += "c{\\p0}"
    return s

def DevImg(IMG, DEVX, DEVY):       # 图形偏移 DeviateIamge
    NEW_IMG_LIS = IMG.split()
    NEW_IMG = "m " + str(int(NEW_IMG_LIS[1]) + DEVX) + " " + str(int(NEW_IMG_LIS[2]) + DEVY) + " b "
    n = len(NEW_IMG_LIS)
    for i in range(4, n - 1):
        if i % 2 == 0:
            NEW_IMG_LIS[i] = int(NEW_IMG_LIS[i]) + DEVX
            NEW_IMG += str(NEW_IMG_LIS[i]) + " "
        else:
            NEW_IMG_LIS[i] = int(NEW_IMG_LIS[i]) + DEVY
            NEW_IMG += str(NEW_IMG_LIS[i]) + " "
    return NEW_IMG + "c"

def RevImgX(IMG):       # 图形按照x轴对换 ReverseIamgeX
    NEW_IMG_LIS = IMG.split()
    NEW_IMG = "m " + str(-int(NEW_IMG_LIS[1])) + " " + NEW_IMG_LIS[2] + " b "
    n = len(NEW_IMG_LIS)
    for i in range(4, n - 1):
        if i % 2 == 0:
            NEW_IMG_LIS[i] = -int(NEW_IMG_LIS[i])
            NEW_IMG += str(NEW_IMG_LIS[i]) + " "
        else:
            NEW_IMG += NEW_IMG_LIS[i] + " "
    return NEW_IMG + "c"

def RevImgY(IMG):       # 图形按照y轴对换 ReverseIamgeY
    NEW_IMG_LIS = IMG.split()
    NEW_IMG = "m " + NEW_IMG_LIS[1] + " " + str(-int(NEW_IMG_LIS[2])) + " b "
    n = len(NEW_IMG_LIS)
    for i in range(4, n - 1):
        if i % 2 == 0:
            NEW_IMG += NEW_IMG_LIS[i] + " "
        else:
            NEW_IMG_LIS[i] = -int(NEW_IMG_LIS[i])
            NEW_IMG += str(NEW_IMG_LIS[i]) + " "
    return NEW_IMG + "c"

def DivClr(COLOR1, COLOR2, n):       # 拆分颜色 DivideColor
    CLR_LIS_11 = HexToDec(COLOR1[0:2])
    CLR_LIS_12 = HexToDec(COLOR1[2:4])
    CLR_LIS_13 = HexToDec(COLOR1[4:6])
    CLR_LIS_21 = HexToDec(COLOR2[0:2])
    CLR_LIS_22 = HexToDec(COLOR2[2:4])
    CLR_LIS_23 = HexToDec(COLOR2[4:6])
    CLR_LIS = []
    if n == 2:
        return [COLOR1, COLOR2]
    else:
        for i in range(0, n - 1):
            CLR_LIS_I1 = FmtHex(CLR_LIS_11 + (CLR_LIS_21 - CLR_LIS_11) * i / (n - 1))
            CLR_LIS_I2 = FmtHex(CLR_LIS_12 + (CLR_LIS_22 - CLR_LIS_12) * i / (n - 1))
            CLR_LIS_I3 = FmtHex(CLR_LIS_13 + (CLR_LIS_23 - CLR_LIS_13) * i / (n - 1))
            CLR_LIS.append(CLR_LIS_I1 + CLR_LIS_I2 + CLR_LIS_I3)
        CLR_LIS.append(COLOR2)
        return CLR_LIS

def Jump(P_START, P_END, P_TOP, T):         # 跳跃特效函数
    JUMP = []
    d = P_TOP[0]
    for i in range(T):
        a = (P_START[1] - P_TOP[1]) / float((P_START[0] - d) * (P_START[0] - d))
        P_X = AdvInt(P_START[0] + i * (P_END[0] - P_START[0]) / float(T))
        P_Y = AdvInt(a * (P_X - d) * (P_X - d) + P_TOP[1])
        JUMP.append([P_X, P_Y])
    return JUMP

def AssDrawOffset(draw, x, y):      # 平移ASS绘图代码
    return Yutils.Shape.move(draw, x, y)

#############################################################################################################
