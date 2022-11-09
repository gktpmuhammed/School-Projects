Hi, in this project we are assigned to implement naive bayes and knn algorithms.
First we need to make preprocessing and splitting data as training and test set.
I used BeautifulSoup library so yuo need to install it to your working environment.
1) "pip install bs4" will install this library.
2) "python createDict.py" will run indexing module and create several files to prepare necessary probabilites and values for knn and naive bayes.
3) "python naive.py" Then you need to run this command for naive bayes algorithm to run. Since I am storing all necessary values in preprocessing this code runs in 1-2 second.
4) "python knn.py" Then you need to run this command for knn algorithm  to run. although I stored many values on files to ease the code it finishes in 5 minutes.
Results will be displayed in console for both. In naive bayes results also printed on a document named MNB_Precision_Recall.json.