# The dropout rates of the Dropout layer
# The activation function for each layer
# The number of epochs of training
# The batch size while training
import time

import torch
import torch.nn as nn
from torch.utils.data import DataLoader
from torchvision import datasets
from torchvision.transforms import ToTensor

from main import device, test
from modified_model import MyNetwork
import matplotlib.pyplot as plt


# single train with specific training params
def train_with_params(batch_size=64, epoch_num=5, activate_function=nn.ReLU(), dropout_rate=0.5, all_epoch=False):
    print(
        f"start training with: batch size={batch_size}, epoch={epoch_num}, dropout rate={dropout_rate}, activate function as {activate_function}")
    start_time = time.time()

    # Download training data from open datasets.
    training_data = datasets.FashionMNIST(
        root="data",
        train=True,
        download=True,
        transform=ToTensor(),
    )

    # Download test data from open datasets.
    test_data = datasets.FashionMNIST(
        root="data",
        train=False,
        download=True,
        transform=ToTensor(),
    )

    train_dataloader = DataLoader(training_data, batch_size=batch_size)
    test_dataloader = DataLoader(test_data, batch_size=batch_size)

    model = MyNetwork(activate_function=activate_function, dropout_rate=dropout_rate).to(device)
    loss_fn = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=1e-3)

    test_accs = []
    train_accs = []

    for t in range(epoch_num):
        print(f"Epoch {t + 1}\n-------------------------------")
        fast_train(train_dataloader, model, loss_fn, optimizer)
        if all_epoch:
            _, train_acc = test(train_dataloader, model, loss_fn)
            train_accs.append(train_acc)
        _, test_acc = test(test_dataloader, model, loss_fn)
        test_accs.append(test_acc)

    end_time = time.time()
    print(f"finish training process with {end_time - start_time} s")
    if all_epoch:
        return train_accs, test_accs
    return test_accs[-1], end_time - start_time


# train without print results
def fast_train(dataloader, model, loss_fn, optimizer):
    model.train()
    for batch, (X, y) in enumerate(dataloader):
        X, y = X.to(device), y.to(device)

        # Compute prediction error
        pred = model(X)
        loss = loss_fn(pred, y)

        # Backpropagation
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()


# main part of evaluation of batch size, epoch number, activate functions, dropout rates
def eval_params():
    batch_sizes = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]

    epoch_num = 20

    activate_functions = [nn.ReLU(), nn.LeakyReLU(), nn.ReLU6(), nn.GELU(), nn.Softmax(), nn.Sigmoid(), nn.Tanh()]

    dropout_rates = [x / 100 for x in range(0, 100, 5)]

    # evaluate batch sizes
    batch_accs = []
    batch_times = []
    for batch_size in batch_sizes:
        acc, train_time = train_with_params(batch_size=batch_size)
        batch_accs.append(acc)
        batch_times.append(train_time)
    plt.xscale('log', base=2)
    fig, ax1 = plt.subplots()
    ax2 = ax1.twinx()
    ax1.plot(batch_sizes, batch_accs, label='final acc', color='green')
    ax2.plot(batch_sizes, batch_times, label='training time', color='blue')
    plt.xlabel('batch size')
    ax1.set_ylabel('accuracy', color='green')
    ax2.set_ylabel('training time/(s)', color='blue')
    plt.savefig('images/batch_size.png')
    plt.close()

    # evaluate epoch nums
    epoch_train_accs, epoch_test_accs = train_with_params(epoch_num=epoch_num, all_epoch=True)
    plt.plot(list(range(1, epoch_num + 1)), epoch_train_accs, label='train accuracy')
    plt.plot(list(range(1, epoch_num + 1)), epoch_test_accs, label='test accuracy')
    plt.legend()
    plt.xlabel('epoch number')
    plt.ylabel('train/test accuracy')
    plt.savefig('images/epoch_number.png')
    plt.close()

    # evaluate activate functions
    function_accs = []
    function_times = []
    for af in activate_functions:
        acc, train_time = train_with_params(activate_function=af)
        function_accs.append(acc)
        function_times.append(train_time)
    afs = ["ReLU", "LeakyReLU", "ReLU6", "GELU", "Softmax", "Sigmoid", "Tanh"]
    plt.bar(afs, function_accs)
    plt.xlabel('activate functions')
    plt.ylabel('test accuracy')
    plt.savefig('images/activate_function_acc.png')
    plt.close()

    plt.bar(afs, function_times)
    plt.xlabel('activate functions')
    plt.ylabel('train time')
    plt.savefig('images/activate_function_time.png')
    plt.close()

    # evaluate drop out rates
    dropout_accs = []
    dropout_times = []
    for dropout_rate in dropout_rates:
        acc, train_time = train_with_params(dropout_rate=dropout_rate)
        dropout_accs.append(acc)
        dropout_times.append(train_time)
    fig, ax1 = plt.subplots()
    ax2 = ax1.twinx()
    ax1.plot(dropout_rates, dropout_accs, label='final acc', color='green')
    ax2.plot(dropout_rates, dropout_times, label='training time', color='blue')
    plt.xlabel('dropout rate')
    ax1.set_ylabel('accuracy', color='green')
    ax2.set_ylabel('training time/(s)', color='blue')
    plt.savefig('images/dropout_rate.png')
    plt.close()


# main function
def main():
    eval_params()


if __name__ == "__main__":
    # torch.manual_seed(42)
    # torch.backends.cudnn.enabled = False
    main()
