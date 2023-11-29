#creat conda env

```
conda create -n fairseq2 python=3.11 -y

conda activate fairseq2
```
#install pytorch torchaudio

```
##cuda11.7, pytorch=2.0.1
conda install pytorch==2.0.1  torchaudio==2.0.2 pytorch-cuda=11.7 -c pytorch -c nvidia -c https://mirrors.bfsu.edu.cn/anaconda/cloud/pytorch/linux-64/ -y


## cuda11.8 pytorch=2.1.1
conda install pytorch=2.1.1  torchaudio==2.1.1 pytorch-cuda=11.8 -c pytorch -c nvidia -c https://mirrors.bfsu.edu.cn/anaconda/cloud/pytorch/linux-64/ -y
```
#install fairseq2
```
git clone --recurse-submodules https://github.com/shanguanma/fairseq2.git
cd fairseq2

#### install fairseq2n
## cuda11.7 version
pip install fairseq2n  --pre --upgrade --extra-index-url https://fair.pkg.atmeta.com/fairseq2/whl/nightly/pt2.0.1/cu117
## cuda11.8 version
pip install fairseq2n  --pre --upgrade --extra-index-url https://fair.pkg.atmeta.com/fairseq2/whl/nightly/pt2.1.1/cu118


#### install fairseq2
pip install -e .
#### install ibsndfile
conda install -c conda-forge libsndfile==1.0.31 
```

### check fairseq whether is work
```
view python path 
$  echo $PYTHONPATH

remove all python path
$  unset PYTHONPATH

add your fairseq2 path
$  PYTHONPATH=/path/to/fairseq2:$PYTHONPATH
e.g.PYTHONPATH=/mntnfs/lee_data1/maduo/fairseq2:$PYTHONPATH
$ echo $PYTHONPATH

$ python
Python 3.11.5 (main, Sep 11 2023, 13:54:46) [GCC 11.2.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> from fairseq2.data.typing import PathLike
>>>
```


