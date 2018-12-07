# BTM - Biterm Topic Modelling for Short Text with R

This is an R package wrapping the C++ code available at https://github.com/xiaohuiyan/BTM for constructing a **Biterm Topic Model (BTM)**. This model models word-word co-occurrences patterns (e.g., biterms). 

> Topic modelling using biterms is particularly good for finding topics in short texts (as occurs in short survey answers or twitter data).

### Installation

This R package is not on CRAN, you can only install it as follows: `devtools::install_github("jwijffels/BTM")`

### What

The Biterm Topic Model (BTM) is a word co-occurrence based topic model that learns topics by modeling word-word co-occurrences patterns (e.g., biterms)

- A biterm consists of two words co-occurring in the same context, for example, in the same short text window. 
- BTM models the biterm occurrences in a corpus (unlike LDA models which model the word occurrences in a document). 
- It's a generative model. In the generation procedure, a biterm is generated by drawing two words independently from a same topic `z`. In other words, the distribution of a biterm `b=(wi,wj)` is defined as: `P(b) = sum_k{P(wi|z)*P(wj|z)*P(z)}` where k is the number of topics you want to extract.
- Estimation of the topic model is done with the Gibbs sampling algorithm. Where estimates are provided for `P(w|k)=phi` and `P(z)=theta`.

More detail can be referred to the following paper:

> Xiaohui Yan, Jiafeng Guo, Yanyan Lan, Xueqi Cheng. A Biterm Topic Model For Short Text. WWW2013.
> https://github.com/xiaohuiyan/xiaohuiyan.github.io/blob/master/paper/BTM-WWW13.pdf


### Example

```
library(udpipe)
library(BTM)
data("brussels_reviews_anno", package = "udpipe")

## Taking only nouns of Dutch data
x <- subset(brussels_reviews_anno, language == "nl")
x <- subset(x, xpos %in% c("NN", "NNP", "NNS"))
x <- x[, c("doc_id", "token")]

## Building the model
set.seed(321)
model  <- BTM(x, k = 4, beta = 0.01, iter = 1000, trace = 100)

## Inspect the model - topic frequency + conditional term probabilities
model$theta
[1] 0.3749164 0.2176499 0.1983099 0.2091239

topicterms <- terms(model, top_n = 10)
topicterms
[[1]]
         token probability
1  appartement 0.081536297
2      Brussel 0.045403638
3      centrum 0.025393918
4      locatie 0.016958166
5        buurt 0.014770052
6     aanrader 0.013589623
7     verblijf 0.011718210
8  loopafstand 0.011545464
9         stad 0.010451408
10       kamer 0.008867905

[[2]]
         token probability
1  appartement 0.063439432
2      Brussel 0.027032724
3        buurt 0.019989465
4     verblijf 0.016666237
5      centrum 0.013491810
6        super 0.012847005
7        kamer 0.011259791
8     centraal 0.008730170
9     badkamer 0.008184565
10        stad 0.008134965

[[3]]
         token probability
1         huis  0.03032317
2  appartement  0.02433486
3        buurt  0.01921758
4     badkamer  0.01693113
5        kamer  0.01557015
6      Brussel  0.01546127
7          bed  0.01290263
8     verblijf  0.01241268
9      centrum  0.01159609
10  slaapkamer  0.01143277

[[4]]
         token probability
1  appartement 0.042021835
2      Brussel 0.031852056
3        kamer 0.019101115
4         huis 0.017500795
5        buurt 0.014558270
6     verblijf 0.009963802
7      centrum 0.009705686
8          dag 0.009602439
9     aanrader 0.009550816
10        stad 0.009292700

scores <- predict(model, newdata = x)
```

Make a specific topic called the background

```
# If you set background to TRUE
# The first topic is set to a background topic that equals to the empirical word dsitribution. 
# This can be used to filter out common words. Defaults to FALSE.
set.seed(321)
model  <- BTM(x, k = 5, beta = 0.01, background = TRUE, iter = 1000, trace = 100)
topicterms <- terms(model, top_n = 5)
topicterms
```