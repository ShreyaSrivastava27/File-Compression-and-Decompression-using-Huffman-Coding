# File-Compression-and-Decompression-using-Huffman-Coding
Command line C++ application that performs lossless file compression and decompression using Huffman Coding with binary bit-packing to reduce storage space.
<h1>Features</h1>
<ul>
  <li>Lossless compression using Huffman Tree</li>
  <li>Supports any text file (ASCII content)</li>
  <li>Efficient encoding/decoding using 'bitset', 'map', priority_queue'</li>
</ul>
<h1>Working</h1>
<h3>Compression</h3>
<ul>
  <li>Calculate frequency of each character.</li>
  <li>Build a Huffman tree (min-heap based).</li>
  <li>Assign binary codes to characters.</li>
  <li>Encode original file using these binary codes.</li>
  <li>Pack bits into bytes using 'bitset<8>' and store them using 'unsigned char'.</li>
  <li>Add padding bits and padding info to make decoding possible.</li>
</ul>
<h3>Decompression</h3>
<ul>
  <li>Read compressed file in binary mode.</li>
  <li>Create map which stores encoding and correponding character pair</li>
  <li>Decode the binary stream</li>
</ul>
<h1>Technologies used</h1>
   <h4>Language: C++</h4>
  <h4>STL: priority_queue, map, bitset, ifstream, ofstream</h4>
  <h4>File I/O Mode: ios::binary for raw byte-level operations</h4>
<h1>Outcome</h1>
<ul>
  <li>Original file hello.txt is of 11kb</li>
  <li>After compression hello.txt.huff is of 7kb</li>
  <li>Now after decompression we get hello.txt_decoded which is same as original one and of 11kb</li>
</ul>
