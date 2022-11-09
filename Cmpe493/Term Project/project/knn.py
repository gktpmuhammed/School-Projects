import json
import math
import string
import sys
classnames=['Treatment', 'Mechanism', 'Prevention', 'Case Report', 'Diagnosis', 'Transmission',
               'Epidemic Forecasting']
classcounts=[ 8717,  4438, 11102, 2063,  6193,  1088,  645]
threshold = 5/11103
def lenghtcalculator2(id, dictionary):
    counter=0
    dist=0
    for word, values in dictionary.items():
      if id in values and word != "news_id":
            doctfidf = (1+math.log(values[id],10))*idfcalculator(word,dictionary)
            counter +=doctfidf**2
    return math.sqrt(counter)
def lenghtcalculator(id, dictionary, query,newdict):
    tfidf=0
    for word in set(query):
      if word in dictionary:
        if id in dictionary[word]:
            idf=idfcalculator(word,dictionary)
            doctfidf = (1+math.log(dictionary[word][id],10))*idf
            testtfidf = (1+math.log(newdict[word],10))*idf
            tfidf +=doctfidf*testtfidf
    return tfidf
def idfcalculator(word,dictionary):
    result = dictionary[word]
    counterdf=len(result)
    return math.log((len(dictionary["news_id"])-1) / counterdf, 10)
#same tokenizer in create.py. It tokenize query input
def tokenize(raw_text, stopwords):
    punc_text = raw_text.translate(raw_text.maketrans("", "", string.punctuation))
    casefold_text = punc_text.casefold()
    norm_text = list()
    for word in casefold_text.split():
        if word not in stopwords:
            norm_text.append(word)
    return norm_text
#query processor
def knn(query,dictionary):
    result = []
    newdict={}
    if query:
        for a in query:
            if a in newdict:
                newdict[a]+=1
            else:
                newdict[a]=1
        counter=0
        for i in newdict:
            if i in dictionary:
                doctfidf = (1 + math.log(newdict[i], 10)) * idfcalculator(i, dictionary)
                counter += doctfidf ** 2
        len=math.sqrt(counter)
        for b in dictionary["news_id"]:
            lendoc=lenghtcalculator(b,dictionary,query,newdict)
            cosine=lendoc/((dictionary["news_id"][b]["length"])*(len))
            result.append((dictionary["news_id"][b]["labels"],cosine,b))
    return sorted(result,key=lambda x:x[1],reverse=True)

def processor(query, k, dictionary):
    predict=knn(query,dictionary)
    #print(predict[0])
    liste=[]
    liste2=[]
    for i in predict[:k]:

        liste=liste+i[0]
    for i in set(liste):
        if liste.count(i)/len(liste)>=classcounts[classnames.index(i)]/34246:
            liste2.append(i)
    return liste2



if __name__ == '__main__':
    with open('KNN_TEST_SET.json') as f:
        data = f.read()
    testdict=json.loads(data)
    liste = []
    counter=0
    count=0
    tp = [0, 0, 0, 0, 0, 0, 0]
    fp = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    tn = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    fn = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    precision1 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    recall1 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    f1=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

    with open('KNN_TRAINING_SET.json') as f:
        data = f.read()

    dictionary = json.loads(data)   #create dictionary
    for i in testdict:
        dcclassnames=testdict[i]["class"]

        prediction=processor(testdict[i]["words"], 34 , dictionary)     #this is the calling of processor. First parameter is document, second one is k and last one is dictionary

        for lab in prediction:
            if lab in dcclassnames:
                tp[classnames.index(lab)]=tp[classnames.index(lab)]+1
            else:
                fp[classnames.index(lab)]=fp[classnames.index(lab)]+1
        for lab in dcclassnames:
            if lab not in prediction:
                fn[classnames.index(lab)] = fn[classnames.index(lab)] +1
    print("                            precision    recall      f-1 score ")
    for kk in range (0,7):
        recall=(tp[kk]) / (tp[kk] + fn[kk])
        recall1[kk]=recall
        precision=(tp[kk]) / (tp[kk] + fp[kk])
        precision1[kk]=precision
        f1[kk]=((2*recall*precision)/(recall+precision))
        if (tp[kk] + fn[kk])>0:
            line='{:>20}  {:>10}  {:>11}  {:>10}'.format(classnames[kk],"    %.2f" % precision," %.2f" % recall,"      %.2f" % ((2*recall*precision)/(recall+precision)))
            print(line)
    microrecall=(sum(tp))/(sum(tp)+sum(fp))
    macrorecall=sum(recall1)/7
    micropre=(sum(tp))/(sum(tp)+sum(fp))
    macropre=sum(precision1)/7
    microf1=2*micropre*microrecall/(microrecall+micropre)
    macrof1=2*macropre*macrorecall/(macrorecall+micropre)
    print("            microavg \t    %.2f" % micropre,"    \t %.2f"% microrecall,"   \t %.2f" % microf1)
    print("            macroavg \t    %.2f" % macropre,"    \t %.2f"% macrorecall,"   \t %.2f"% macrof1)
