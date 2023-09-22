import torch
import cv2

from model import MyNetwork


# test the generated model with the handwrite images.
def handwrite_test():
    model = MyNetwork()
    model.load_state_dict(torch.load('model/myNetworkModel'))
    model.eval()

    data = []

    # read our handwrite image from data/my_little_mnist and transform
    for i in range(10):
        img = cv2.imread('./data/my_little_mnist/d' + str(i) + '.png', cv2.IMREAD_UNCHANGED)
        img = cv2.resize(img, [28, 28])
        cv2.imwrite(f'./data/test/{i}_new.png', img)
        data.append(img[None, :])
    data = torch.FloatTensor(data)

    data = data.to("cpu")
    pred = model(data)
    y = torch.IntTensor(range(10))
    correct = (pred.argmax(1) == y).type(torch.float).sum().item()

    print("data label: ", y.tolist())
    print("pred result: ", pred.argmax(1).tolist())
    print(f"pred accuracy: {(100 * correct / 10):>0.1f}%")


# main function
def main():
    handwrite_test()


if __name__ == "__main__":
    main()
