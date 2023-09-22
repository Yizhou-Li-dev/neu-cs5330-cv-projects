from torchvision.io import read_image
from torchvision.models import resnet50, ResNet50_Weights
import sys
from torchvision import datasets
from torchvision.transforms import ToTensor
import matplotlib.pyplot as plt
import torch
import cv2

device = "cuda" if torch.cuda.is_available() else "cpu"

def examine():
    weights = ResNet50_Weights.DEFAULT
    model = resnet50(weights=weights)
    model.eval()
    print(model)

    with torch.no_grad():
        plt.subplots_adjust(hspace=0.3)
        shape_list = []
        weight_list = []
        for i in range(4):
            shape_list.append(model.conv1.weight[i, 0].shape)
            shape_list.append(model.conv1.weight[i, 1].shape)
            shape_list.append(model.conv1.weight[i, 2].shape)
            weight_list.append(model.conv1.weight[i, 0].numpy())
            weight_list.append(model.conv1.weight[i, 1].numpy())
            weight_list.append(model.conv1.weight[i, 2].numpy())
        for i in range(12):
            print(f"************************** Filter {i + 1} **************************")
            print(f"shape:", shape_list[i])
            print(f"weight:\n", weight_list[i])
            ax = plt.subplot(3, 4, i + 1)
            ax.set_title(f'Filter: {i + 1}')
            ax.set_xticks([])
            ax.set_yticks([])
            ax.imshow(weight_list[i])
        plt.savefig('images/test_first_layer_weight.png')

    training_data = datasets.MNIST(
        root="data",
        train=True,
        download=True,
        transform=ToTensor(),
    )

    img = training_data.data[1]
    with torch.no_grad():
        for i in range(1, 25, 2):
            kernel = weight_list[i // 2]
            ax1 = plt.subplot(6, 4, i)
            ax1.set_xticks([])
            ax1.set_yticks([])
            ax1.imshow(kernel, cmap='gray')

            ax2 = plt.subplot(6, 4, i + 1)
            ax2.set_xticks([])
            ax2.set_yticks([])
            ax2.imshow(cv2.filter2D(src=img.numpy(), ddepth=-1, kernel=kernel), cmap='gray')
            plt.savefig('images/test_filter_effect.png')

# main function (yes, it needs a comment too)
def main(argv):
    examine()

if __name__ == "__main__":
    main(sys.argv)

# img = read_image("test/assets/encode_jpeg/grace_hopper_517x606.jpg")

# Step 1: Initialize model with the best available weights
# weights = ResNet50_Weights.DEFAULT
# model = resnet50(weights=weights)
# model.eval()
# print(model)

# # Step 2: Initialize the inference transforms
# preprocess = weights.transforms()

# # Step 3: Apply inference preprocessing transforms
# batch = preprocess(img).unsqueeze(0)

# # Step 4: Use the model and print the predicted category
# prediction = model(batch).squeeze(0).softmax(0)
# class_id = prediction.argmax().item()
# score = prediction[class_id].item()
# category_name = weights.meta["categories"][class_id]
# print(f"{category_name}: {100 * score:.1f}%")