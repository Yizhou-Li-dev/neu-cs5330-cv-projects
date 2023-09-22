import sys

import torchvision
from torch import nn
from torch.utils.data import DataLoader
from torchvision import datasets
from torchvision.transforms import ToTensor

from main import train, test
from model import MyNetwork
import matplotlib.pyplot as plt
import torch

device = "cuda" if torch.cuda.is_available() else "cpu"


# greek data set transform
class GreekTransform:
    def __init__(self):
        pass

    def __call__(self, x):
        x = torchvision.transforms.functional.rgb_to_grayscale(x)
        x = torchvision.transforms.functional.affine(x, 0, (0, 0), 36 / 128, 0)
        x = torchvision.transforms.functional.center_crop(x, (28, 28))
        return torchvision.transforms.functional.invert(x)


# greek letter train and test procedure
def greek_letter():
    model = MyNetwork()
    model.load_state_dict(torch.load('model/myNetworkModel'))

    # freezes the parameters for the whole network
    for param in model.parameters():
        param.requires_grad = False

    # print(model)
    model.layers[10] = nn.Linear(50, 3)
    print(model)

    for param in model.parameters():
        param.requires_grad = True

    # DataLoader for the Greek data set
    greek_train = torch.utils.data.DataLoader(
        torchvision.datasets.ImageFolder("data/greek_train",
                                         transform=torchvision.transforms.Compose(
                                             [torchvision.transforms.ToTensor(),
                                              GreekTransform(),
                                              torchvision.transforms.Normalize(
                                                  (0.1307,), (0.3081,))])),
        batch_size=5,
        shuffle=True)

    greek_test = torch.utils.data.DataLoader(
        torchvision.datasets.ImageFolder("data/greek_test",
                                         transform=torchvision.transforms.Compose(
                                             [torchvision.transforms.ToTensor(),
                                              GreekTransform(),
                                              torchvision.transforms.Normalize(
                                                  (0.1307,), (0.3081,))])),
        batch_size=5,
        shuffle=True)

    epochs = 10

    sample_size = len(greek_train)
    train_loss_x = list(range(epochs * sample_size))
    train_loss_y = []
    test_acc_y = []

    loss_fn = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=1e-3)

    for t in range(epochs):
        print(f"Epoch {t + 1}\n-------------------------------")
        train_losses = train(greek_train, model, loss_fn, optimizer)
        test(greek_train, model, loss_fn)
        _, test_acc = test(greek_test, model, loss_fn)

        train_loss_y.extend(train_losses)
        test_acc_y.append(test_acc)

    # loss plot
    plt.title('greek letter train loss')
    plt.plot(train_loss_x, train_loss_y, label='train loss')
    plt.xlabel('number of training example seen')
    plt.ylabel('cross entropy loss')
    plt.savefig('images/greek_letter_loss.png')

    plt.close()

    # acc plot
    plt.title('greek letter test accuracy')
    plt.plot(list(range(1, epochs + 1)), test_acc_y, label='test acc')
    plt.xlabel('number of epochs')
    plt.ylabel('accuracy scores')
    plt.savefig('images/greek_letter_acc.png')

    print("Done!")


# main function
def main(argv):
    greek_letter()


if __name__ == "__main__":
    main(sys.argv)
