#Implementation of Tensor Softmax Function

import torch
import numpy as np


#Implementation of Tensor Softmax function without using inbuilt function
def softmax_2d_custom(x):
    # Subtract the maximum value along each row for numerical stability(to avoid large exponentials that might lead to overflow).
    x_max = np.max(x, axis=0, keepdims=True)

    e_x = np.exp(x - x_max)

    sum_e_x = np.sum(e_x, axis=0, keepdims=True)

    softmax_values = e_x / sum_e_x
    softmax_values = np.round(softmax_values,decimals=4)

    return softmax_values

x = torch.tensor([[0.0,1.0,2.0,3.0],
                  [4.0,5.0,6.0,7.0]
                 ])
y = torch.softmax(x,dim=0)
print("With built in function softmax\n")
print(y)

x = np.array([[0.0,1.0,2.0,3.0],
              [4.0,5.0,6.0,7.0]
              ])
y = softmax_2d_custom(x)
print("\nWithout built in function\n")
print(y)