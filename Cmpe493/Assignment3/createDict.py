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


# splits data as Training data and testing data by only looking at topic and lewissplit fields.
# it counts the number of documents in each classes and determines biggest 10 class.
def split_data():
    reuters = os.path.join(os.getcwd(), 'reuters21578')
    class_dict = {}
    train_data = dict()
    test_data = dict()
    for file in os.listdir(reuters):  # I am selecting files in reuters21578 directory
        if file.endswith(".sgm"):  # I am filtering .sgm files
            with open(os.path.join(reuters, file), "r", encoding="latin-1") as f:
                data = f.read()  # File read as requested encoding
                soup = BeautifulSoup(data, "html.parser")  # I used a third party library to ease .sgm file read
                news_list = soup.findAll("reuters")  # Stored all the news in a list
                for news in news_list:
                    isTopic = news.get("topics")  # Traversed all news and got their topics
                    isTrain = news.get("lewissplit")
                    news_id = int(news.get("newid"))  # Traversed all news and got their ids
                    news_title = news.find("title")  # Traversed all news and got their titles
                    news_body = news.find("body")  # Traversed all news and got their bodys
                    body = ""
                    title = ""
                    if news_title:  # If news has a title then take the title in string format
                        title = news_title.text
                    if news_body:  # If news has a body then take the body in string format
                        body = news_body.text
                    raw_text = title + " " + body  # Concatenate title and body for normalization process
                    if raw_text == ' ':
                        continue
                    topic_list = list()
                    if isTopic == "YES":
                        news_topic = ""
                        temp_topic_list = list()
                        news_topic = news.find("topics")
                        if "<d>" in str(news_topic):
                            temp_topic_list = news_topic.find_all('d')
                            for topic in temp_topic_list:
                                a = str(topic).replace("<d>", "")
                                b = a.replace("</d>", "")
                                topic_list.append(b)
                            if isTrain == "TRAIN":

                                for top in topic_list:
                                    if top in class_dict:
                                        class_dict[top] += 1
                                    else:
                                        class_dict[top] = 1
                                train_data[news_id] = {"class": topic_list, "text": raw_text}
                            elif isTrain == "TEST":

                                test_data[news_id] = {"class": topic_list, "text": raw_text}
    print("All classes included Training Data: ", len(train_data))
    print("All classes included Testing Data: ", len(test_data))
    class_dict = sorted(class_dict.items(), key=lambda x: x[1], reverse=True)
    top10_topics = list()
    class_probs = dict()
    for k in class_dict[:10]:
        top10_topics.append(k[0])
        class_probs[k[0]] = math.log(k[1] / len(train_data))
    print(class_dict)
    print(top10_topics)
    print(class_probs)
    return train_data, test_data, class_dict, class_probs


def mnb_test_train_data(train_data, test_data, class_probs):
    stopwords_file = open("stopwords.txt", 'r')
    stopwords = stopwords_file.read().splitlines()  # I have stored stopwords in this variable
    MNB_TRAINING_SET = dict()
    MNB_TEST_SET = dict()
    class_dict = dict()
    class_dict_test = dict()
    mult_occ_of_class_train = 0
    mult_occ_of_class_test = 0

    vocabulary = {"vocab": {}, "length": 0}
    for el in class_probs:
        MNB_TRAINING_SET[el] = {"vocab": {}, "length": 0}
    train_count = 0
    for news_id in train_data:
        valid_classes_train = list()
        added = False
        for topic in train_data[news_id]["class"]:
            if topic in class_probs:
                valid_classes_train.append(topic)

                if not added:
                    train_count += 1
                    added = True
                if topic in class_dict:
                    class_dict[topic] += 1
                else:
                    class_dict[topic] = 1
                norm_text = normalize(train_data[news_id]["text"], stopwords)  # Normalized text returned
                for word in norm_text:
                    if word in vocabulary["vocab"]:
                        vocabulary["vocab"][word] += 1
                    else:
                        vocabulary["vocab"][word] = 1
                        vocabulary["length"] += 1

                    if word in MNB_TRAINING_SET[topic]["vocab"]:
                        MNB_TRAINING_SET[topic]["vocab"][word]["count"] += 1
                    else:
                        MNB_TRAINING_SET[topic]["vocab"][word] = {"count": 1, "prob": 0}

                    MNB_TRAINING_SET[topic]["length"] += 1
        if valid_classes_train:
            if len(valid_classes_train) > 1:
                mult_occ_of_class_train += 1
    for news_id in test_data:
        norm_text = normalize(test_data[news_id]["text"], stopwords)  # Normalized text returned

        valid_classes = list()
        for nw in test_data[news_id]["class"]:
            if nw in class_probs:
                valid_classes.append(nw)
                if nw in class_dict_test:
                    class_dict_test[nw] += 1
                else:
                    class_dict_test[nw] = 1
        if valid_classes:
            MNB_TEST_SET[news_id] = {"class": valid_classes, "words": {}}
            if len(valid_classes) > 1:
                mult_occ_of_class_test += 1
            for word in norm_text:
                if word in MNB_TEST_SET[news_id]["words"]:
                    MNB_TEST_SET[news_id]["words"][word] += 1
                else:
                    MNB_TEST_SET[news_id]["words"][word] = 1

    for clas in MNB_TRAINING_SET:
        for word in MNB_TRAINING_SET[clas]["vocab"]:
            MNB_TRAINING_SET[clas]["vocab"][word]["prob"] = math.log(
                (MNB_TRAINING_SET[clas]["vocab"][word]["count"] + 1) / (
                        MNB_TRAINING_SET[clas]["length"] + vocabulary["length"]))
    print("Top 10 classes included Training Data: ", train_count)
    print("Top 10 classes included Testing Data: ", len(MNB_TEST_SET))
    print("mnb class TRAIN: ", class_dict)
    print("mnb class TEST: ", class_dict_test)
    class_dict = sorted(class_dict.items(), key=lambda x: x[1], reverse=True)
    class_probs_after = dict()
    for k in class_dict[:10]:
        class_probs_after[k[0]] = math.log(k[1] / len(train_data))
    print("After elim:", class_probs_after)
    print("Vocab Size: ", vocabulary["length"])
    print("train_multiple: ", mult_occ_of_class_train)
    print("test_multiple: ", mult_occ_of_class_test)
    print("len(train_data)", len(train_data))
    return MNB_TRAINING_SET, MNB_TEST_SET, vocabulary


