#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

//auxiliary function
vector<string> split(const string &input, char delim = '\t');
string merge(const vector<string> &item, char delim = '\t');
string extract(vector<string> str, string key);
vector<string> &modify(vector<string>& item, 
		               string gene_id, string transcript_id);
void preprocess(vector<string> &item, 
		        vector<string> &gene_ids,
	            unordered_map<string, vector<string>> &feature_maps,
	            unordered_map<string, vector<string>> &feature_infos);	

string generate_output(string gene_id,
	            unordered_map<string, vector<string>> &feature_maps,
	            unordered_map<string, vector<string>> &feature_infos
	);


int main(int argc, char **argv)
{
	iostream::ios_base::sync_with_stdio(false);

	ifstream gff(argv[1]);
	ofstream gtf(argv[2]);

	string input ;

    vector<string> gene_ids; // store the gene ids
	unordered_map<string, vector<string>> feature_maps; //feature and its parent
	unordered_map<string, vector<string>> feature_infos; //feature infomation


	while ( getline( gff, input)  ){
		if (input[0] == '#') 
			continue;
		auto item = split(input, '\t');
		if ( item[2] == "gene" || item[2] == "mRNA" || item[2] == "exon")
			preprocess(item, gene_ids, feature_maps, feature_infos);
	}

	for (auto it = gene_ids.cbegin(); it != gene_ids.cend(); ++it){
		gtf << generate_output( *it,  feature_maps, feature_infos) << endl;
	}

	gtf.close();
	gff.close();

	return 0;
}

// split the string to vector by delimeter
vector<string> split(const string &input, char delim){

	string field;
	vector<string> item;
	istringstream line(input);
	while ( getline(line, field, delim ) ) {
		item.push_back( field );
	}
	return item;
}

//merge the vector to single string with delimeter
string merge(const vector<string> &item, char delim){
	string line;
	for (auto it = item.begin(); it != item.end(); ++it) {
		line += *it;
		line += delim;
	}
	return line.substr(0, line.size() - 1);
}

string extract(vector<string> str, string key){
    string value;
	int key_size = key.size();
	for (auto it = str.begin(); it != str.end(); ++it){
		if ((*it).substr(0,key_size) == key) {
	       value = (*it).substr(key_size+1);
		}
	}
	return value;
}

void preprocess(vector<string> &item, 
		        vector<string> &gene_ids,
	            unordered_map<string, vector<string>> &feature_maps,
	            unordered_map<string, vector<string>> &feature_infos)
{
	string feature = item[2];
	// split the attributes column
	auto attr = split(item[8], ';');
	// extract the ID
	string feature_id  = extract(attr, "ID");
	feature_infos[ feature_id ] = item; 
	vector <string> tmp;

	if ( feature == "gene"){
		gene_ids.push_back( feature_id );
	} else if (feature == "mRNA") {
		string parent_id = extract(attr, "Parent");
		if (feature_maps.find( parent_id ) == feature_maps.end()) {
			tmp.push_back(feature_id);
			feature_maps[parent_id] = tmp;
		} else{
			feature_maps[parent_id].push_back(feature_id);
		}
	} else if ( feature == "exon" ) {
		string parent_id = extract(attr, "Parent");
		if ( parent_id.find(',') == string::npos) {
			if (feature_maps.find( parent_id ) == feature_maps.end()) {
				tmp.push_back(feature_id);
				feature_maps[parent_id] = tmp;
			} else{
				feature_maps[parent_id].push_back(feature_id);
			}
		} else{
			vector<string> tx_ids = split(parent_id, ',');
			for (auto it = tx_ids.cbegin(); it != tx_ids.cend(); ++it){
				if (feature_maps.find( *it) == feature_maps.end()) {
					tmp.push_back(feature_id);
					feature_maps[*it] = tmp;
				} else{
					feature_maps[*it].push_back(feature_id);
				}
			}

		}
	}
}

string generate_output(string gene_id,
	            unordered_map<string, vector<string>> &feature_maps,
	            unordered_map<string, vector<string>> &feature_infos
		){
	vector<string> result;
    //add the gene to result
	vector<string> gene_info = feature_infos[gene_id] ;
	result.push_back( merge( modify(gene_info, "",""), '\t')) ;

    //add transcript and its exon	
	vector<string> transcripts = feature_maps[gene_id];

	for ( auto it = transcripts.cbegin(); it != transcripts.cend(); ++it){
	    vector<string> tx_info = feature_infos[*it] ;
	    result.push_back( merge( modify(tx_info, gene_id, "" ), '\t')) ;

		vector<string> exons = feature_maps[*it];
		for (auto it2 = exons.cbegin(); it2 != exons.cend(); ++it2){
	        vector<string> exon_info = feature_infos[*it2] ;
	        result.push_back( merge( modify(exon_info, gene_id, *it), '\t')) ;
		}
	}
	return merge(result, '\n');
}

vector<string> &modify(vector<string>& item, string gene_id, string transcript_id){

	string feature = item[2];
	auto attr = split(item[8], ';');
	string feature_id  = extract(attr, "ID");

	if (feature == "gene"){
		item[8] = "gene_id \"" + feature_id + "\";";
	} else if (feature == "mRNA") {
		item[2] = "transcript";
		item[8] = "gene_id \"" + gene_id + "\";" + "transcript_id \"" + feature_id + "\";";
	} else if ( feature == "exon" ){
		string parent_id = extract(attr, "Parent");
		item[8] = "gene_id \"" + gene_id + "\";" +   "transcript_id \"" + transcript_id + "\";" + "exon_id \"" + feature_id + "\";";
	} else{
		throw "unkown feature";
	}
	return item;
}
