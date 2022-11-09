import random
import matplotlib.pyplot as plt
import numpy as np
from sklearn.cluster import KMeans
from sklearn.datasets import make_blobs
from sklearn.datasets import make_moons


def create_2d_dataset(sample_size, k, cluster_mode):
    if cluster_mode == "convex":
        X, y = make_blobs(n_samples=sample_size, centers=k, n_features=2, random_state=0)
    elif cluster_mode == "nonconvex":
        X, y = make_moons(n_samples=sample_size, random_state=0)
    return X, y


def scikit_kmeans_func(k, X, is_plot):
    if is_plot:
        fig, ax = plt.subplots(1, 1)

    km = KMeans(n_clusters=k, init='random', n_init=10, max_iter=300, tol=1e-04, random_state=0)
    y_km = km.fit_predict(X)
    allColors = ["red", "green", "blue", "yellow", "pink", "brown", "purple", "olive", "cyan", "honeydew"]
    colors = allColors[:k]
    if is_plot:
        for idd in range(len(X)):
            plt.scatter(X[idd, 0], X[idd, 1], color=colors[y_km[idd]], s=20)

        plt.scatter(km.cluster_centers_[:, 0], km.cluster_centers_[:, 1], c=colors, marker='x', s=50)

    print("Scikit learn k=", k, " centers:")
    print(km.cluster_centers_)
    if is_plot:
        plt.grid()

    return plt


def assign_centroids(center_num):
    np.random.seed(100)
    centroids = np.random.rand(center_num, 2) * 15
    return centroids


def plot_graph(dataset, centers, i, point_centers, plot_type, ax, colors):
    if plot_type == "a":
        plot_num = 2 * (i + 2) - 2
    else:
        plot_num = 2 * (i + 2) - 1
    row = plot_num // 4
    col = plot_num % 4
    for idd in range(len(dataset)):
        ax[row, col].scatter(dataset[idd, 0], dataset[idd, 1], color=colors[point_centers[idd]], s=20)

    ax[row, col].scatter(centers[:, 0], centers[:, 1], c=colors, marker='x', s=50)

    if i >= 3:
        title = "Iteration final step "
    else:
        title = "Iteration " + str(i + 1) + " step " + str(i + 1) + plot_type

    ax[row, col].set_title(title)
    ax[row, col].grid()


def calcul_centers(dataset, k, point_centers):
    new_centers = np.zeros((k, 2))
    centers = np.zeros((k, 2))
    elem_count = [0] * k
    for idx in range(len(point_centers)):
        new_centers[point_centers[idx]] = np.add(new_centers[point_centers[idx]], dataset[idx])
        elem_count[point_centers[idx]] += 1
    for ix in range(len(elem_count)):
        if elem_count[ix]:
            centers[ix] = new_centers[ix] / elem_count[ix]

    return centers


def kmeans(k, dataset, is_plot):
    if is_plot:
        fig, ax = plt.subplots(3, 4)
        ax[0, 0].scatter(dataset[:, 0], dataset[:, 1], color="black", s=20)
        ax[0, 0].set_title("Dataset and Initial Centers")
        ax[0, 0].grid()

    allColors = ["red", "green", "blue", "yellow", "pink", "brown", "purple", "olive", "cyan", "honeydew"]
    colors = allColors[:k]
    point_centers = list()
    random.seed(50)
    for r in range(len(dataset)):
        point_centers.append(random.randint(0, k - 1))
    if is_plot:
        for idd in range(len(dataset)):
            ax[0, 1].scatter(dataset[idd, 0], dataset[idd, 1], color=colors[point_centers[idd]], s=20)

        ax[0, 1].set_title("Step 1")
        ax[0, 1].grid()
    obj_fun = 2 ** 29
    i = 0
    obj_fun_list_x = list()
    obj_fun_list_y = list()

    while True:
        centers = calcul_centers(dataset, k, point_centers)
        if i < 3 and is_plot:
            plot_graph(dataset, centers, i, point_centers, "a", ax, colors)

        old_obj_fun = obj_fun
        obj_fun = 0
        for point_index in range(len(dataset)):
            a = list()
            for j in range(k):
                a.append(np.sum(np.square(dataset[point_index] - centers[j])))

            min_index = np.argmin(a)
            point_centers[point_index] = min_index
            obj_fun += a[min_index]

        obj_fun_list_x.append(i + 1)
        obj_fun_list_y.append(obj_fun)

        if i < 3 and is_plot:
            plot_graph(dataset, centers, i, point_centers, "b", ax, colors)

        i += 1

        diff = old_obj_fun - obj_fun
        if diff < 0.0001:
            break
    if is_plot:
        plot_graph(dataset, centers, 3, point_centers, "a", ax, colors)
        ax[2, 1].plot(obj_fun_list_x, obj_fun_list_y)
        ax[2, 1].plot(obj_fun_list_x, obj_fun_list_y, marker="o")
        ax[2, 1].grid()
        print("Kmeans k=", k, " centers:")
        print(centers)
    if is_plot:
        return plt
    else:
        return centers, point_centers


def find_best_k_elbow(dataset, is_plot):
    # This method was called Elbow method.
    fig, ax = plt.subplots(1, 1)
    cost = []

    for i in range(1, 11):
        centers, point_centers = kmeans(i, dataset, is_plot)
        # calculates squared error for the clustered points
        sqrd_cost = 0
        for idx in range(len(dataset)):
            sqrd_cost += np.sum(np.square(centers[point_centers[idx]] - dataset[idx]))
        cost.append(sqrd_cost)
    # plot the cost against K values
    best_k = 0
    max_ratio = 0
    for i in range(len(cost)-2):
        old_slope = cost[i] - cost[i+1]
        new_slope = cost[i+1] - cost[i+2]
        ratio = old_slope / new_slope
        if ratio > max_ratio:
            max_ratio = ratio
            best_k = i + 2
    plt.plot(range(1, 11), cost, color='g', linewidth='3')

    plt.title("Elbow Method")
    plt.grid()
    # the point of the elbow is the
    # most optimal value for choosing k
    return plt, best_k


def kmeans_con(k, dataset):
    plt_a = kmeans(k, dataset, True)
    plt_c = scikit_kmeans_func(k, dataset, True)
    plt_b, best_k = find_best_k_elbow(dataset, False)
    print("Optimal k value for this dataset is: ", best_k)
    plt_a.show()
    plt_c.show()
    plt_b.show()


def main():
    dataset, y = create_2d_dataset(200, 3, "convex")
    dataset1, y = create_2d_dataset(200, 7, "convex")
    dataset2, y = create_2d_dataset(200, 2, "nonconvex")

    kmeans_con(3, dataset)
    kmeans_con(7, dataset1)
    kmeans_con(2, dataset2)


if __name__ == '__main__':
    main()
