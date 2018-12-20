# rmdopfq

[![travis-ci](https://travis-ci.org/djhshih/rmdopfq.svg?branch=master)](https://travis-ci.org/djhshih/rmdopfq)
[![codecov](https://codecov.io/gh/djhshih/rmdopfq/branch/master/graph/badge.svg)](https://codecov.io/gh/djhshih/rmdopfq)

Remove doppelganger reads (those with the same name) from a fastq file.

Tools such as Picard and GATK forbid reads with the same name, because they
cause a hash collision. Such reads do not normally occur during preprocessing
and usually occur as a result of pipeline bugs.
`rmdopfq` is a band-aid fix for improperly merged fastq files that contain
doppelganger reads.

```{bash}
usage: rmdopfq <in.fq> <out.fq>
```

Assume that each sequence and quality score entry `r1.fq` and `r2.fq` is single-line.

The output file `out.fq` will contain only reads with unique names.
Duplicate read names are emitted to `stderr`.

## Example workflow

We can read from a gziped fastq file and output to a gziped fastq file by:

```{bash}
rmdopfq <(gunzip -c r1.fq.gz) /dev/stdout | gzip > r1rd.fq.gz
```

If the reads are paired, we can do the same with the other read.

```{bash}
rmdopfq <(gunzip -c r2.fq.gz) /dev/stdout | gzip > r2rd.fq.gz
```

Now, we can align the pair-end reads by

```{bash}
bwa mem hg38.fa r1rd.fq.gz r2rd.fq.gz | samtools view -b - > sample_hg38.bam
```

