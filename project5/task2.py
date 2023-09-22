import sys

from torchsummary import summary
from torchvision import datasets
from torchvision.transforms import ToTensor

from model import MyNetwork
import matplotlib.pyplot as plt
import torch
import cv2

device = "cuda" if torch.cuda.is_available() else "cpu"


# examine the model first layer and print out result, save the images.
def examine():
    model = MyNetwork()
    model.load_state_dict(torch.load('model/myNetworkModel'))
    model.eval()
    # summary(model, (1, 28, 28))
    # print(model)
    with torch.no_grad():
        plt.subplots_adjust(hspace=0.3)
        for i in range(10):
            print(f"************************** Filter {i + 1} **************************")
            print(f"shape:", list(model.layers[0].weight[i, 0].shape))
            print(f"weight:\n", model.layers[0].weight[i, 0].numpy())
            ax = plt.subplot(3, 4, i + 1)
            ax.set_title(f'Filter: {i + 1}')
            ax.set_xticks([])
            ax.set_yticks([])
            ax.imshow(model.layers[0].weight[i, 0])
        plt.savefig('images/first_layer_weight.png')

    training_data = datasets.MNIST(
        root="data",
        train=True,
        download=True,
        transform=ToTensor(),
    )

    img = training_data.data[1]
    with torch.no_grad():
        for i in range(1, 21, 2):
            kernel = model.layers[0].weight[i // 2, 0]
            ax1 = plt.subplot(5, 4, i)
            ax1.set_xticks([])
            ax1.set_yticks([])
            ax1.imshow(kernel, cmap='gray')

            ax2 = plt.subplot(5, 4, i + 1)
            ax2.set_xticks([])
            ax2.set_yticks([])
            ax2.imshow(cv2.filter2D(src=img.numpy(), ddepth=-1, kernel=kernel.numpy()), cmap='gray')
            plt.savefig('images/filter_effect.png')


# main function
def main():
    examine()


if __name__ == "__main__":
    main()
