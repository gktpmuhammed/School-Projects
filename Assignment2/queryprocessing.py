import json
import math
import string


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


def phrase_query(new_dict, input_list):
    isInvalid = False
    result = dict()  # stores results
    if len(input_list) != 0:
        for term in input_list:
            if term not in new_dict:
                isInvalid = True
        if not isInvalid:
            first = new_dict[input_list[0]]  # first word to search and its corresponding ids
            result = first
            # I iterate over the list by taking two new words from input. A look for consecutive positions in same docs.
            for i in range(0, len(input_list) - 1):
                second = new_dict[input_list[i + 1]]  # next word
                len_res = len(result)
                len_sec = len(second)
                keys_first = list(result.keys())  # stores doc ıds of first word
                keys_second = list(second.keys())  # stores doc ıds of next word
                a, b = 0, 0
                temp = dict()
                # this loop runs until one list is finished
                while a < len_res and b < len_sec:
                    if int(keys_first[a]) == int(keys_second[b]):  # if doc ıds match then iterate over positions
                        c, d = 0, 0
                        len_res_pos = len(result[keys_first[a]])  # number of occurrence of a word in a document
                        len_sec_pos = len(second[keys_second[b]])  # number of occurrence of a word in a document
                        while c < len_res_pos and d < len_sec_pos:
                            # if two words position difference ina document is 1 then they are consecutive. Select this doc
                            diff = second[keys_second[b]][d] - result[keys_first[a]][c]
                            if diff == 1:
                                # if ids difference is one then it adds the value to result list.
                                # temp.append(keys_first[a])
                                if keys_first[a] in temp:
                                    temp[keys_first[a]].append(second[keys_second[b]][d])
                                else:
                                    temp[keys_first[a]] = [second[keys_second[b]][d]]
                                c += 1
                                d += 1
                            elif diff > 1:
                                c += 1
                            elif diff < 1:
                                d += 1
                        a += 1
                        b += 1
                    elif int(keys_first[a]) < int(keys_second[b]):
                        a += 1
                    elif int(keys_first[a]) > int(keys_second[b]):
                        b += 1
                result = temp
            result_list = list()
            for key, value in result.items():
                result_list.append(key)
            print(result_list)
        else:
            print("[]")

    else:
        print("[]")


def free_text_query(new_dict, input_list):
    result = list()
    N = 21579
    if len(input_list) != 0:
        first = list()
        if input_list[0] in new_dict:
            first = list(new_dict[input_list[0]].keys())  # first word to search and its corresponding ids
        first = [eval(x) for x in first]
        result = first
        # I iterate over the list by taking two new words from input. After making first boolean operation I am taking
        # next operand and next word and calculate this results an so on.
        for i in range(0, len(input_list) - 1):
            second = list(new_dict[input_list[i + 1]].keys())  # next word
            second = [eval(x) for x in second]
            # it will combine two list and eliminates duplicates.
            result_as_set = set(result)
            union = result_as_set.union(second)
            result = list(union)
            result.sort()  # it sorts the list in ascending order

        final_result = list()
        sqrt_q = 0
        dict_list = dict()  # this dict will store each elements occurrance in input list to calculate term frequency
        for inp in input_list:
            if inp not in dict_list:
                dict_list[inp] = 1
            else:
                dict_list[inp] += 1

        for word, count in dict_list.items():
            idf_doc = 0
            if word in new_dict:
                idf_doc = math.log10(N / len(new_dict[word]))  # idf calculation of each word
            else:
                idf_doc = 0  # if word in input does not appear on indexing dict then it will have zero weight
            tf_query = 1 + math.log10(count)  # log scaled term frequency of query terms
            tfidf_q = idf_doc * tf_query  # tfidf value for query terms
            sqrt_q += tfidf_q ** 2  # sum of the squares of each words tfidf
        for common_id in result:  # tfidf calculation for each doc
            sqrt_doc, q1_times_dn = 0, 0
            for key, value in new_dict.items():
                if str(common_id) in value:  # if a word appears in a doc then calculates it tfidf value
                    idf_doc = math.log10(N / len(new_dict[key]))
                    tf_doc = 1 + math.log10(len(value[str(common_id)]))
                    tfidf_doc = tf_doc * idf_doc
                    sqrt_doc += tfidf_doc ** 2
                    if key in dict_list:
                        q1_times_dn += tfidf_doc * (
                                math.log10(N / len(new_dict[key])) * (1 + math.log10(dict_list[key])))
            sqrt_all = math.sqrt(sqrt_doc * sqrt_q)
            if sqrt_all == 0:
                continue
            else:
                cos_sim = q1_times_dn / sqrt_all
                if cos_sim != 0:
                    final_result.append((common_id, cos_sim))

        final_result.sort(key=lambda el: el[1], reverse=True)
        print(final_result)
    else:
        print("[]")


def query_process():
    new_dict = {}  # it will store dictionary read from "myIndex.txt" file
    with open("myIndex.txt", encoding="latin-1") as h:
        new_dict = json.load(h)
    input_query = input("Please enter query:")  # takes query from user
    stopwords_file = open("stopwords.txt", 'r')
    stopwords = stopwords_file.read().splitlines()  # I have stored stopwords in this variable
    if input_query[:1] == '“' or input_query[:1] == '"':
        input_query = input_query[1:-1]
        input_list = normalize(input_query, stopwords)
        phrase_query(new_dict, input_list)
    else:
        input_list = normalize(input_query, stopwords)
        free_text_query(new_dict, input_list)


query_process()
