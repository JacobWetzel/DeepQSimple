import transformers
import torch

model_id = "C:/Users/Jacob Wetzel/Meta-Llama-3.1-8B-Instruct"

pipeline = transformers.pipeline(
  "text-generation",
  model=model_id,
  tokenizer=model_id,
  model_kwargs={"torch_dtype": torch.bfloat16},
  device=0,  # 0 for the first CUDA device
)

# Test the pipeline
prompt = "Hello, Llama 3!"
result = pipeline(prompt, max_length=50)
print(result)
