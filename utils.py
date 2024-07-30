import torch


class AttrDict(dict):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.__dict__ = self
        for k, v in self.items():
            if isinstance(v, dict):
                self[k] = AttrDict(v)


def get_optimizer(optimizer, parameters, kwargs):
    if optimizer == "adam":
        return torch.optim.Adam(parameters, **kwargs)
    elif optimizer == "adamw":
        return torch.optim.AdamW(parameters, **kwargs)
    elif optimizer == "sgd":
        return torch.optim.SGD(parameters, **kwargs)
    else:
        raise ValueError(f"Optimizer {optimizer} not supported")


def get_criterion(criterion, kwargs):
    if criterion == "mse":
        return torch.nn.MSELoss(**kwargs)
    else:
        raise ValueError(f"Criterion {criterion} not supported")
