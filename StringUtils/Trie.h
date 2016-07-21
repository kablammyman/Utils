//http://login2win.blogspot.com/2011/06/c-tries.html

#include <iostream>
#include <vector>

class Node {
public:
	Node() { mContent = ' '; mMarker = false; }
	~Node() {}
	char content() { return mContent; }
	void setContent(char c) { mContent = c; }
	bool wordMarker() { return mMarker; }
	void setWordMarker() { mMarker = true; }
	Node* findChild(char c);
	void appendChild(Node* child) { mChildren.push_back(child); }
	std::vector<Node*> children() { return mChildren; }

private:
	char mContent;
	bool mMarker;
	std::vector<Node*> mChildren;
};

class Trie {
public:
	Trie();
	~Trie();
	void AddWordToTrie(std::string s);
	bool SearchWordInTrie(std::string s);

private:
	Node* root;
};


