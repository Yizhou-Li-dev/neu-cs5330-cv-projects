from torch import nn


# the model with structure defined in the assignment
class MyNetwork(nn.Module):

    def __init__(self):
        super().__init__()
        self.layers = nn.Sequential(
            nn.Conv2d(1, 10, (5, 5)),  # 28*28*1 -> 24*24*10
            nn.MaxPool2d((2, 2)),
            nn.ReLU(),
            nn.Conv2d(10, 20, (5, 5)),
            nn.Dropout(),
            nn.MaxPool2d((2, 2)),
            nn.ReLU(),
            nn.Flatten(),
            nn.Linear(20*4*4, 50),
            nn.ReLU(),
            nn.Linear(50, 10),
            nn.LogSoftmax(dim=1)
        )

    def forward(self, x):
        res = self.layers(x)
        return res
