import os
import urllib.request
from playsound import playsound
import sys
from PyQt5.QtWidgets import QApplication, QWidget

def readword(word):
    file = 'audio/' + word + '.mp3'
    if os.path.exists(file):
        print("文件存在")
    else:
        url = 'https://dict.youdao.com/dictvoice?audio=' + word + '&type=1'
        print(url)
        urllib.request.urlretrieve(url, filename=file)
        print(word + ".mp3 下载完成")

    playsound(file)


def main():
    app = QApplication(sys.argv)

    w = QWidget()
    w.resize(800, 600)
    w.move(300, 300)
    w.setWindowTitle('Simple')
    w.show()

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
    # with open('level4.txt', 'r') as f:
    #     for line in f:
    #         print(line.strip())
    #         readword(line.strip())
