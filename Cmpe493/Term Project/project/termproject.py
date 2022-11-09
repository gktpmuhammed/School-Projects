import numpy as np
from matplotlib import pyplot as plt
from sklearn.decomposition import PCA
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn import metrics
import re
import nltk
import pandas as pd
from nltk.stem import PorterStemmer
from sklearn.naive_bayes import MultinomialNB
from sklearn.neighbors import KNeighborsClassifier
from sklearn.pipeline import Pipeline
from sklearn.linear_model import SGDClassifier
from sklearn.model_selection import GridSearchCV
from sklearn.preprocessing import MultiLabelBinarizer
import math
import string
import json
from sklearn.multiclass import OneVsRestClassifier
from sklearn.svm import SVC

porter_stemmer = PorterStemmer()


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


def my_cool_preprocessor(text):
    text = text.lower()
    text = re.sub("\\W", " ", text)  # remove special chars
    # text = re.sub("\\s+(in|the|all|for|and|on)\\s+", " _connector_ ", text)  # normalize certain words

    # stem words
    words = re.split("\\s+", text)
    stemmed_words = [porter_stemmer.stem(word=word) for word in words]
    return ' '.join(stemmed_words)


def load_data(filename, classes, tip):
    dataset = pd.read_csv(filename)
    list_dataset = list()
    targets = list()
    for i in range(dataset.shape[0]):
        str_line = str(dataset.loc[i, "title"])
        str_line += " " + str(dataset.loc[i, "abstract"])
        keywords = dataset.loc[i, 'keywords']
        if keywords != keywords:  # check if keywords nan
            keywords = ''
        str_line += " " + keywords
        label = str(dataset.loc[i, "label"]).split(';')
        if tip == "train":
            for lb in label:
                targets.append(classes.index(lb))
                list_dataset.append(str_line)
        elif tip == "test":
            targets.append(classes.index(label[0]))
            list_dataset.append(str_line)
    return list_dataset, targets


def load_data_knn_single(filename, classes, tip):
    dataset = pd.read_csv(filename)
    list_dataset = list()
    targets = list()
    for i in range(dataset.shape[0]):
        str_line = str(dataset.loc[i, "title"])
        str_line += " " + str(dataset.loc[i, "abstract"])
        keywords = dataset.loc[i, 'keywords']
        if keywords != keywords:  # check if keywords nan
            keywords = ''
        str_line += " " + keywords
        label = str(dataset.loc[i, "label"]).split(';')
        if tip == "train":
            lst = list()
            for lb in label:
                lst.append(classes.index(lb))
            targets.append(lst)
            list_dataset.append(str_line)
        elif tip == "test":
            lst = list()
            for lb in label:
                lst.append(classes.index(lb))
            targets.append(lst)
            list_dataset.append(str_line)
    return list_dataset, targets


def load_data_knn(filename,stopwords, tip):
    dataset = pd.read_csv(filename)
    list_dataset = list()
    targets = list()
    for i in range(dataset.shape[0]):
        str_line = str(dataset.loc[i, "title"])
        str_line += " " + str(dataset.loc[i, "abstract"])
        keywords = dataset.loc[i, 'keywords']
        if keywords != keywords:  # check if keywords nan
            keywords = ''
        str_line += " " + keywords
        label = str(dataset.loc[i, "label"]).split(';')
        if tip == "train":
            targets.append(label)
            list_dataset.append(str_line)
        elif tip == "test":
            targets.append(label)
            list_dataset.append(str_line)
    test_index_dict={}
    if tip=="test":
        for i in range(len(list_dataset)):
            norm_text = normalize(list_dataset[i], stopwords)
            test_index_dict[i] = {"class": targets[i], "words": norm_text}
        with open("KNN_TEST_SET.json", 'w', encoding="latin-1") as g:
            json.dump(test_index_dict, g)
    return list_dataset, targets


