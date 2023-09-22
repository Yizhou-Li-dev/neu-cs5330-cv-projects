import sys

from torch import nn
from torch.utils.data import DataLoader
from torchvision import datasets
from torchvision.transforms import ToTensor
from model import MyNetwork
import matplotlib.pyplot as plt
import torch

device = "cuda" if torch.cuda.is_available() else "cpu"


# test the generated model with the first 10 mnist test images.
def F_test():
    model = MyNetwork()
    model.load_state_dict(torch.load('model/myNetworkModel'))
    model.eval()

    loss_fn = nn.CrossEntropyLoss()

    test_data = datasets.MNIST(
        root="data",
        train=False,
        download=True,
        transform=ToTensor(),
    )

    test_dataloader = DataLoader(test_data, batch_size=10)
    test_loss, correct = 0, 0
    with torch.no_grad():
        X, y = next(iter(test_dataloader))
        X, y = X.to(device), y.to(device)
        pred = model(X)
        test_loss += loss_fn(pred, y).item()
        correct += (pred.argmax(1) == y).type(torch.float).sum().item()
    for i in range(10):
        print(f"************************** Example {i + 1} **************************")
        print(f"output values:", "\t", "\t", " ".join(str(round(float(pred[i][j]), 2)) for j in range(10)), "\t")
        print(f"max output index:", "\t", f"{pred[i].argmax()}", "\t")
        print(f"correct label:", "\t", "\t", f"{y[i]}", "\t")

    pred_label = pred.argmax(1)
    plt.subplots_adjust(hspace=0.3)
    for i in range(9):
        ax = plt.subplot(3, 3, i + 1)
        ax.set_title(f'prediction: {pred_label[i]}')
        ax.set_xticks([])
        ax.set_yticks([])
        ax.imshow(X[i][0], cmap='gray')
    plt.savefig('images/9_digit_prediction.png')


# main function (yes, it needs a comment too)
def main(argv):
    F_test()


if __name__ == "__main__":
    main(sys.argv)