def idf_calculator(key, KNN_TRAINING_SET):
    result = KNN_TRAINING_SET[key]
    counterdf = len(result)
    return math.log((len(KNN_TRAINING_SET["news_id"]) - 1) / counterdf, 10)


def lenght_calculator(i, KNN_TRAINING_SET):
    count = 0
    for key, values in KNN_TRAINING_SET.items():
        if i in values and key != "news_id":
            doctfidf = (1 + math.log(values[i], 10)) * idf_calculator(key, KNN_TRAINING_SET)
            count += doctfidf ** 2
    return math.sqrt(count)


def knn_test_train_data(train_data, class_probs):
    stopwords_file = open("stopwords.txt", 'r')
    stopwords = stopwords_file.read().splitlines()  # I have stored stopwords in this variable
    KNN_TRAINING_SET = dict()
    KNN_TRAINING_SET["news_id"] = {}
    for news_id in train_data:
        valid_classes = list()
        for nw in train_data[news_id]["class"]:
            if nw in class_probs:
                valid_classes.append(nw)
        if valid_classes:
            norm_text = normalize(train_data[news_id]["text"], stopwords)  # Normalized text returned
            KNN_TRAINING_SET["news_id"][news_id] = {}
            KNN_TRAINING_SET["news_id"][news_id]["length"] = len(norm_text)
            KNN_TRAINING_SET["news_id"][news_id]["labels"] = valid_classes

            for word in norm_text:
                if word in KNN_TRAINING_SET:
                    if news_id in KNN_TRAINING_SET[word]:
                        KNN_TRAINING_SET[word][news_id] += 1
                    else:
                        KNN_TRAINING_SET[word][news_id] = 1
                else:
                    KNN_TRAINING_SET[word] = {}
                    KNN_TRAINING_SET[word][news_id] = 1

    for i in KNN_TRAINING_SET["news_id"]:
        KNN_TRAINING_SET["news_id"][i]["length"] = lenght_calculator(i, KNN_TRAINING_SET)
    return KNN_TRAINING_SET


def pre_processing():
    train_data, test_data, class_dict, class_probs = split_data()
    MNB_TRAINING_SET, MNB_TEST_SET, MNB_vocabulary = mnb_test_train_data(train_data, test_data, class_probs)
    KNN_TRAINING_SET = knn_test_train_data(train_data, class_probs)
    print("KNN_TRAINING_SET Size: ", len(KNN_TRAINING_SET["news_id"]))
    with open("MNB_TRAINING_SET.json", 'w', encoding="latin-1") as g:
        json.dump(MNB_TRAINING_SET, g, indent=4)
    with open("MNB_TEST_SET.json", 'w', encoding="latin-1") as g:
        json.dump(MNB_TEST_SET, g, indent=4)
    with open("MNB_vocabulary.json", 'w', encoding="latin-1") as g:
        json.dump(MNB_vocabulary, g, indent=4)
    with open("MNB_Class_Probs.json", 'w', encoding="latin-1") as g:
        json.dump(class_probs, g, indent=4)
    with open('KNN_TRAINING_SET.json', 'w', encoding='latin-1') as g:
        json.dump(KNN_TRAINING_SET, g, indent=4)


pre_processing()
