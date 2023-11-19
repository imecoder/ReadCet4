import os
import urllib.request

def download(_word):
    file = 'build/debug/audio/' + _word + '.mp3'
    if not os.path.exists(file):
        url = 'https://dict.youdao.com/dictvoice?audio=' + _word + '&type=2'
        print(url)
        urllib.request.urlretrieve(url, filename=file)
        print(_word + ".mp3 下载完成")


if __name__ == '__main__':
    with open('allwords.txt', 'r', encoding='utf-8') as f:
        for line in f:
            print(line)
            word = line.strip().split(' ')[0]
            download(word)