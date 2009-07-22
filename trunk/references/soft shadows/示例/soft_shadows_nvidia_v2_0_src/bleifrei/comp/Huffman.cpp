/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
//#include <algorithm>
//#include <climits>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "../io/BitOutStream.hpp"
#include "../io/BitInStream.hpp"

#include "Huffman.hpp"

using namespace bleifrei::io;
using namespace std;

namespace bleifrei {
	namespace comp {
		namespace huffman {

			struct Node {
				Node(Node *parent = 0)
					: freq(0), parent(parent), isRightChild(true) {}
				~Node(void)
				{
					if (!isRightChild && parent) delete parent;
				}
				int freq;
				Node *parent;
				bool isRightChild;
			};

			struct DecoderNode {
				DecoderNode(char ch = 0) : ch(ch), left(0), right(0) {}
				~DecoderNode(void) {
					if (left) delete left;
					if (right) delete right;
				}
				char ch;
				DecoderNode *left;
				DecoderNode *right;
			};

			struct greater {
				bool operator()(const Node *& n1, const Node *& n2) const
				{
					return n1->freq > n2->freq;
				}
			};

			int char2int(char ch)
			{
				return (ch < 0) ? 256 + (int)ch : ch;
			}

			char int2char(int i)
			{
				unsigned char ch = i;
				return ch;
			}

			unsigned char getCodeLength(Node *node)
			{
				if (node->parent) return getCodeLength(node->parent) + 1;
				return 0;
			}

			void writeCode(Node *node)
			{
				if (node->parent) {
					writeCode(node->parent);
					cout << node->isRightChild;
				}
			}

			void writeCode(Node *node, BitOutStream &bos)
			{
				if (node->parent) {
					writeCode(node->parent, bos);
					bos.put(node->isRightChild);
				}
			}

			void compress(istream &in, ostream &out, int numbytes)
			{
				map<char, Node *> freq;
				streampos startPosition = in.tellg();
			
				// count character frequency
				char ch;
				for (int i = 0; i < numbytes && in.get(ch).good(); ++i) {
					if (freq.find(ch) == freq.end()) {
						freq[ch] = new Node();
					}
					freq[ch]->freq++;
				}

				// create leaf nodes from characters
				priority_queue<Node *, vector<Node *>, greater()> nodes;
				map<char, Node *>::iterator it;
				for (it = freq.begin(); it != freq.end(); it++) {
					nodes.push(it->second);	
				}

				// create huffman tree
				while (nodes.size() > 1) {
					Node *n = new Node();
					Node *left = nodes.top();
					left->isRightChild = false;
					nodes.pop();
					Node *right = nodes.top();
					nodes.pop();
					left->parent = right->parent = n;
					n->freq = left->freq + right->freq;
					nodes.push(n);
				}

				// calculate code lengths
				vector<unsigned char> lengths(256,0);
				int first = 255;
				int last = 0;
				for (it = freq.begin(); it != freq.end(); it++) {
					int index = char2int(it->first);
					first = (index < first) ? index : first;
					last = (index > last) ? index : last;
					lengths[index] = getCodeLength(it->second);
				}
				if (first > last) {
					out.put(0);
					return;
				}
				
				// output code length header
				out.put(int2char(first));
				out.put(int2char(last));
				out.write((char *)&lengths[first], last - first + 1);
				
				BitOutStream bos(out);

				// output decoding table
				for (int i = first; i <= last; ++i) {
					map<char, Node *>::iterator it = freq.find(int2char(i));

					if (it != freq.end()) {
						writeCode(it->second, bos);
					}
				}
				bos.flush();

				// output compressed data
				in.seekg(startPosition);
				out.write((char *)&numbytes, sizeof(int));
				for (int i = 0; i < numbytes; ++i) {
					char ch;
					in.get(ch);
					if (freq.find(ch) == freq.end()) cout << "THROW ERROR HERE!";
					writeCode(freq[ch], bos);
				}
				bos.flush();
			}
			
			void decompress(istream &in, ostream &out)
			{
				char ch;
				in.get(ch);
				int first = char2int(ch);
				in.get(ch);
				int last = char2int(ch);

				// read code lengths
				vector<char> lengths(256, 0);
				in.read((char *)&lengths[first], last - first + 1);

				DecoderNode *root = new DecoderNode();

				// create decoder tree
				BitInStream bis(in);
				for (int i = first; i <= last; ++i) {
					if (lengths[i] != 0) {
						DecoderNode *current = root;
						for (int j = 0; j < lengths[i]; ++j) {
							bool value;
							bis.get(value);
							if (current->right == 0) current->right = new DecoderNode();
							if (current->left == 0) current->left = new DecoderNode();
							current = value ? current->right : current->left;
						}
						current->ch = int2char(i);
					}
				}
				bis.byteAlign();

				// decode compressed data
				int numbytes = 0;
				in.read((char *)&numbytes, sizeof(int));
				
				for (int i = 0; i < numbytes; ++i) {
					DecoderNode *current = root;
					while (current->right && current->left) {
						bool value;
						bis.get(value);
						current = value ? current->right : current->left;
					}
					out.put(current->ch);
				}
				bis.byteAlign();
			}

		}
	}
}