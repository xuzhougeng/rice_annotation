library("GenomicFeatures")

#seq.circ <- c("ChrC", "ChrM")

fn <- "IRGSP_v1.gff.gz"

metadata <- data.frame(
    name = "Resource URL",
    value = "https://rapdb.dna.affrc.go.jp/download/irgsp1.html"
)

txdb <- makeTxDbFromGFF(
        file = fn,
        format = "gff",
        dataSource = "MSU7",
        organism = "Oryza sativa",
        #circ_seqs = seq.circ,
        metadata = metadata)


makeTxDbPackage(
        txdb,
        version = "1.0",
        maintainer = "Xu ZhouGeng <xuzhougeng@163.com>",
        author = "Xu ZhouGeng <xuzhougeng@163.com>",
        license = "GPL (>= 2)",
        pkgname = "TxDb.Osativa.MSU.xzg",
        provider = "RAP-DB",
        providerVersion = "1.0" 
        )
