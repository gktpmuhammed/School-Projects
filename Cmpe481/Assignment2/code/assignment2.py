from numpy import genfromtxt
import math
import matplotlib.pyplot as plt
from sklearn import tree
from sklearn.tree import DecisionTreeClassifier
import numpy as np
from sklearn.datasets import load_iris

boundaries = list()


# loading data to a numpy array
def load_data():
    my_data = genfromtxt('data.txt', delimiter=',')
    return my_data


def unique_x_y(data):
    unique_x = set()
    unique_y = set()
    for row in data:
        unique_x.add(row[0])
        unique_y.add(row[1])

    return sorted(unique_x), sorted(unique_y)


def label_count(data):
    labelCount = dict()
    for row in data:
        if row[2] in labelCount:
            labelCount[row[2]] += 1
        else:
            labelCount[row[2]] = 1
    return labelCount


def calcul_entropy(partition):
    label1 = 0
    label2 = 0
    for row in partition:
        if row[2] == 1:
            label1 += 1
        else:
            label2 += 1
    p1 = label1 / (label2 + label1)
    p2 = label2 / (label2 + label1)
    entropy = 0
    if not (p1 == 0 or p2 == 0):
        entropy = -p1 * math.log2(p1) - p2 * math.log2(p2)
    return entropy


def calculate_whole_entopy(dataset):
    labels = label_count(dataset)
    key = list(labels.keys())
    entropy_data = 0
    if len(key) < 2:
        entropy_data = 0
    else:
        p1 = labels[key[0]] / (labels[key[0]] + labels[key[1]])
        p2 = labels[key[1]] / (labels[key[0]] + labels[key[1]])
        entropy_data = -p1 * math.log2(p1) - p2 * math.log2(p2)
    return entropy_data


def calculate_ig_x(dataset, x, y):
    entropy_data = calculate_whole_entopy(dataset)
    sorted_data = dataset[dataset[:, 0].argsort()]
    ig = - 2 ** 30
    temp_ig = - 2 ** 29
    final_coord = 0
    left, right, final_left, final_right, final_left_ent, final_right_ent = list(), list(), list(), list(), 0, 0
    entropy_left, entropy_right = 0, 0
    for val in x:
        i = 0
        while i < len(sorted_data) and not (sorted_data[i][0] > val):
            i += 1
        if i < len(sorted_data):
            left = sorted_data[:i]
            right = sorted_data[i:]
            entropy_left = calcul_entropy(left)
            entropy_right = calcul_entropy(right)
            sum_entropy = (len(left) * entropy_left + len(right) * entropy_right) / len(sorted_data)
            temp_ig = entropy_data - sum_entropy

        if temp_ig > ig:
            ig = temp_ig
            final_coord = val
            final_left = left
            final_right = right
            final_left_ent = entropy_left
            final_right_ent = entropy_right
    sorted_data = dataset[dataset[:, 1].argsort()]
    ymin = sorted_data[0][1]
    ymax = sorted_data[-1][1]
    return ig, final_coord, final_right, final_left, final_left_ent, final_right_ent, ymin, ymax, (entropy_data - ig)


def calculate_ig_y(dataset, x, y):
    entropy_data = calculate_whole_entopy(dataset)
    sorted_data = dataset[dataset[:, 1].argsort()]
    ig = - 2 ** 30
    temp_ig = - 2 ** 29
    final_coord = 0
    left, right, final_left, final_right, final_left_ent, final_right_ent = list(), list(), list(), list(), 0, 0
    entropy_left, entropy_right = 0, 0
    for val in y:
        i = 0
        while i < len(sorted_data) and not (sorted_data[i][1] > val):
            i += 1
        if i < len(sorted_data):
            left = sorted_data[:i]
            right = sorted_data[i:]
            entropy_left = calcul_entropy(left)
            entropy_right = calcul_entropy(right)
            sum_entropy = (len(left) * entropy_left + len(right) * entropy_right) / len(sorted_data)
            temp_ig = entropy_data - sum_entropy

        if temp_ig > ig:
            ig = temp_ig
            final_coord = val
            final_left = left
            final_right = right
            final_left_ent = entropy_left
            final_right_ent = entropy_right
    sorted_data = dataset[dataset[:, 0].argsort()]
    xmin = sorted_data[0][0]
    xmax = sorted_data[-1][0]
    return ig, final_coord, final_right, final_left, final_left_ent, final_right_ent, xmin, xmax, (entropy_data - ig)


