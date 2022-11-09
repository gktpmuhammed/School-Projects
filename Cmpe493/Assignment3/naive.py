import json
import math

g = open("MNB_TRAINING_SET.json", 'r', encoding="latin-1")
TRAINING_SET = json.load(g)
g = open("MNB_TEST_SET.json", 'r', encoding="latin-1")
TEST_SET = json.load(g)
g = open("MNB_vocabulary.json", 'r', encoding="latin-1")
MNB_vocabulary = json.load(g)
g = open("MNB_Class_Probs.json", 'r', encoding="latin-1")
class_probs = json.load(g)

test_probs = dict()
for i in TEST_SET:
    test_probs[i] = {"results": {}, "decision": {}}
    for tp in TRAINING_SET:
        test_probs[i]["results"][tp] = 0

for test in TEST_SET:
    for cls in test_probs[test]["results"]:
        for word in TEST_SET[test]["words"]:
            if word in TRAINING_SET[cls]["vocab"]:
                test_probs[test]["results"][cls] += (TEST_SET[test]["words"][word] * TRAINING_SET[cls]["vocab"][word]["prob"])
            else:
                test_probs[test]["results"][cls] += (TEST_SET[test]["words"][word] * math.log(
                    1 / (TRAINING_SET[cls]["length"] + MNB_vocabulary["length"])))
        test_probs[test]["results"][cls] += class_probs[cls]
# print(test_probs)
true_res = 0
for doc in test_probs:
    dec = max(test_probs[doc]["results"], key=test_probs[doc]["results"].get)
    test_probs[doc]["decision"] = dec
    if dec in TEST_SET[doc]["class"]:
        true_res += (1 / len(TEST_SET[doc]["class"]))

pre_recall = dict()
for cl in class_probs:
    pre_recall[cl] = {"tp": 0, "fp": 0, "fn": 0, "tn": 0, "precision": 0, "recall": 0, "f1score": 0}

for res in test_probs:
    if test_probs[res]["decision"] in TEST_SET[res]["class"]:
        pre_recall[test_probs[res]["decision"]]["tp"] += 1
    else:
        pre_recall[test_probs[res]["decision"]]["fp"] += 1

for res in TEST_SET:
    for cl in TEST_SET[res]["class"]:
        if cl != test_probs[res]["decision"]:
            pre_recall[cl]["fn"] += 1

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

microAVGprecision = tp_sum / (tp_sum+fp_sum)
microAVGrecall = tp_sum / (tp_sum+fn_sum)
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

print("accuracy: ", true_res / len(TEST_SET))

with open("MNB_Test_Results.json", 'w', encoding="latin-1") as g:
    json.dump(test_probs, g, indent=4)

with open("MNB_Precision_Recall.json", 'w', encoding="latin-1") as g:
    json.dump(pre_recall, g, indent=4)
