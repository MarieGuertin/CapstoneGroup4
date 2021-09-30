---
language: en
thumbnail:
tags:
- Spoken language understanding
- speechbrain
license: cc0-1.0
datasets:
- Timers and Such
metrics:
- Accuracy
---

<iframe src="https://ghbtns.com/github-btn.html?user=speechbrain&repo=speechbrain&type=star&count=true&size=large&v=2" frameborder="0" scrolling="0" width="170" height="30" title="GitHub"></iframe>
<br/><br/>


# End-to-end SLU model for Timers and Such

Attention-based RNN sequence-to-sequence model for [Timers and Such](https://arxiv.org/abs/2104.01604) trained on the `train-real` subset. This model checkpoint achieves 86.7% accuracy on `test-real`.

The model uses an ASR model trained on LibriSpeech ([`speechbrain/asr-crdnn-rnnlm-librispeech`](https://huggingface.co/speechbrain/asr-crdnn-rnnlm-librispeech)) to extract features from the input audio, then maps these features to an intent and slot labels using a beam search. 

The dataset has four intents: `SetTimer`, `SetAlarm`, `SimpleMath`, and `UnitConversion`. Try testing the model by saying something like "set a timer for 5 minutes" or "what's 32 degrees Celsius in Fahrenheit?"

You can try the model on the `math.wav` file included here as follows:
```python
from speechbrain.pretrained import EndToEndSLU
slu = EndToEndSLU.from_hparams("speechbrain/slu-timers-and-such-direct-librispeech-asr")
slu.decode_file("speechbrain/slu-timers-and-such-direct-librispeech-asr/math.wav")
```
### Inference on GPU
To perform inference on the GPU, add  `run_opts={"device":"cuda"}`  when calling the `from_hparams` method.

### Training
The model was trained with SpeechBrain (d254489a).
To train it from scratch follows these steps:
1. Clone SpeechBrain:
```bash
git clone https://github.com/speechbrain/speechbrain/
```
2. Install it:
```
cd speechbrain
pip install -r requirements.txt
pip install -e .
```

3. Run Training:
```
cd  recipes/timers-and-such/direct
python train.py hparams/train.yaml --data_folder=your_data_folder
```

You can find our training results (models, logs, etc) [here](https://drive.google.com/drive/folders/18c2anEv8hx-ZjmEN8AdUA8AZziYIidON?usp=sharing).

### Limitations
The SpeechBrain team does not provide any warranty on the performance achieved by this model when used on other datasets.

#### Referencing SpeechBrain

```bibtex
@misc{speechbrain,
  title={{SpeechBrain}: A General-Purpose Speech Toolkit},
  author={Mirco Ravanelli and Titouan Parcollet and Peter Plantinga and Aku Rouhe and Samuele Cornell and Loren Lugosch and Cem Subakan and Nauman Dawalatabad and Abdelwahab Heba and Jianyuan Zhong and Ju-Chieh Chou and Sung-Lin Yeh and Szu-Wei Fu and Chien-Feng Liao and Elena Rastorgueva and François Grondin and William Aris and Hwidong Na and Yan Gao and Renato De Mori and Yoshua Bengio},
  year={2021},
  eprint={2106.04624},
  archivePrefix={arXiv},
  primaryClass={eess.AS},
  note={arXiv:2106.04624}
}
```


#### Referencing Timers and Such

```
@article{lugosch2021timers,
      title={{Timers and Such: A Practical Benchmark for Spoken Language Understanding with Numbers}}, 
      author={Lugosch, Loren and Papreja, Piyush and Ravanelli, Mirco and Heba, Abdelwahab and Parcollet, Titouan},
      year={2021},
      journal={NeurIPS Datasets and Benchmarks}
}
```

#### About SpeechBrain
SpeechBrain is an open-source and all-in-one speech toolkit. It is designed to be simple, extremely flexible, and user-friendly. Competitive or state-of-the-art performance is obtained in various domains.

Website: https://speechbrain.github.io/

GitHub: https://github.com/speechbrain/speechbrain