def mnb(train_dataset, train_targets, test_dataset, test_targets, stopwords, classes):
    text_clf = Pipeline([
        ('vect', CountVectorizer(preprocessor=my_cool_preprocessor, stop_words=stopwords)),
        ('tfidf', TfidfTransformer()),
        ('clf', MultinomialNB()),
    ])

    text_clf.fit(train_dataset, train_targets)

    predicted = text_clf.predict(test_dataset)
    print("--------------------------- Multinomial Naive Bayes Implementation ---------------------------------------")
    print(np.mean(predicted == test_targets))
    print(metrics.classification_report(test_targets, predicted,
                                        target_names=classes))


def svm(train_dataset, train_targets, test_dataset, test_targets, stopwords, classes):
    text_clf = Pipeline([
        ('vect', CountVectorizer(preprocessor=my_cool_preprocessor, stop_words=stopwords)),
        ('tfidf', TfidfTransformer()),
        ('clf', SGDClassifier(loss='hinge', penalty='l2',
                              alpha=1e-3, random_state=42,
                              max_iter=5, tol=None)),
    ])

    text_clf.fit(train_dataset, train_targets)
    predicted = text_clf.predict(test_dataset)

    print("--------------------------- Support Vector Machine Implementation ---------------------------------------")
    print(np.mean(predicted == test_targets))
    print(metrics.classification_report(test_targets, predicted,
                                        target_names=classes))
    metrics.confusion_matrix(test_targets, predicted)
    grid_search(text_clf, test_dataset, test_targets, classes)


def grid_search(text_clf, list_dataset, targets, classes):
    parameters = {
        'vect__ngram_range': [(1, 1), (1, 2)],
        'tfidf__use_idf': (True, False),
        'clf__alpha': (1e-2, 1e-3),
    }

    gs_clf = GridSearchCV(text_clf, parameters, cv=5, n_jobs=-1)
    gs_clf = gs_clf.fit(list_dataset[:400], targets[:400])

    print(classes[gs_clf.predict([list_dataset[0]])[0]])
    print(gs_clf.best_score_)

    for param_name in sorted(parameters.keys()):
        print("%s: %r" % (param_name, gs_clf.best_params_[param_name]))


