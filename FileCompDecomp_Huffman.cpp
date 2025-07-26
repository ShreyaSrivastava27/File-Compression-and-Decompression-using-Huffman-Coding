#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }
};

class compare {
public:
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;  // min-heap-> overloads the operator () to compare Node* and ensure a will have less priority than b that means min heap
    }
};

void buildCode(Node* root, string str, unordered_map<char, string>& huffCode) {
    if (!root) return;
    if (!root->left && !root->right) huffCode[root->ch] = str;
    buildCode(root->left, str + "0", huffCode);
    buildCode(root->right, str + "1", huffCode);
}

//-------------------------------------Compression--------------------------------------------------------------

void compress(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);

    unordered_map<char, int> freq;
    char ch;
    while (in.get(ch)) freq[ch]++;

    priority_queue<Node*, vector<Node*>, compare> pq; //creates  min heap
    for (auto& p : freq) pq.push(new Node(p.first, p.second));

    while (pq.size() > 1) {
        Node *l = pq.top(); pq.pop();
        Node *r = pq.top(); pq.pop();
        Node *sum = new Node('\0', l->freq + r->freq);
        sum->left = l;
        sum->right = r;
        pq.push(sum);
    }

    Node* root = pq.top();
    unordered_map<char, string> huffCode;
    buildCode(root, "", huffCode);

    in.clear();
    in.seekg(0); //we are writing both these lines so that next time reading of input file start from index 0

    out << freq.size() << '\n'; // In the compressed file first store the number of total char means frq map size

    // storing char and its corresponding encoding in huffcode map
    for (auto& p : huffCode) {
        if (p.first == '\n') out << "\\n " << p.second << '\n';
        else if (p.first == ' ') out << "\\s " << p.second << '\n';
        else out << p.first << " " << p.second << '\n';
    }
    out << "====\n"; // as a sepeator

    string bitString = "";
    while (in.get(ch)) bitString += huffCode[ch];  // bitstring will store string of 0s and 1s encoding
    //encoding produces a total number of bits that's not divisible by 8, we pad it with extra '0' bits to make it a full byte.
    int padding = (8 - (bitString.size() % 8)) % 8; 
    bitString.append(padding, '0');

    out << "PADDING " << padding << "\n"; // In copression file also store the padding info which will help while decompression because in decompression we need to ignore these padding bits.

    for (size_t i = 0; i < bitString.size(); i += 8) { //step through 8 bits each time
        //we need to divide entire encoding string in 8 bits chunks and write it in binary format so that actual compression happen
        //If we directly store it in string format 010101101010 will be store as array of char and one char is 1 byte so total storage will be 12*8 bits therefore instead of compression expansion will happen therfore will storing in binary format so that 12 bits storage 
        bitset<8> b(bitString.substr(i, 8)); // bitset<8> is STL function which convert into binary number b=01000001 is stored 
        out.put(static_cast<unsigned char>(b.to_ulong())); // converts b to unsigned long means b=65 and then cast it into unsigned char b = 'A' beacause put() only considers 8 bits char once...In compresed file it store as actual bonary format not readable unsigned char actually stores and writes binary data to the file — not text.
    }
// In short
// bitset<8> bits("10101010");                            // Step 1: Convert string to bitset
// unsigned long val = bits.to_ulong();                   // Step 2: Convert to number (170)
// unsigned char byte = static_cast<unsigned char>(val);  // Step 3: Convert to binary byte
// out.put(byte);                                          // Step 4: Write to file

    in.close();
    out.close();
}

//-----------------------------Decompression-----------------------------------------------------------------

void decompress(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);

    unordered_map<string, char> codeToChar;
    string line;
    getline(in, line); // In decompressed file first line store total number of char extract that
    int n = stoi(line); // convert that string to integer

    for (int i = 0; i < n; ++i) { // loop till total number of char
        getline(in, line);
        char ch;
        string code;
        if (line.substr(0, 2) == "\\n") ch = '\n', code = line.substr(3);
        else if (line.substr(0, 2) == "\\s") ch = ' ', code = line.substr(3);
        else ch = line[0], code = line.substr(2);
        codeToChar[code] = ch; // codetochar map will store encoded string and its corresponding char
    }

    getline(in, line); // skip "===="
    getline(in, line); // read PADDING line
    int padding = stoi(line.substr(8));

    string encodedBits = "";
    char byte;
    while (in.get(byte)) {  //reads byte and convert them into binary string 0101..
        bitset<8> bits(byte);
        encodedBits += bits.to_string();
    }

    encodedBits = encodedBits.substr(0, encodedBits.size() - padding); //now delete extra padding bits
    // now performing decoding
    string current = "";
    for (char bit : encodedBits) {
        current += bit;
        if (codeToChar.find(current) != codeToChar.end())
        {
            out.put(codeToChar[current]);
            current = "";
        }
    }

    in.close();
    out.close();
}

//-------------------------------------------main fun------------------------------------------------------------

int main() {
    string tocomp, todecomp;
    int choice;
    while(true){
    cout<<"Enter 1 to compress file or 2 to decompress file or 3 to exit: ";
    cin >> choice;
    switch(choice){
        case 1:
            cout << "Enter  filename to compress : ";
            cin >> tocomp;
            compress(tocomp, tocomp + ".huff");
            cout<<"Compressed file saved as "<<tocomp<<".huff\n";
            break;
        case 2:
            cout<<"Enter filename to decompress : ";
            cin >> todecomp;
            decompress(todecomp, todecomp + "_decoded.txt");
            cout << "Decompressed file saved as "<<todecomp<<"_decoded.txt\n";
            break;
        case 3:
            return 0;
        default:
            cout<<"Invalid\n";
    }
    }
    return 0;
}