def find_split(dataset, x, y):
    ig_x, threshold_x, true_part_x, false_part_x, eRight, eLeft, ymin, ymax, wAvg = calculate_ig_x(dataset, x, y)
    ig_y, threshold_y, true_part_y, false_part_y, eBottom, eTop, xmin, xmax, wAvg = calculate_ig_y(dataset, x, y)
    if ig_x == 0 or ig_y == 0:
        return
    true_rows = list()
    false_rows = list()
    if ig_y > ig_x:
        true_rows = true_part_y
        false_rows = false_part_y
        boundaries.append(("y", ig_y, threshold_y, eBottom, eTop, xmin, xmax, wAvg))
    else:
        true_rows = true_part_x
        false_rows = false_part_x
        boundaries.append(("x", ig_x, threshold_x, eRight, eLeft, ymin, ymax, wAvg))

    x, y = unique_x_y(true_rows)
    true_branch = find_split(true_rows, x, y)

    x, y = unique_x_y(false_rows)
    false_branch = find_split(false_rows, x, y)

    return threshold_x


def create_tree(dataset):
    x, y = unique_x_y(dataset)
    find_split(dataset, x, y)


def prepare_data(data):
    coord = []
    label = []
    for a in data:
        coord.append([a[0], a[1]])
        label.append(a[2])
    return coord, label


def scikit_tree(dataset):
    clf = DecisionTreeClassifier(random_state=0, criterion="entropy")
    data, labels = prepare_data(dataset)
    clf.fit(data, labels)
    tree.plot_tree(clf, filled=True)
    plt.figure(3)
    plt.scatter(dataset[:, 0], dataset[:, 1], c=dataset[:, 2])
    x_min, x_max = dataset[:, 0].min() - 1, dataset[:, 0].max() + 1
    y_min, y_max = dataset[:, 1].min() - 1, dataset[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.1), np.arange(y_min, y_max, 0.1))
    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    plt.contourf(xx, yy, Z, alpha=0.4)


def own_dataset(dataset):
    # bound x>2 x<5 y>3 y<6
    for i in range(len(dataset)):
        if 5 > dataset[i][0] > 2:
            if 8 > dataset[i][1] > 5:
                dataset[i][2] = 1
            else:
                dataset[i][2] = 2
        else:
            dataset[i][2] = 2
    return dataset


def own_tree(dataset):
    boundaries.clear()
    newData = own_dataset(dataset)
    create_tree(newData)
    scikit_tree(newData)
    plt.figure(4)
    plt.scatter(newData[:, 0], newData[:, 1], c=newData[:, 2])
    newlegends = list()
    for i, tup in enumerate(boundaries):
        if tup[0] == "x":
            plt.plot([tup[2], tup[2]], [tup[5], tup[6]])
            newlegends.append(f'Boundary {i + 1} {tup[0]} = {tup[2]}')
        else:
            plt.plot([tup[5], tup[6]], [tup[2], tup[2]])
            newlegends.append(f'Boundary {i + 1} {tup[0]} = {tup[2]}')
    plt.legend(newlegends)

    plt.show()


def main():
    dataset = load_data()
    create_tree(dataset)
    scikit_tree(dataset)
    plt.figure(2)
    plt.scatter(dataset[:, 0], dataset[:, 1], c=dataset[:, 2])
    legends = list()
    for i, tup in enumerate(boundaries):
        if tup[0] == "x":
            plt.plot([tup[2], tup[2]], [tup[5], tup[6]])
            legends.append(f'Boundary {i + 1} {tup[0]} = {tup[2]}')
        else:
            plt.plot([tup[5], tup[6]], [tup[2], tup[2]])
            legends.append(f'Boundary {i + 1} {tup[0]} = {tup[2]}')
    plt.legend(legends)
    plt.show()
    own_tree(dataset)


if __name__ == '__main__':
    main()
