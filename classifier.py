from sklearn import svm
import numpy as np
import matplotlib.pyplot as plt

class eye_classifier:
    cls = None
    samples = []
    labels = []

    def __init__(self):
        pass

    def train(self, samples, labels):
        self.samples = samples
        self.labels = labels
        self.cls = svm.SVC()
        self.cls.fit(self.samples, self.labels)
    
    def classify(self, sample):
        return self.cls.predict([sample])

    def plot(self):
        fig, sub = plt.subplots()

        X0, X1 = [it[0] for it in samples], [it[1] for it in samples]
        xx, yy = self.make_meshgrid(X0, X1)

        self.plot_contours(sub, self.cls, xx, yy, cmap=plt.cm.coolwarm, alpha=0.8)
        sub.scatter(X0, X1, c=self.labels, cmap=plt.cm.coolwarm, s=20, edgecolors='k')
        sub.set_xlim(xx.min(), xx.max())
        sub.set_ylim(yy.min(), yy.max())
        sub.set_xlabel('x')
        sub.set_ylabel('y')
        sub.set_xticks(())
        sub.set_yticks(())
        
        plt.show()

    def make_meshgrid(self, x, y, h=.02):
        x_min, x_max = min(x) - 1, max(x) + 1
        y_min, y_max = min(y) - 1, max(y) + 1
        xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                             np.arange(y_min, y_max, h))
        return xx, yy

    def plot_contours(self, ax, clf, xx, yy, **params):
        Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
        Z = Z.reshape(xx.shape)
        out = ax.contourf(xx, yy, Z, **params)
        return out