def self_mnb(train_dataset, train_targets, test_dataset, test_targets, stopwords, classes):
    Num_Docs = len(train_dataset)
    p_docs_dict = dict()
    train_index_dict = dict({'Treatment': {}, 'Mechanism': {}, 'Prevention': {}, 'Case Report': {}, 'Diagnosis': {},
                             'Transmission': {}, 'Epidemic Forecasting': {}})
    for el in classes:
        train_index_dict[el] = {"vocab": {}, "length": 0}
    vocabulary = {"vocab": {}, "length": 0}
    for i in range(Num_Docs):
        # norm_text = normalize(train_dataset[i], stopwords)
        norm_text = train_dataset[i].split()
        if classes[train_targets[i]] in p_docs_dict:
            p_docs_dict[classes[train_targets[i]]] += 1
        else:
            p_docs_dict[classes[train_targets[i]]] = 1
        for word in norm_text:
            if word in vocabulary["vocab"]:
                vocabulary["vocab"][word] += 1
            else:
                vocabulary["vocab"][word] = 1
                vocabulary["length"] += 1

            if word in train_index_dict[classes[train_targets[i]]]["vocab"]:
                train_index_dict[classes[train_targets[i]]]["vocab"][word]["count"] += 1
            else:
                train_index_dict[classes[train_targets[i]]]["vocab"][word] = {"count": 1, "prob": 0}

            train_index_dict[classes[train_targets[i]]]["length"] += 1

    allWordSet = set()
    for label in list(train_index_dict.keys()):
        allWordSet = allWordSet.union(list(train_index_dict[label]["vocab"].keys()))

    totWordsLength = len(allWordSet)
    for label in list(train_index_dict.keys()):
        words = list(train_index_dict[label]["vocab"].keys())
        length = len(words)
        for word in words:
            train_index_dict[label]["vocab"][word]['prob'] = math.log(
                (train_index_dict[label]["vocab"][word]['count'] + 1) / (length + totWordsLength))

    print(p_docs_dict)
    ratio_docs_dict = dict()

    for key, value in p_docs_dict.items():
        ratio_docs_dict[key] = math.log(value / Num_Docs)
    print(ratio_docs_dict)
    test_index_dict = dict()
    for i in range(len(test_dataset)):
        norm_text = normalize(test_dataset[i], stopwords)
        test_index_dict[i] = {"class": classes[test_targets[i]], "words": {}}
        for word in norm_text:
            if word in test_index_dict[i]["words"]:  # If this word already in dictionary then append new id to that key
                test_index_dict[i]["words"][word] += 1
            else:
                test_index_dict[i]["words"][word] = 1
    tot_len = 0
    for i in train_index_dict:
        tot_len += train_index_dict[i]["length"]
    print("tot_len:", tot_len)
    with open("train_SET.json", 'w', encoding="latin-1") as g:
        json.dump(train_index_dict, g, indent=4)
    with open("test_SET.json", 'w', encoding="latin-1") as g:
        json.dump(test_index_dict, g, indent=4)

    test_probs = dict()
    for i in test_index_dict:
        test_probs[i] = {"results": {}, "decision": {}}
        for tp in train_index_dict:
            test_probs[i]["results"][tp] = 0

    for test in test_index_dict:
        for cls in test_probs[test]["results"]:
            for word in test_index_dict[test]["words"]:
                if word in train_index_dict[cls]["vocab"]:
                    test_probs[test]["results"][cls] += (
                            test_index_dict[test]["words"][word] * train_index_dict[cls]["vocab"][word]["prob"])
                else:
                    test_probs[test]["results"][cls] += (test_index_dict[test]["words"][word] * math.log(
                        1 / (train_index_dict[cls]["length"] + len(allWordSet))))
            test_probs[test]["results"][cls] += ratio_docs_dict[cls]
    # print(test_probs)
    true_res = 0
    for doc in test_probs:
        dec = max(test_probs[doc]["results"], key=test_probs[doc]["results"].get)
        test_probs[doc]["decision"] = dec
        if dec == test_index_dict[doc]["class"]:
            true_res += 1

    pre_recall = dict()
    for cl in ratio_docs_dict:
        pre_recall[cl] = {"tp": 0, "fp": 0, "fn": 0, "tn": 0, "precision": 0, "recall": 0, "f1score": 0}

    for res in test_probs:
        # testte yalnızca ilk labelları aldığım için liste tutmuyorum tek bir string
        # var ve bu şekilde okursam ilk harfi alıyor sadece
        if test_probs[res]["decision"] == test_index_dict[res]["class"]:
            pre_recall[test_probs[res]["decision"]]["tp"] += 1
        else:
            pre_recall[test_probs[res]["decision"]]["fp"] += 1

    for res in test_index_dict:
        # testte yalnızca ilk labelları aldığım için liste tutmuyorum tek bir string
        # var ve bu şekilde okursam ilk harfi alıyor sadece
        # for cl in test_index_dict[res]["class"]:
        if test_index_dict[res]["class"] != test_probs[res]["decision"]:
            pre_recall[test_index_dict[res]["class"]]["fn"] += 1

    for cl in pre_recall:
        if (pre_recall[cl]["tp"] + pre_recall[cl]["fp"]) != 0:
            pre_recall[cl]["precision"] = pre_recall[cl]["tp"] / (pre_recall[cl]["tp"] + pre_recall[cl]["fp"])

        if (pre_recall[cl]["tp"] + pre_recall[cl]["fn"]) != 0:
            pre_recall[cl]["recall"] = pre_recall[cl]["tp"] / (pre_recall[cl]["tp"] + pre_recall[cl]["fn"])
        if pre_recall[cl]["recall"] + pre_recall[cl]["precision"] != 0:
            pre_recall[cl]["f1score"] = (2 * pre_recall[cl]["precision"] * pre_recall[cl]["recall"]) / (
                    pre_recall[cl]["recall"] + pre_recall[cl]["precision"])

    macroAVGprecision = 0
    microAVGprecision = 0
    macroAVGrecall = 0
    microAVGrecall = 0
    macroAVGf1Score = 0
    microAVGf1Score = 0

    tp_sum = 0
    fp_sum = 0
    fn_sum = 0
    for cls in pre_recall:
        tp_sum += pre_recall[cls]["tp"]
        fp_sum += pre_recall[cls]["fp"]
        fn_sum += pre_recall[cls]["fn"]
        macroAVGprecision += pre_recall[cls]["precision"]
        macroAVGrecall += pre_recall[cls]["recall"]

    microAVGprecision = tp_sum / (tp_sum + fp_sum)
    microAVGrecall = tp_sum / (tp_sum + fn_sum)
    macroAVGprecision /= 7
    macroAVGrecall /= 7
    microAVGf1Score = (2 * microAVGrecall * microAVGprecision) / (microAVGrecall + microAVGprecision)
    macroAVGf1Score = (2 * macroAVGrecall * macroAVGprecision) / (macroAVGrecall + macroAVGprecision)
    print()
    line = '{:>32}  {:>11}  {:>10}'.format("precision", "recall", "f1-score")
    print(line)
    print()
    for cls in pre_recall:
        line = '{:>20}  {:>10}  {:>11}  {:>10}'.format(cls, "    %.2f" % pre_recall[cls]["precision"],
                                                       " %.2f" % pre_recall[cls]["recall"],
                                                       "      %.2f" % pre_recall[cls]["f1score"])
        print(line)
    print()
    line = '{:>20}  {:>10}  {:>11}  {:>10}'.format("macro avg", "    %.2f" % macroAVGprecision,
                                                   " %.2f" % macroAVGrecall,
                                                   "      %.2f" % macroAVGf1Score)
    print(line)
    line = '{:>20}  {:>10}  {:>11}  {:>10}'.format("weighted avg", "    %.2f" % microAVGprecision,
                                                   " %.2f" % microAVGrecall,
                                                   "      %.2f" % microAVGf1Score)
    print(line)

    print()
    print("microAVGprecision: ", microAVGprecision)
    print("microAVGrecall: ", microAVGrecall)
    print("macroAVGprecision: ", macroAVGprecision)
    print("macroAVGrecall: ", macroAVGrecall)
    print("microAVGf1Score: ", microAVGf1Score)
    print("macroAVGf1Score: ", macroAVGf1Score)

    print("accuracy: ", true_res / len(test_index_dict))

    with open("MNB_Test_Results.json", 'w', encoding="latin-1") as g:
        json.dump(test_probs, g, indent=4)

    with open("MNB_Precision_Recall.json", 'w', encoding="latin-1") as g:
        json.dump(pre_recall, g, indent=4)


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


