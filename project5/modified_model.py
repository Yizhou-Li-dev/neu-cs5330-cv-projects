from torch import nn


# the model with same structure but need dropout rate/activate function as parameters
class MyNetwork(nn.Module):

    def __init__(self, dropout_rate, activate_function):
        super().__init__()
        self.layers = nn.Sequential(
            nn.Conv2d(1, 10, (5, 5)),  # 28*28*1 -> 24*24*10
            nn.MaxPool2d((2, 2)),
            activate_function,
            nn.Conv2d(10, 20, (5, 5)),
            nn.Dropout(dropout_rate),
            nn.MaxPool2d((2, 2)),
            activate_function,
            nn.Flatten(),
            nn.Linear(20*4*4, 50),
            activate_function,
            nn.Linear(50, 10),
            nn.LogSoftmax(dim=1)
        )

    def forward(self, x):
        res = self.layers(x)
        return res
