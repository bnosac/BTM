### CHANGES IN BTM VERSION 0.3.6

- Remove LazyData from DESCRIPTION as there is no data to be lazy about
- Add plot example in README

### CHANGES IN BTM VERSION 0.3.5

- terms.data.frame now returns the 'terms' attribute if it is part of x and it is not null, otherwise it calls the previously implemented terms.data.frame, this is done in order to circumvent the assumption that stats::terms always calls stats:::terms.default on the output of model.frame (which is used o.a in the hardhat package). While one can debate over what stats::terms should return, note that the use of terms in the BTM package is mimicked from similar functionality in the terms function from the topicmodels NLP package.

### CHANGES IN BTM VERSION 0.3.4

- Fix -Wself-assign issue on fedora-clang

### CHANGES IN BTM VERSION 0.3.3

- Make example conditionally on availability of udpipe

### CHANGES IN BTM VERSION 0.3.2

- Added terms.data.frame

### CHANGES IN BTM VERSION 0.3.1

- Allow in BTM to return as well the biterms as the vocabulary using an extra argument called detailed. For compatibility with the textplot package.
- Added in the docs examples of plotting but put these in dontrun blocks to avoid package dependencies

### CHANGES IN BTM VERSION 0.3

- Allow to provide your own set of biterms to cluster upon

### CHANGES IN BTM VERSION 0.2.1

- Add URL in DESCRIPTION file

### CHANGES IN BTM VERSION 0.2

- Allow to get all biterms from the model using terms.BTM
- Get the likelihood of a biterm alongside the model

### CHANGES IN BTM VERSION 0.1

- Initial release based on BTM commit 66cc9b475afec81f3e74bb393b874b3fe5d5a148
- Contains functions BTM, predict.BTM and terms.BTM