def self_knn_create_dict(train_dataset, train_targets, stopwords, classes):
    KNN_TRAINING_SET = dict()
    KNN_TRAINING_SET["news_id"] = {}
    for news_id in range(len(train_dataset)):
        norm_text = normalize(train_dataset[news_id], stopwords)  # Normalized text returned
        KNN_TRAINING_SET["news_id"][news_id] = {}
        KNN_TRAINING_SET["news_id"][news_id]["length"] = len(norm_text)
        KNN_TRAINING_SET["news_id"][news_id]["labels"] = train_targets[news_id]

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
    with open('KNN_TRAINING_SET.json', 'w', encoding='latin-1') as g:
        json.dump(KNN_TRAINING_SET, g)


def knnAlgorithm(train_dataset, train_targets, test_dataset, test_targets, stopwords, classes):
    text_clf = Pipeline([
        ('vect', CountVectorizer(preprocessor=my_cool_preprocessor, stop_words=stopwords)),
        ('tfidf', TfidfTransformer()),
        ('clf', KNeighborsClassifier(7)),
    ])
    text_clf.fit(train_dataset, train_targets)

    predicted = text_clf.predict(test_dataset)
    print("--------------------------- KNeighborsClassifier Implementation ---------------------------------------")
    print("predict: ", predicted[0], predicted[1], predicted[2], predicted[3], predicted[4], predicted[5], predicted[6])
    print(np.mean(predicted == test_targets))
    print(metrics.classification_report(test_targets, predicted,
                                        target_names=classes))


