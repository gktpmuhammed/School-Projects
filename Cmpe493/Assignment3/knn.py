import json
import string
import math


def idfcalculator(word):
    result = TRAINING_SET[word]
    cdf = len(result)
    return math.log10((len(TRAINING_SET["news_id"]) - 1) / cdf)


def calculate_length(deneme):
    counter = 0
    for word, values in TRAINING_SET.items():
        if deneme in values and word != "news_id":
            doctfidf = (1 + math.log10(values[deneme])) * idfcalculator(word)
            counter += doctfidf ** 2
    return math.sqrt(counter)


def lenghtcalculator(idd, query):
    tfidf = 0
    for word in set(query):
        if word in TRAINING_SET:
            if idd in TRAINING_SET[word]:
                idf = idfcalculator(word)
                doctfidf = (1 + math.log(TRAINING_SET[word][idd], 10)) * idf
                testtfidf = (1 + math.log(TRAINING_SET[word]["deneme"], 10)) * idf
                tfidf += doctfidf * testtfidf
    return tfidf


def find_best_knn(TEST_DOC):
    predicted = list()
    if TEST_DOC:
        for word in TEST_DOC:
            if word in TRAINING_SET:
                if "deneme" in TRAINING_SET[word]:
                    TRAINING_SET[word]["deneme"] += 1
                else:
                    TRAINING_SET[word]["deneme"] = 1
        uzaklik = calculate_length("deneme")
        for news_id in TRAINING_SET["news_id"]:
            lendoc = lenghtcalculator(news_id, TEST_DOC)
            cosine = lendoc / ((TRAINING_SET["news_id"][news_id]["length"]) * uzaklik)
            predicted.append((TRAINING_SET["news_id"][news_id]["labels"], cosine))
    return sorted(predicted, key=lambda x: x[1], reverse=True)


g = open("KNN_TRAINING_SET.json", 'r', encoding="latin-1")
TRAINING_SET = json.load(g)
g = open("MNB_TEST_SET.json", 'r', encoding="latin-1")
TEST_SET = json.load(g)
g = open("MNB_vocabulary.json", 'r', encoding="latin-1")
MNB_vocabulary = json.load(g)
g = open("MNB_Class_Probs.json", 'r', encoding="latin-1")
class_probs = json.load(g)

pre_recall = dict()
for cl in class_probs:
    pre_recall[cl] = {"tp": 0, "fp": 0, "fn": 0, "tn": 0, "precision": 0, "recall": 0, "f1score": 0}

for doc in TEST_SET:
    classes = TEST_SET[doc]["class"]
    k = 3
    predicted_results = find_best_knn(TEST_SET[doc]["words"])
    pred_labels = []
    sec_list = []
    for pr_res in predicted_results[:k]:
        pred_labels = pred_labels + pr_res[0]
    max_occ_label = max(set(pred_labels), key=pred_labels.count)
    for i in range(0, len(pred_labels)):
        if pred_labels[i] != max_occ_label:
            sec_list.append(pred_labels[i])
    if len(sec_list) > 0:
        second_label = max(sec_list)
        # return [label1,label2]
    result = set(pred_labels)
    for lbl in result:
        if lbl in classes:
            pre_recall[lbl]["tp"] += 1
        else:
            pre_recall[lbl]["fp"] += 1
    for lbl in classes:
        if lbl not in result:
            pre_recall[lbl]["fn"] += 1
for cl in pre_recall:
    pre_recall[cl]["precision"] = pre_recall[cl]["tp"] / (pre_recall[cl]["tp"] + pre_recall[cl]["fp"])
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
macroAVGprecision /= 10
macroAVGrecall /= 10
microAVGf1Score = (2 * microAVGrecall * microAVGprecision) / (microAVGrecall + microAVGprecision)
macroAVGf1Score = (2 * macroAVGrecall * macroAVGprecision) / (macroAVGrecall + macroAVGprecision)

print("microAVGprecision: ", microAVGprecision)
print("microAVGrecall: ", microAVGrecall)
print("macroAVGprecision: ", macroAVGprecision)
print("macroAVGrecall: ", macroAVGrecall)
print("microAVGf1Score: ", microAVGf1Score)
print("macroAVGf1Score: ", macroAVGf1Score)
