Stanford CS 231n Deep Learning for Computer Vision Spring 2024 Assignments
==============================

# Assignment 1

## Goals
In this assignment you will practice putting together a simple image classification pipeline based on the k-Nearest Neighbor or the SVM/Softmax classifier. The goals of this assignment are as follows:

* Understand the basic Image Classification pipeline and the data-driven approach (train/predict stages).
* Understand the train/val/test splits and the use of validation data for hyperparameter tuning.
* Develop proficiency in writing efficient vectorized code with numpy.
* Implement and apply a k-Nearest Neighbor (kNN) classifier.
* Implement and apply a Multiclass Support Vector Machine (SVM) classifier.
* Implement and apply a Softmax classifier.
* Implement and apply a Two layer neural network classifier.
* Understand the differences and tradeoffs between these classifiers.
* Get a basic understanding of performance improvements from using higher-level representations as opposed to raw pixels, e.g. color histograms, Histogram of Oriented Gradient (HOG) features, etc.

## Q1: k-Nearest Neighbor classifier
The notebook [knn.ipynb](assignment1/knn.ipynb) will walk you through implementing the kNN classifier.

## Q2: Training a Support Vector Machine
The notebook [svm.ipynb](assignment1/svm.ipynb) will walk you through implementing the SVM classifier.

## Q3: Implement a Softmax classifier
The notebook [softmax.ipynb](assignment1/softmax.ipynb) will walk you through implementing the Softmax classifier.

## Q4: Two-Layer Neural Network
The notebook [two_layer_net.ipynb](assignment1/two_layer_net.ipynb) will walk you through the implementation of a two-layer neural network classifier.

## Q5: Higher Level Representations: Image Features
The notebook [features.ipynb](assignment1/features.ipynb) will examine the improvements gained by using higher-level representations as opposed to using raw pixel values.



# Assignment 2

## Goals

In this assignment you will practice writing backpropagation code, and training Neural Networks and Convolutional Neural Networks. The goals of this assignment are as follows:

* Understand Neural Networks and how they are arranged in layered architectures.
* Understand and be able to implement (vectorized) backpropagation.
* Implement various update rules used to optimize Neural Networks.
* Implement Batch Normalization and Layer Normalization for training deep networks.
* Implement Dropout to regularize networks.
* Understand the architecture of Convolutional Neural Networks and get practice with training them.
* Gain experience with a major deep learning framework, PyTorch.

## Q1: Multi-Layer Fully Connected Neural Networks
The notebook [FullyConnectedNets.ipynb](assignment2/FullyConnectedNets.ipynb) will have you implement fully connected networks of arbitrary depth. To optimize these models you will implement several popular update rules.

## Q2: Batch Normalization
In notebook [BatchNormalization.ipynb](assignment2/BatchNormalization.ipynb) you will implement batch normalization, and use it to train deep fully connected networks.

## Q3: Dropout
The notebook [Dropout.ipynb](assignment2/Dropout.ipynb) will help you implement dropout and explore its effects on model generalization.

## Q4: Convolutional Neural Networks
In the notebook [ConvolutionalNetworks.ipynb](assignment2/ConvolutionalNetworks.ipynb) you will implement several new layers that are commonly used in convolutional networks.

## Q5: PyTorch on CIFAR-10
For this part, you will be working with PyTorch, a popular and powerful deep learning framework.

Open up [PyTorch.ipynb](assignment2/PyTorch.ipynb). There, you will learn how the framework works, culminating in training a convolutional network of your own design on CIFAR-10 to get the best performance you can.



# Assignment 3

## Goals

In this assignment, you will implement language networks and apply them to image captioning on the COCO dataset. Then you will train a Generative Adversarial Network to generate images that look like a training dataset. Finally, you will be introduced to self-supervised learning to automatically learn the visual representations of an unlabeled dataset.

The goals of this assignment are as follows:

* Understand and implement RNN and Transformer networks. Combine them with CNN networks for image captioning.
* Understand how to train and implement a Generative Adversarial Network (GAN) to produce images that resemble samples from a dataset.
* Understand how to leverage self-supervised learning techniques to help with image classification tasks.

You will use PyTorch for the majority of this homework.

## Q1: Image Captioning with Vanilla RNNs
The notebook [RNN_Captioning.ipynb](assignment3/RNN_Captioning.ipynb) will walk you through the implementation of vanilla recurrent neural networks and apply them to image captioning on COCO.

## Q2: Image Captioning with Transformers
The notebook [Transformer_Captioning.ipynb](assignment3/Transformer_Captioning.ipynb) will walk you through the implementation of a Transformer model and apply it to image captioning on COCO.

## Q3: Generative Adversarial Networks
In the notebook [Generative_Adversarial_Networks.ipynb](assignment3/Generative_Adversarial_Networks.ipynb) you will learn how to generate images that match a training dataset and use these models to improve classifier performance when training on a large amount of unlabeled data and a small amount of labeled data. When first opening the notebook, go to Runtime > Change runtime type and set Hardware accelerator to GPU.

## Q4: Self-Supervised Learning for Image Classification
In the notebook [Self_Supervised_Learning.ipynb](assignment3/Self_Supervised_Learning.ipynb), you will learn how to leverage self-supervised pretraining to obtain better performance on image classification tasks. When first opening the notebook, go to Runtime > Change runtime type and set Hardware accelerator to GPU.

## Extra Credit: Image Captioning with LSTMs
The notebook [LSTM_Captioning.ipynb](assignment3/LSTM_Captioning.ipynb) will walk you through the implementation of Long-Short Term Memory (LSTM) RNNs and apply them to image captioning on COCO.