def knnAlgorithm_single_label(train_dataset, train_targets, test_dataset, test_targets, stopwords, classes):
    text_clf = Pipeline([
        ('vect', CountVectorizer(preprocessor=my_cool_preprocessor, stop_words=stopwords)),
        ('tfidf', TfidfTransformer()),
        ('clf', KNeighborsClassifier(7)),
    ])
    text_clf.fit(train_dataset, train_targets)
    count_vect = CountVectorizer(strip_accents=ascii, stop_words={'english'})
    X_train_counts = count_vect.fit_transform(test_dataset)
    predicted = text_clf.predict(test_dataset)
    print("predict: ", predicted[0], predicted[1], predicted[2], predicted[3], predicted[4], predicted[5], predicted[6])
    print("--------------------------- KNeighborsClassifier Implementation ---------------------------------------")
    print(np.mean(predicted == test_targets))
    print(metrics.classification_report(test_targets, predicted,
                                        target_names=classes))
    colorlist = ["purple", "darkcyan", "blue", "green", "lightgreen", "yellow", "red"]
    pca = PCA(n_components=2)
    X_pca = pca.fit_transform(X_train_counts.toarray())
    colors = [colorlist[i] for i in test_targets]
    colors = np.array(colors)
    fig, ax = plt.subplots()
    scatter = ax.scatter(X_pca.T[0], X_pca.T[1], c=colors)
    plt.show()


def main():
    stopwords_file = open("stopwords.txt", 'r')
    stopwords = stopwords_file.read().splitlines()  # I have stored stopwords in this variable
    classes = ['Treatment', 'Mechanism', 'Prevention', 'Case Report', 'Diagnosis', 'Transmission',
               'Epidemic Forecasting']
    TRAIN_FILENAME = "BC7-LitCovid-Train.csv"
    DEV_FILENAME = "BC7-LitCovid-Dev.csv"
    # train_dataset, train_targets = load_data_knn(TRAIN_FILENAME, classes, "train")
    # test_dataset, test_targets = load_data_knn(DEV_FILENAME, stopwords, "test")
    # self_knn_create_dict(train_dataset, train_targets, stopwords, classes)
    #
    # train_dataset_knn, train_targets_knn = load_data_knn_single(TRAIN_FILENAME, classes, "train")
    # test_dataset_knn, test_targets_knn = load_data_knn_single(DEV_FILENAME, classes, "test")
    # train_multi_target = MultiLabelBinarizer().fit_transform(train_targets_knn)
    # multi_target = MultiLabelBinarizer().fit_transform(test_targets_knn)
    # knnAlgorithm(train_dataset_knn, train_multi_target, test_dataset_knn, multi_target, stopwords, classes)
    # knnAlgorithm_single_label(train_dataset, train_targets, test_dataset, test_targets, stopwords, classes)

    train_dataset, train_targets = load_data(TRAIN_FILENAME, classes, "train")
    test_dataset, test_targets = load_data(DEV_FILENAME, classes, "test")
    # mnb(train_dataset, train_targets, test_dataset, test_targets, stopwords, classes)
    # svm(train_dataset, train_targets, test_dataset, test_targets, stopwords, classes)
    self_mnb(train_dataset, train_targets, test_dataset, test_targets, stopwords, classes)


if __name__ == '__main__':
    main()
