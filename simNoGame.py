import hydra
from omegaconf import DictConfig

from env import MouseEnv
from model import DQN


# Initialize squares
@hydra.main(
    version_base=None,
    config_path="config",
    config_name="config",
)
def train(config: DictConfig):
    # config = OmegaConf.to_container(config, resolve=True)
    # config = AttrDict(config)
    env = MouseEnv(**config.env)
    model = DQN(env=env, **config.model)
    model.train()
