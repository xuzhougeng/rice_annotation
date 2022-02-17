from collections import defaultdict
import re

# gene 列表
gene_list = []

# 记录gene的详细信息: [ chr, start, end score, strand, phase]
gene_info = defaultdict(list) 
#"locus.gff"文件路径
locus_file = "locus.gff"
for line in open(locus_file):
    line = line.strip()
    items =  line.split('\t')
    chrom = items[0].replace('hr0','hr').replace('c','C')
    start,end = items[3:5]
    score,strand,phase = items[5:8]
    gene_ID = re.findall( 'ID=(.*?);', items[8] )[0]
    gene_list.append(gene_ID)
    gene_info[gene_ID] =  [chrom, start, end, score, strand, phase] 

#  记录mRNA的元件信息
# {mRNA_ID:  [ [type(UTR, CDS), chr, start, end score, strand, phase] ] }
mRNA_exon = defaultdict(list) #

#  记录gene 和 mRNA的对应关系 {gene_ID: [ [mRNA_ID, chr, start, end score, strand, phase] ]}
gene_mRNA = defaultdict(list) 

#"transcripts.gff"文件路径
transcript_file = "transcripts.gff"

for line in open(transcript_file):
    line = line.strip()
    items =  line.split('\t')
    chrom = items[0].replace('0','').replace('c','C')
    start,end = items[3:5]
    score,strand,phase = items[5:8]
    feature_type = items[2]
    if feature_type == 'mRNA':
        mRNA_ID = re.findall( 'ID=(.*?);', items[8] )[0]
        mRNA_parent = re.findall( 'Locus_id=(.*?);', items[8] )[0]
        gene_mRNA[mRNA_parent].append( [ mRNA_ID, chrom, start, end, score, strand, phase ]  )
    else:
        exon_parent = re.findall( 'Parent=(.*)', items[8] )[0]
        mRNA_exon[exon_parent].append( [ feature_type, chrom, start, end, score, strand, phase] )

# write the result
gff_line = "IRGSP_v1.gff"
f = open(gff_line,"w")
for gene in gene_list:

    # write the gene line
    chrom, start, end, score, strand, phase = gene_info[gene]
    line = f'{chrom}\tirgsp1_rep\tgene\t{start}\t{end}\t{score}\t{strand}\t{phase}\tID={gene};Name={gene}\n'
    f.write(line)
    # write mRNA line
    mRNA_infos = gene_mRNA[gene]
    for mRNA_info in mRNA_infos:
        mRNA_ID, chrom, start, end, score, strand, phase = mRNA_info
        line = f'{chrom}\tirgsp1_rep\tmRNA\t{start}\t{end}\t{score}\t{strand}\t{phase}\tID={mRNA_ID};Parent={gene};Name={mRNA_ID}\n'
        f.write(line)
        # write the exon line
        exon_infos = mRNA_exon[mRNA_ID]
        feature_count = defaultdict(int)
        for exon_info in exon_infos:
            feature_type, chrom, start, end, score, strand, phase = exon_info

            # write the feature other than exon 
            feature_count[feature_type] += 1
            exon_name = f"{mRNA_ID}:{feature_type}:{feature_count[feature_type]}"
            line = f'{chrom}\tirgsp1_rep\t{feature_type}\t{start}\t{end}\t{score}\t{strand}\t{phase}\tID={exon_name};Parent={mRNA_ID};Name={exon_name}\n'
            f.write(line)           
            
            # add exon
            feature_count["exon"] += 1
            exon_name = f'{mRNA_ID}:{feature_type}:{feature_count["exon"]}'
            line = f'{chrom}\tirgsp1_rep\texon\t{start}\t{end}\t{score}\t{strand}\t{phase}\tID={exon_name};Parent={mRNA_ID};Name={exon_name}\n'
            f.write(line) 
f.close()
