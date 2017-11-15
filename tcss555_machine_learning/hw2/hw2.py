# course: TCSS555
# Homework 2
# date: 10/03/2017
# name: Jesse Bannon
# description: Training and testing decision trees with discrete-values attributes

import sys
import math
import re
import copy
import pandas as pd
from sklearn import tree as sktree
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

def split(examples, target, attributes, level=0, verbose=False):
    parent_entropy = entropy(examples, target)

    # If entropy is 0, mark as leaf
    if parent_entropy == 0:
        return DecisionNode(examples.iloc[0][target])
    # If there are no remaining attributes, mark as leaf
    elif len(attributes) == 0:
        counts = examples[target].value_counts()
        max_idx = np.array(counts, dtype=pd.Series).argmax()

        if verbose:
            countsArr = np.array(counts, dtype=pd.Series)
            print("LEAF: LEVEL {}, COUNT: {}, {}".format(level, countsArr[0], countsArr[1]))

        return DecisionNode(counts.index[max_idx])

    min_entropy = parent_entropy
    min_i = 0

    # Find feature with min entropy to split on
    for i in range(0, len(attributes)):
        nuniques = examples[attributes[i]].value_counts()
        nuniques_arr = np.array(nuniques, dtype=pd.Series)
        nuniques_count = np.sum(nuniques_arr)
        child_entropies = np.zeros(len(nuniques))
        
        # Take each class of feature, compute entropy of its split
        for j in range(0, len(nuniques)):
            qstr = '{}==\'{}\''.format(attributes[i], nuniques.index[j])
            child_examples = examples.query(qstr)
            child_entropies[j] = entropy(child_examples, target) * (len(child_examples) / nuniques_count)
            
        # Compare weighted sum of entropies with min entropy; update min entropy variable
        child_entropy = np.sum(child_entropies)
        if child_entropy < min_entropy:
            min_entropy = child_entropy
            min_i = i

    # If no information gain from splitting, mark it as leaf node
    if min_entropy == parent_entropy:
        counts = examples[target].value_counts()
        max_idx = np.array(counts, dtype=pd.Series).argmax()
        return DecisionNode(counts.index[max_idx])

    if verbose:
        counts = np.array(examples[target].value_counts(), dtype=pd.Series)
        print("SPLIT: {}, LEVEL {}, MIN ENTROPY: {}, COUNT: {}, {}".format(min_i, level, min_entropy, counts[0], counts[1]))

    # Recurse to build tree
    subtree_attributes = copy.deepcopy(attributes)
    subtree_feature = subtree_attributes.pop(min_i)
    subtree = DecisionNode(subtree_feature)

    nuniques = examples[subtree_feature].value_counts()
    nuniques_arr = np.array(nuniques, dtype=pd.Series)

    # Create branch for every class of feature
    for split_val in np.sort(nuniques.index):
        qstr = '{}==\'{}\''.format(subtree_feature, split_val)

        if verbose:
            print(qstr)

        child_examples = examples.query(qstr)
        subtree.children[split_val] = split(examples.query(qstr), target, subtree_attributes, level+1)

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

# Evaluating the tree on the test data
correct = 0
for i in range(0,len(test)):
    if str(tree.predicts(test.loc[i])) == str(test.loc[i,target]):
        correct += 1
print("\nThe accuracy is: ", correct/len(test))