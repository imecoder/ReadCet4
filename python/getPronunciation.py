import cmudict

# 加载音标数据
d = cmudict.dict()

# 输入单词
word = 'dog'

# 获取音标
phones = d[word.lower()]

# 打印音标
print(phones)