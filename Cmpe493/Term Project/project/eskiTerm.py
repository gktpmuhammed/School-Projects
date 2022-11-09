import numpy as np
import pandas as pd

FILENAME = R"C:\Users\muhammed\Desktop\Ders\7.Dönem\Cmpe493\Term Project\BC7-LitCovid-Train.csv"
dataset = pd.read_csv(FILENAME)
classes = ['Treatment', 'Mechanism', 'Prevention', 'Case Report', 'Diagnosis', 'Transmission', 'Epidemic Forecasting']
Num_Docs = dataset.shape[0]
p_docs_dict = dict()
index_dict = dict({'Treatment': {}, 'Mechanism': {}, 'Prevention': {}, 'Case Report': {}, 'Diagnosis': {},
                   'Transmission': {}, 'Epidemic Forecasting': {}})
for i in range(Num_Docs):
    title = dataset.loc[i, 'title']
    abstract = dataset.loc[i, 'abstract']
    keywords = dataset.loc[i, 'keywords']
    if keywords != keywords:  # check if keywords nan
        keywords = ''
    review = title + ' ' + abstract + ' ' + keywords
    review = review.lower()
    norm_text = review.split()
    labels = dataset.loc[i, 'label'].split(';')
    for label in labels:
        if label in p_docs_dict:
            p_docs_dict[label] += 1
        else:
            p_docs_dict[label] = 1
        for word in norm_text:
            if word in index_dict[label]:  # If this word already in dictionary then append new id to that key
                index_dict[label][word]['count'] += 1
            else:
                index_dict[label][word] = {'count': 1, 'prob': 0}

allWordSet = set()
for label in list(index_dict.keys()):
    allWordSet = allWordSet.union(list(index_dict[label].keys()))

totWordsLength = len(allWordSet)
for label in list(index_dict.keys()):
    words = list(index_dict[label].keys())
    length = len(words)
    for word in words:
        index_dict[label][word]['prob'] = (index_dict[label][word]['count'] + 1) / (length + totWordsLength)

print(p_docs_dict)
ratio_docs_dict = dict()
for key, value in p_docs_dict.items():
    ratio_docs_dict[key] = value / Num_Docs
print(ratio_docs_dict)

TEST_FILENAME = R"C:\Users\muhammed\Desktop\Ders\7.Dönem\Cmpe493\Term Project\BC7-LitCovid-Dev.csv"

dataset_dev = pd.read_csv(TEST_FILENAME)

prob_s_with_ls = []
title = dataset_dev.loc[0, 'title']
abstract = dataset_dev.loc[0, 'abstract']
keywords = dataset_dev.loc[0, 'keywords']
if keywords != keywords:  # check if keywords nan
    keywords = ''
review = title + ' ' + abstract + ' ' + keywords
review = review.lower()
norm_text = review.split()

probs = dict({'Treatment': 1, 'Mechanism': 1, 'Prevention': 1, 'Case Report': 1, 'Diagnosis': 1,
              'Transmission': 1, 'Epidemic Forecasting': 1})
for label in classes:
    words = list(index_dict[label].keys())
    length = len(words)
    for word in norm_text:
        if word in list(index_dict[label].keys()):
            count = index_dict[label][word]['count']
        else:
            count = 0
        probs[label] *= (count + 1) / (length + totWordsLength)

print(probs)
