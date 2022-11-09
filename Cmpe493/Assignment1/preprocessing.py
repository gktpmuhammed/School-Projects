from bs4 import BeautifulSoup
import string
import os


# In this method punctuations will be removed. All strings will be in lower case.
# Lastly if string contains any stopwords, they will be removed from the string
def normalize(raw_text, stopwords):
    punc_text = raw_text.translate(raw_text.maketrans("", "", string.punctuation))
    casefold_text = punc_text.casefold()
    norm_text = list()
    for word in casefold_text.split():
        if word not in stopwords:
            if word not in norm_text:
                norm_text.append(word)

    return norm_text


def pre_processing():
    stopwords_file = open("stopwords.txt", 'r')
    stopwords = stopwords_file.read().splitlines()  # I have stored stopwords in this variable
    index_dict = {}  # I am keeping indexes of the words in here
    reuters = os.path.join(os.getcwd(), 'reuters21578')
    for file in os.listdir(reuters):  # I am selecting files in reuters21578 directory
        if file.endswith(".sgm"):  # I am filtering .sgm files
            with open(os.path.join(reuters, file), "r", encoding="latin-1") as f:
                data = f.read()  # File read as requested encoding
                soup = BeautifulSoup(data, "html.parser")  # I used a third party library to ease .sgm file read
                news_list = soup.findAll("reuters")  # Stored all the news in a list
                for news in news_list:
                    news_id = news.get("newid")  # Traversed all news and got their ids
                    news_title = news.find("title")  # Traversed all news and got their titles
                    news_body = news.find("body")  # Traversed all news and got their bodys
                    body = ""
                    title = ""
                    if news_title:  # If news has a title then take the title in string format
                        title = news_title.text
                    if news_body:  # If news has a body then take the body in string format
                        body = news_body.text
                    raw_text = title + " " + body  # Concatenate title and body for normalization process
                    norm_text = normalize(raw_text, stopwords)  # Normalized text returned

                    for word in norm_text:
                        if word in index_dict:  # If this word already in dictionary then append new id to that key
                            index_dict[word].append(int(news_id))
                        else:  # If this word does not exist in dictionary then create new element in dictionary
                            index_dict[word] = [int(news_id)]
    # this part writes each key and their values line by line to the file named "myIndex.txt"
    with open("myIndex.txt", 'w') as g:
        for key, value in index_dict.items():
            g.write('%s:%s\n' % (key, value))


pre_processing()
