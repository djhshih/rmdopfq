#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstring>
#include <set>

using namespace std;


struct fastq_entry {
    /// read name
    string qname;
    /// read sequence
    string seq;
    /// read quality scores
    string qual;
};


/** 
 * Read one fastq entry from file.
 *
 * Assume that each sequence and quality score entry `r1.fq` and `r2.fq` is single-line.
 */
bool read_fastq_entry(istream& f, fastq_entry& x) {
    getline(f, x.qname);
    if (x.qname.empty()) return false;
    
    getline(f, x.seq);

    string marker;
    getline(f, marker);
    if (marker != "+") {
        throw runtime_error("fastq entry is malformed");
    }

    getline(f, x.qual);
    
    return true;
}

/** 
 * Write one fastq entry to file.
 */
void write_fastq_entry(ostream& f, const fastq_entry& x) {
    f << x.qname << endl;
    f << x.seq << endl;
    f << '+' << endl;
    f << x.qual << endl;
}

/**
 * Remove reads with duplicate query names (doppelgangers).
 */
int main(int argc, char* argv[]) {

    char* in_fname;
    char* out_fname;

    int nargs = argc - 1;
    if (nargs < 2) {
        cerr << "usage: pairfq <in.fq> <out.fq>" << endl;
        return 1;
    } else {
        in_fname = argv[1];
        out_fname = argv[2];
    }
    
    // open input and output files
    
    ifstream inf(in_fname);
    ofstream outf(out_fname);

    // process reads
    
    set<string> qnames;
    fastq_entry entry;

    while (true) {
        if (!read_fastq_entry(inf, entry)) break;

        if (qnames.find(entry.qname) == qnames.end()) {
            // record read name
            qnames.insert(entry.qname);
            // write read to file
            write_fastq_entry(outf, entry); 
        } else {
            // read name is duplicated: print entry to stderr instead
            cerr << entry.qname << endl;
        }
    }
    
    // clean up
    
    inf.close();
    outf.close();

    return 0;
}
