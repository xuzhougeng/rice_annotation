# rice_annotaiton

基于RAP-DB网站的已有注释信息，进行整理，便于自己后续分析使用

GFF转成GTF(GFF_to_cellranger_gtf)是我用来将GFF文件转成符合CellRanger输入要求的GTF写的程序

```
gff3_to_cellranger_gtf ./IRGSP_v1.gff IRGSP_v1.gtf
```


## 建立索引


## R语言中使用

基于自己生成fasta和gff文件，我生成了BSgenome安装包和TxDb安装

BSgenome.Osativa.MSU.xzg_1.0.tar.gz 基于 Osativa.fa.xz, 只包括 Chr1-12, ChrC和ChrM (代码见 build_BSgenome.R, build_BSgenome.sh)

TxDb.Osativa.MSU.xzg_1.0.tar.gz 基于 IRGSP_v1.gff.gz 进行构建（代码见build_txdb.R)

安装方式(以BSgenome为例):

1. 在R里安装：install.pakcages("/路径/到/BSgenome.Osativa.MSU.xzg_1.0.tar.gz", repos=NULL, type="source) 
1. 在命令行里安装: R CMD INSTALL BSgenome.Osativa.MSU.xzg_1.0.tar.gz


参考资料
- https://bioconductor.org/packages/devel/bioc/vignettes/BSgenome/inst/doc/BSgenomeForge.pdf
