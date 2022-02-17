Rscript build_txdb.R
R CMD build TxDb.Osativa.MSU.xzg
R CMD INSTALL TxDb.Osativa.MSU.xzg_1.0.tar.gz

rm -rf TxDb.Osativa.MSU.xzg
