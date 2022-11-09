# Core
import matplotlib.image as mpimg
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.manifold import TSNE


def print_first_10_each(data, labels_loc):
    plt.figure(figsize=(20, 20))
    counter = [0] * 10
    isFinished = False
    i = 0
    while not isFinished:
        isFinished = True
        for j in range(10):
            if counter[j] != 10:
                isFinished = False
                break
        img = np.asarray(data.iloc[i, 0:].values.reshape((28, 28)) / 255)
        label = labels_loc[i]
        if counter[label] != 10:
            counter[label] += 1
            ax = plt.subplot(10, 10, label * 10 + counter[label])
            plt.axis('off')
            ax.grid(False)
            plt.imshow(img, cmap='gray_r')
            plt.xticks(color='w')
            plt.yticks(color='w')
        i += 1
    plt.subplots_adjust(left=0.1, bottom=0.1, right=0.9, top=0.9, wspace=0.4, hspace=0.4)
    plt.show()


def second_step():
    a = train_set.mean(axis=0)
    mean = train_set - a
    img = np.asarray(mean.iloc[10, 0:].values.reshape((28, 28)) / 255)
    plt.figure(figsize=(5, 5))
    ax = plt.subplot(1, 1, 1)
    ax.grid(False)
    plt.imshow(img, cmap='gray_r')
    plt.xticks(color='w')
    plt.yticks(color='w')
    plt.show()

    train_cov = mean.cov()
    eigen_values, eigen_vectors = np.linalg.eigh(train_cov)
    sorted_index = np.argsort(eigen_values)[::-1]  # descending
    sorted_eigenvalue = eigen_values[sorted_index]
    sorted_eigenvectors = eigen_vectors[:, sorted_index]
    n_components = 100
    eigenvector_subset = sorted_eigenvectors[:, 0:n_components]
    transpose_eig_vec = np.transpose(eigenvector_subset)
    plt.figure(figsize=(20, 20))
    for i in range(len(transpose_eig_vec)):
        ax = plt.subplot(10, 10, i + 1)
        ax.grid(False)
        img = np.reshape(transpose_eig_vec[i], (28, 28)) / 255
        plt.imshow(img, cmap='gray_r')
        plt.xticks(color='w')
        plt.yticks(color='w')

    plt.subplots_adjust(left=0.1, bottom=0.1, right=0.9, top=0.9, wspace=0.4, hspace=0.4)
    plt.show()
    plt.figure(figsize=(20, 20))
    plt.plot(range(1, 51), sorted_eigenvalue[:50], marker='o')
    plt.show()


def third_step():
    a = test_set.mean(axis=0)
    mean = test_set - a
    test_cov = mean.cov()
    eigen_values, eigen_vectors = np.linalg.eigh(test_cov)
    sorted_index = np.argsort(eigen_values)[::-1]  # descending
    sorted_eigenvalue = eigen_values[sorted_index]
    sorted_eigenvectors = eigen_vectors[:, sorted_index]
    n_components = 2  # you can select any number of components.
    eigenvector_subset = sorted_eigenvectors[:, 0:n_components]
    transpose_eig_vec = np.transpose(eigenvector_subset)
    dim_reduced = np.dot(transpose_eig_vec, mean.transpose()).transpose()
    fig, ax = plt.subplots(figsize=(20, 20))
    scatter = ax.scatter(dim_reduced[:, 0], dim_reduced[:, 1], c=test_labels)
    legend1 = ax.legend(*scatter.legend_elements(),
                        loc="lower right", title="Classes")
    ax.add_artist(legend1)
    plt.legend()
    plt.show()


def fourth_step():
    tsne = TSNE(n_components=2, verbose=1, perplexity=40, n_iter=300)
    tsne_results = tsne.fit_transform(test_set)
    fig, ax = plt.subplots(figsize=(20, 20))
    scatter = ax.scatter(tsne_results[:, 0], tsne_results[:, 1], c=test_labels)
    legend1 = ax.legend(*scatter.legend_elements(),
                        loc="lower right", title="Classes")
    ax.add_artist(legend1)
    plt.legend()
    plt.show()


