import urllib.request

_word = 'cash'
file = 'build/debug/audio/' + _word + '.mp3'
url = 'https://dict.youdao.com/dictvoice?audio=' + _word + '&type=1'
print(url)
urllib.request.urlretrieve(url, filename=file)
print("下载完成")
