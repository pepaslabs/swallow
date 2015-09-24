# swallow
Buffer all of stdin until EOF, then dump to stdout.

## Compilation

```
gcc swallow.c
```

## Installation

```
mkdir -p ~/opt/swallow-0.1/bin
mv a.out ~/opt/swallow-0.1/bin/swallow
splice ~/opt/swallow-0.1 ~/local
```

(see https://github.com/pepaslabs/splice)

## Usage:

Insert swallow into any existing pipeline as needed.

```
echo foo | swallow | gzip > /dev/null
```
