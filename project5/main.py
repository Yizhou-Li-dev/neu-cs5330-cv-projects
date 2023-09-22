# Tianyu Yang, Yizhou Li
# CV project 5

# import statements
import sys

import torch
from torch import nn
from torch.utils.data import DataLoader
from torchvision import datasets
from torchvision.transforms import ToTensor
from model import MyNetwork
import matplotlib.pyplot as plt

device = "cuda" if torch.cuda.is_available() else "cpu"


# main train and test procedure
def train_network(batch_size=64):
    # Download training data from open datasets.
    training_data = datasets.MNIST(
        root="data",
        train=True,
        download=True,
        transform=ToTensor(),
    )

    # Download test data from open datasets.
    test_data = datasets.MNIST(
        root="data",
        train=False,
        download=True,
        transform=ToTensor(),
    )

    train_dataloader = DataLoader(training_data, batch_size=batch_size)
    test_dataloader = DataLoader(test_data, batch_size=batch_size)

    model = MyNetwork().to(device)
    loss_fn = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=1e-3)

    epochs = 5

    # Define the x/y axis for the plot
    sample_size = len(train_dataloader)
    train_loss_x = list(range(epochs * sample_size))
    test_loss_x = list(range(sample_size, epochs * sample_size + 1, sample_size))
    train_loss_y = []
    test_loss_y = []

    epoch_x = list(range(1, epochs + 1))
    train_accs = []
    test_accs = []

    for t in range(epochs):
        print(f"Epoch {t + 1}\n-------------------------------")
        train_losses = train(train_dataloader, model, loss_fn, optimizer)
        _, train_acc = test(train_dataloader, model, loss_fn)
        test_loss, test_acc = test(test_dataloader, model, loss_fn)

        # save the loss and acc data
        train_loss_y.extend(train_losses)
        test_loss_y.append(test_loss)

        train_accs.append(train_acc)
        test_accs.append(test_acc)

    # plot the loss and acc
    # fig, axes = plt.subplots(1, 2)

    # loss plot
    plt.title('train/test loss')
    plt.scatter(test_loss_x, test_loss_y, label='test loss', c='red', zorder=2)
    plt.plot(train_loss_x[::10], train_loss_y[::10], label='train loss', zorder=1)
    plt.legend()
    plt.xlabel('number of training example seen')
    plt.ylabel('cross entropy loss')
    plt.savefig('images/loss.png')

    plt.close()

    # acc plot
    plt.title('train/test accuracy')
    plt.plot(epoch_x, train_accs, label='train acc')
    plt.plot(epoch_x, test_accs, label='test acc')
    plt.legend()
    plt.xlabel('number of epochs')
    plt.ylabel('accuracy scores')
    plt.savefig('images/acc.png')

    torch.save(model.state_dict(), './model/myNetworkModel')
    print("Done!")


# train function with loss printed every 100 batches
def train(dataloader, model, loss_fn, optimizer):
    size = len(dataloader.dataset)
    model.train()
    losses = []
    for batch, (X, y) in enumerate(dataloader):
        X, y = X.to(device), y.to(device)

        # Compute prediction error
        pred = model(X)
        loss = loss_fn(pred, y)
        losses.append(loss.item())

        # Backpropagation
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        if batch % 100 == 0:
            loss, current = loss.item(), (batch + 1) * len(X)
            print(f"loss: {loss:>7f}  [{current:>5d}/{size:>5d}]")
    return losses


# test function with test accuracy and loss printed
def test(dataloader, model, loss_fn):
    size = len(dataloader.dataset)
    num_batches = len(dataloader)
    model.eval()
    test_loss, correct = 0, 0
    with torch.no_grad():
        for X, y in dataloader:
            X, y = X.to(device), y.to(device)
            pred = model(X)
            test_loss += loss_fn(pred, y).item()
            correct += (pred.argmax(1) == y).type(torch.float).sum().item()
    test_loss /= num_batches
    correct /= size
    print(f"Test Error: \n Accuracy: {(100 * correct):>0.1f}%, Avg loss: {test_loss:>8f} \n")
    return test_loss, correct


# main function (yes, it needs a comment too)
def main(argv):
    # handle any command line arguments in argv
    if len(argv) == 2 and argv[1].isdigit():
        train_network(int(argv[1]))
    else:
        train_network()
    # main function code
    return


if __name__ == "__main__":
    # torch.manual_seed(42)
    # torch.backends.cudnn.enabled = False
    main(sys.argv)
