import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_blobs
from sklearn.covariance import EllipticEnvelope
from sklearn.neighbors import LocalOutlierFactor


def create_2d_dataset(n_samples):
    outliers_fraction = 0.15
    n_outliers = int(outliers_fraction * n_samples)
    n_inliers = n_samples - n_outliers
    blobs_params = dict(random_state=0, n_samples=n_inliers, n_features=2)
    blob = make_blobs(centers=[[2, 2], [-2, -2]], cluster_std=[1.5, 0.3], **blobs_params)[0]
    true_cov = np.array([[.8, .3], [.3, .4]])
    eliptic = np.random.RandomState(0).multivariate_normal(mean=[0, 0], cov=true_cov, size=500)
    return blob, eliptic


def local_outlier(data):
    fit_data = LocalOutlierFactor(n_neighbors=400, contamination=0.15).fit(data)
    predicted = fit_data.fit_predict(data)
    return predicted


def elliptic_outlier(data):
    fit_data = EllipticEnvelope(random_state=0).fit(data)
    predicted = fit_data.predict(data)
    return predicted


def algo(data):
    # xx, yy = np.meshgrid(np.linspace(-7, 7, len(data)//2), np.linspace(-7, 7, len(data)//2))
    plt.figure(figsize=(15, 15))
    plt.subplot(2, 2, 1)
    plt.scatter(data[:, 0], data[:, 1])
    plt.title("Original Dataset")
    predicted = local_outlier(data)
    plt.subplot(2, 2, 2)
    plt.title("Local Outlier Detection")
    # I added two because local outlier return 1 or -1 for prediction result
    plt.scatter(data[:, 0], data[:, 1], c=predicted + 2)
    plt.subplot(2, 2, 3)
    predicted = elliptic_outlier(data)
    plt.title("Elliptic Envelope Outlier Detection")
    plt.scatter(data[:, 0], data[:, 1], c=predicted + 2)
    # Z = EllipticEnvelope(random_state=0).predict(np.c_[xx.ravel(), yy.ravel()])
    # Z = Z.reshape(xx.shape)
    # plt.contour(xx, yy, Z, levels=[0], linewidths=2, colors="black")
    return plt


def main():
    blob, elliptic = create_2d_dataset(600)
    plt1 = algo(blob)
    plt2 = algo(elliptic)
    plt1.show()
    plt2.show()


if __name__ == '__main__':
    main()

