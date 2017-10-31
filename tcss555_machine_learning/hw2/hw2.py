# course: TCSS555
# Homework 2
# date: 10/03/2017
# name: Martine De Cock
# description: Training and testing decision trees with discrete-values attributes

import sys
import math
import re
from id3 import Id3Estimator
import pandas as pd
import numpy as np

class DecisionNode:

    # A DecisionNode contains an attribute and a dictionary of children. 
    # The attribute is either the attribute being split on, or the predicted label if the node has no children.
    def __init__(self, attribute):
        self.attribute = attribute
        self.children = {}
        self.entropy = 1

    # Visualizes the tree
    def display(self, level = 0):
        if self.children == {}: # reached leaf level
            print(": ", self.attribute, end="")
        else:
            for value in self.children.keys():
                prefix = "\n" + " " * level * 4
                print(prefix, self.attribute, "=", value, end="")
                self.children[value].display(level + 1)
     
    # Predicts the target label for instance x
    def predicts(self, x):
        if self.children == {}: # reached leaf level
            return self.attribute
        value = x[self.attribute]
        subtree = self.children[value]
        return subtree.predicts(x)

def entropy(examples, target):
    counts = np.array(examples[target].value_counts(), dtype=pd.Series)
    total_counts = np.sum(counts)

    entropy = 0
    for i in range(0, len(counts)):
        frac = counts[i] / total_counts
        entropy += (frac * np.log2(frac))
    return entropy * -1.0

def split(examples_, target, attributes_, level=0):
    examples = examples_.copy()
    attributes = attributes_.copy()
    parent_entropy = entropy(examples, target)

    if parent_entropy == 0:
        return DecisionNode(examples.iloc[0][target])
    elif len(attributes) == 0:
        counts = examples[target].value_counts()
        max_idx = np.array(counts, dtype=pd.Series).argmax()
        return DecisionNode(counts.index[max_idx])

    max_ig = 0
    max_i = 0

    for i in range(0, len(attributes)):
        nuniques = examples[attributes[i]].value_counts()
        nuniques_arr = np.array(nuniques, dtype=pd.Series)
        nuniques_count = np.sum(nuniques_arr)
        child_entropies = np.zeros(len(nuniques))
        
        for j in range(0, len(nuniques)):
            qstr = '{}==\'{}\''.format(attributes[i], nuniques.index[j])
            child_examples = examples.query(qstr)
            child_entropies[j] = entropy(child_examples, target) * (len(child_examples) / nuniques_count)
        
        ig = parent_entropy - np.sum(child_entropies)
        if ig > max_ig:
            max_ig = ig
            max_i = i

    if max_ig == 0:
        counts = examples[target].value_counts()
        max_idx = np.array(counts, dtype=pd.Series).argmax()
        return DecisionNode(counts.index[max_idx])

    # Recurse to build tree
    subtree_feature = attributes[max_i]
    attributes.remove(subtree_feature)

    subtree = DecisionNode(subtree_feature)

    nuniques = examples[subtree_feature].value_counts()
    nuniques_arr = np.array(nuniques, dtype=pd.Series)
    for j in range(0, len(nuniques)):
        qstr = '{}==\'{}\''.format(subtree_feature, nuniques.index[j])
        child_examples = examples.query(qstr).drop(subtree_feature, 1)
        subtree.children[nuniques.index[j]] = split(child_examples, target, attributes, level+1)

    return subtree


# Illustration of functionality of DecisionNode class
def funTree():
    myLeftTree = DecisionNode('humidity')
    myLeftTree.children['normal'] = DecisionNode('no')
    myLeftTree.children['high'] = DecisionNode('yes')
    myTree = DecisionNode('wind')
    myTree.children['weak'] = myLeftTree
    myTree.children['strong'] = DecisionNode('no')
    return myTree



####################   MAIN PROGRAM ######################

# Reading input data
train = pd.read_csv(sys.argv[1])
test = pd.read_csv(sys.argv[2])
target = sys.argv[3]
attributes = train.columns.tolist()

# Keep only alphanumeric characters in col names for querying
attributes = [re.sub(r'\W+', '', x) for x in attributes]

train.columns = attributes
test.columns = attributes
attributes.remove(target)

tree = split(train, target, attributes)
tree.display()

# clf = Id3Estimator()
# clf.fit(train.drop(target, 1).as_matrix(), train[target].as_matrix())
# out = clf.predict(test.drop(target, 1).as_matrix())

# Evaluating the tree on the test data
correct = 0
for i in range(0,len(test)):
    # if out[i] == test.loc[i,target]:
    if str(tree.predicts(test.loc[i])) == str(test.loc[i,target]):
        correct += 1
print("\nThe accuracy is: ", correct/len(test))