def sixth_step(index):
    a = test_set.mean(axis=0)
    mean = test_set - a
    test_cov = mean.cov()
    eigen_values, eigen_vectors = np.linalg.eigh(test_cov)
    sorted_index = np.argsort(eigen_values)[::-1]  # descending
    sorted_eigenvalue = eigen_values[sorted_index]
    sorted_eigenvectors = eigen_vectors[:, sorted_index]
    sum_eigen_tot = np.sum(sorted_eigenvalue)
    sum_temp_eigen = 0
    eigen_ratio = list()
    reconstructions = []
    max_components = len(test_set.T)
    threshold = 0
    isset = False
    for num_component in range(2, max_components + 1, 10):
        # print(num_component)
        sum_temp_eigen = np.sum(sorted_eigenvalue[:num_component])
        ratio = sum_temp_eigen/sum_eigen_tot
        eigen_ratio.append(ratio)
        if ratio > 0.96 and not isset:
            threshold = num_component
            isset= True
        eigenvector_subset = sorted_eigenvectors[:, 0:num_component]
        transpose_eig_vec = np.transpose(eigenvector_subset)
        projection = eigenvector_subset @ transpose_eig_vec
        reconst = mean @ projection
        reconstructions.append(reconst)
    plt.figure(figsize=(20, 20))
    # print("Explained variance elbow point: ", threshold)
    for i in range(2, max_components + 1, 10):
        arr = reconstructions[i//10].iloc[6, 0:].values.reshape((28, 28)) + a.values.reshape((28, 28))
        img = np.asarray(arr / 255)
        ax = plt.subplot(8, 10, i // 10 + 1)
        plt.axis('off')
        ax.grid(False)
        plt.imshow(img, cmap='gray_r')
        plt.xticks(color='w')
        plt.yticks(color='w')
    plt.figure(figsize=(20, 20))
    for i in range(2, max_components + 1, 10):
        arr = reconstructions[i // 10].iloc[0, 0:].values.reshape((28, 28)) + a.values.reshape((28, 28))
        img = np.asarray(arr / 255)
        ax = plt.subplot(8, 10, i // 10 + 1)
        plt.axis('off')
        ax.grid(False)
        plt.imshow(img, cmap='gray_r')
        plt.xticks(color='w')
        plt.yticks(color='w')
    # print(eigen_ratio)
    plt.subplots_adjust(left=0.1, bottom=0.1, right=0.9, top=0.9, wspace=0.4, hspace=0.4)
    plt.show()
    plt.figure(figsize=(20, 20))
    plt.plot(range(2, len(eigen_ratio)*10+1,10), eigen_ratio, marker='o')
    plt.show()


if __name__ == '__main__':
    train_set = pd.read_csv('mnist_train.csv')
    labels = train_set.label
    train_set.drop('label', inplace=True, axis=1)
    test_set = pd.read_csv('mnist_test.csv')
    test_labels = test_set.label
    test_set.drop('label', inplace=True, axis=1)
    print_first_10_each(train_set, labels)
    second_step()
    third_step()
    fourth_step()
    sec_image = mpimg.imread("../misc/pokemon_28x28.png")
    R, G, B = sec_image[:, :, 0], sec_image[:, :, 1], sec_image[:, :, 2]
    imgGray = 0.2989 * R + 0.5870 * G + 0.1140 * B
    # plt.imshow(imgGray, cmap='gray')
    # plt.show()
    imgGray = imgGray * 255
    imgGray = imgGray.astype('int32')
    resized_sec_img = np.reshape(imgGray, (1, 784))
    resized_sec_img = resized_sec_img[0]
    test_set.iloc[0] = resized_sec_img
    sixth_step(0)
