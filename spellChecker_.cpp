#include <bits/stdc++.h>
using namespace std;

class trieNode{
    public:
    char data;
    trieNode* children[26];
    bool isTerminal;

    trieNode(char ch){
        data = ch;
        for(int i=0; i<26; i++){
            children[i] = NULL;
        }
        isTerminal = false;
    }
};

class trie{
    public:
    trieNode* root;

    trie(){
        root = new trieNode('\0');
    }

    void insertUtil(trieNode* root, string word){
        if(word.length()==0){
            root->isTerminal = true;
            return;
        }

        int index = word[0]-'a';
        trieNode* child;

        if(root->children[index]!=NULL){
            child = root->children[index];
        }
        else{
            child = new trieNode(word[0]);
            root->children[index] = child;
        }

    insertUtil(child, word.substr(1));

    }

    void insertWord(string word){
        insertUtil(root, word);
    }

    bool searchUtil(trieNode* root, string word){
        if(word.length()==0){
            return root->isTerminal;
        }

        int index = word[0]-'a';
        trieNode* child;

        if(root->children[index] != NULL){
            child = root->children[index];
        }
        else{
            return false;
        }

        return searchUtil(child,word.substr(1));
    }

    bool searchWord(string word){
        return searchUtil(root,word);
    }

    void deleteUtil(trieNode* root, string word){
        if(word.length()==0){
            root->isTerminal = false;
        }

        int index = word[0] - 'a';
        trieNode* child;

        if(root->children[index]!=NULL){
            child = root->children[index];
            deleteUtil(child,word.substr(1));
        }

    }

    void deleteWord(string word){
        deleteUtil(root,word);
    }
};

class SpellChecker {
    public:
    trie* dictionary = new trie();

    void loadDictionary(vector<string> words) {
        for(auto i:words) {
            dictionary->insertWord(i);
        }
    }

    bool isCorrectWord(string word) {
        return dictionary->searchWord(word);
    }

    vector<string> suggestCorrections(string word) {
        vector<string> suggestions;
        suggestions = findSuggestions(word);
        return suggestions;
    }

    private:
    void getSuggestionsUtil(trieNode* node, string prefix, vector<string> &suggestions) {
        if (node->isTerminal){
            suggestions.push_back(prefix);
        }
            
        for(int i = 0; i<26; ++i){
            if(node->children[i] != NULL) {
                char ch = 'a' + i;
                string newPrifix = prefix+ch;
                getSuggestionsUtil(node->children[i], newPrifix, suggestions);
            }
        }
    }

    vector<string> findSuggestions(string &prefix) {
        trieNode* current = dictionary->root;

        for (char ch : prefix) {
            int index = ch - 'a';

            if(current->children[index] == NULL){
                return vector<string>();
            }
            current = current->children[index];
        }

        vector<string> suggestions;
        getSuggestionsUtil(current, prefix, suggestions);
        return suggestions;
    }
};

bool preProcess(string &word){
    string newWord = "";
    for(char c:word) {
        newWord += tolower(c);
    }
    word = newWord;

    bool isCharacterString = true;

    for(char c:word){
        if(isdigit(c)) {
            isCharacterString = false;
        }
    }
    return isCharacterString;
}

int main(){
    ifstream data("words_alpha.txt");
    if(!data.is_open()) {
        cerr<<"Error opening the file!"<<endl;
    }

    vector<string> dataInput;

    string temp;
    while(data>>temp) {
        dataInput.push_back(temp);
    }

    data.close();

    SpellChecker* spellChecker = new SpellChecker();
    spellChecker->loadDictionary(dataInput);

    string word;
    cout<<"Enter a word:"<<endl;
    cin>>word;

    if(preProcess(word)){
        if(spellChecker->isCorrectWord(word)){
            cout<<"The word is correct"<<endl;
        }
        else{
            cout<<"The word is misspelled"<<endl;
            cout<<"Suggestions:";

            vector<string> suggestions = spellChecker->suggestCorrections(word);
            for(auto i:suggestions) {
                cout<<i<<" ";
            }
            cout<<endl;
        }
    }
    else{
        cout<<"The word contain numbers"<<endl;
    } 

    return 0;
}