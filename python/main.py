import os
import urllib.request
import sys
from PyQt5.QtWidgets import QApplication, QWidget
import pygame


# 删除字符串中以start开始，以end结尾的内容
# def deleteByStartAndEnd(s, start, end):
#     x1 = s.index(start)
#     x2 = s.rfind(end)+1
#     x3 = s[x1:x2]
#     # print(x3)
#     result = s.replace(x3, "")
#     return result

pygame.mixer.init()


def readword(_word):
    file = 'audio/' + _word + '.mp3'
    if not os.path.exists(file):
        url = 'https://dict.youdao.com/dictvoice?audio=' + _word + '&type=1'
        print(url)
        urllib.request.urlretrieve(url, filename=file)
        print(_word + ".mp3 下载完成")

    pygame.mixer.music.load(file)
    pygame.mixer.music.play()
    while pygame.mixer.music.get_busy():
        continue
    # playsound(file)


def main():
    app = QApplication(sys.argv)

    w = QWidget()
    w.resize(800, 600)
    w.move(300, 300)
    w.setWindowTitle('Simple')
    w.show()

    sys.exit(app.exec_())


if __name__ == '__main__':
    # main()
    with open('cet4.txt', 'r', encoding='utf-8') as f:
        for line in f:
            print(line)
            word = line.strip().split(' ')[0]
            readword(word)
            # getPronunciation(_word)
            # time.sleep(0.01)
