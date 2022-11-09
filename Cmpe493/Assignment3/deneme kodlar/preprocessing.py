from bs4 import BeautifulSoup
import string
import os
import json
import math


# In this method punctuations will be removed. All strings will be in lower case.
# Lastly if string contains any stopwords, they will be removed from the string
def normalize(raw_text, stopwords):
    punc_text = raw_text.translate(raw_text.maketrans("", "", string.punctuation))
    casefold_text = punc_text.casefold()
    norm_text = list()
    for word in casefold_text.split():
        if word not in stopwords:
            norm_text.append(word)
    return norm_text


def pre_processing():
    stopwords_file = open("stopwords.txt", 'r')
    stopwords = stopwords_file.read().splitlines()  # I have stored stopwords in this variable
    reuters = os.path.join(os.getcwd(), 'reuters21578')
    class_dict_train = {}
    class_dict_test = {}
    total_docs_train = 0
    for file in os.listdir(reuters):  # I am selecting files in reuters21578 directory
        if file.endswith(".sgm"):  # I am filtering .sgm files
            with open(os.path.join(reuters, file), "r", encoding="latin-1") as f:
                data = f.read()  # File read as requested encoding
                soup = BeautifulSoup(data, "html.parser")  # I used a third party library to ease .sgm file read
                news_list = soup.findAll("reuters")  # Stored all the news in a list
                for news in news_list:
                    isTopic = news.get("topics")  # Traversed all news and got their topics
                    isTrain = news.get("lewissplit")
                    topic_list = list()
                    if isTopic == "YES" and isTrain == "TRAIN":
                        total_docs_train += 1
                        news_topic = ""
                        temp_topic_list = list()
                        news_topic = news.find("topics")
                        if "<d>" in str(news_topic):
                            # print(news_topic.find_all('d'))
                            temp_topic_list = news_topic.find_all('d')
                            for topic in temp_topic_list:
                                a = str(topic).replace("<d>", "")
                                b = a.replace("</d>", "")
                                topic_list.append(b)
                            # print(topic_list)
                            for top in topic_list:
                                if top in class_dict_train:
                                    class_dict_train[top] += 1
                                else:
                                    class_dict_train[top] = 1
                    # class_dict_test
                    # elif isTopic == "YES" and isTrain == "TEST":

    class_dict_train = sorted(class_dict_train.items(), key=lambda x: x[1], reverse=True)
    top10_topics = list()
    class_probs = dict()
    for k in class_dict_train[:10]:
        top10_topics.append(k[0])
        class_probs[k[0]] = math.log(k[1] / total_docs_train)
    print(class_dict_train)
    print(top10_topics)
    print(class_probs)
    print("total_docs_train: ", total_docs_train)

    TRAINING_SET = dict()
    TEST_SET = dict()
    vocabulary = {"vocab": {}, "length": 0}
    for el in top10_topics:
        TRAINING_SET[el] = {"vocab": {}, "length": 0}

    print(TRAINING_SET)
    print(TEST_SET)
    for file in os.listdir(reuters):  # I am selecting files in reuters21578 directory
        if file.endswith(".sgm"):  # I am filtering .sgm files
            with open(os.path.join(reuters, file), "r", encoding="latin-1") as f:
                data = f.read()  # File read as requested encoding
                soup = BeautifulSoup(data, "html.parser")  # I used a third party library to ease .sgm file read
                news_list = soup.findAll("reuters")  # Stored all the news in a list
                for news in news_list:
                    isTopic = news.get("topics")  # Traversed all news and got their topics
                    isSplit = news.get("lewissplit")
                    # print(isSplit)
                    news_topic = ""
                    temp_topic_list = list()
                    topic_list = list()
                    if isTopic == "YES":
                        news_topic = news.find("topics")
                        if "<d>" in str(news_topic):
                            # print(news_topic.find_all('d'))
                            temp_topic_list = news_topic.find_all('d')
                            for topic in temp_topic_list:
                                a = str(topic).replace("<d>", "")
                                b = a.replace("</d>", "")
                                topic_list.append(b)
                            # print(topic_list)

                            for top in topic_list:
                                if top in top10_topics:
                                    news_id = news.get("newid")  # Traversed all news and got their ids
                                    news_id = int(news_id)
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
                                    if isSplit == "TRAIN":
                                        for word in norm_text:
                                            if word in vocabulary["vocab"]:
                                                vocabulary["vocab"][word] += 1
                                            else:
                                                vocabulary["vocab"][word] = 1
                                                vocabulary["length"] += 1

                                            if word in TRAINING_SET[top]["vocab"]:
                                                TRAINING_SET[top]["vocab"][word]["count"] += 1
                                            else:
                                                TRAINING_SET[top]["vocab"][word] = {"count": 1, "prob": 0}

                                            TRAINING_SET[top]["length"] += 1

                                    elif isSplit == "TEST":
                                        if news_id not in TEST_SET:
                                            valid_clases = list()
                                            for nw in topic_list:
                                                if nw in top10_topics:
                                                    valid_clases.append(nw)
                                            TEST_SET[news_id] = {"class": valid_clases, "words": {}}

                                        for word in norm_text:
                                            if word in TEST_SET[news_id]["words"]:
                                                TEST_SET[news_id]["words"][word] += 1
                                            else:
                                                TEST_SET[news_id]["words"][word] = 1

    for clas in TRAINING_SET:
        for word in TRAINING_SET[clas]["vocab"]:
            TRAINING_SET[clas]["vocab"][word]["prob"] = math.log((TRAINING_SET[clas]["vocab"][word]["count"] + 1) / (
                    TRAINING_SET[clas]["length"] + vocabulary["length"]))

    with open("training_set.json", 'w', encoding="latin-1") as g:
        json.dump(TRAINING_SET, g, indent=4)
    with open("test_set.json", 'w', encoding="latin-1") as g:
        json.dump(TEST_SET, g, indent=4)
    with open("vocabulary.json", 'w', encoding="latin-1") as g:
        json.dump(vocabulary, g, indent=4)
    with open("class_probs.json", 'w', encoding="latin-1") as g:
        json.dump(class_probs, g, indent=4)


pre_processing()
