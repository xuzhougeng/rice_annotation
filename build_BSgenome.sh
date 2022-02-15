
# uncompress
xz -dc Osativa.fa.xz > Osativa.fa

# split
seqkit split -i Osativa.fa -O Osativa

# rename
ls Osativa | while read id ; 
do 
  mv Osativa/$id  Osativa/${id##*_}; 
done

wk_dir=`pwd`
seq_dir=${wk_dir}/Osativa

sed "s|seq_path|${seq_dir}|" template.dcf > BSgenome.dcf

Rscript ./build_BSgenome.R


# build package
R CMD build BSgenome.Osativa.MSU.xzg

# intall package
R CMD INSTALL BSgenome.Osativa.MSU.xzg_1.0.tar.gz


# Clean
rm -rf BSgenome.Osativa.MSU.xzg
rm -rf Osativa
rm -rf Osativa.fa
rm -rf BSgenome.dcf
