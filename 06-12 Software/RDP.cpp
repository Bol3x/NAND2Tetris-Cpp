#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

const unordered_map<char, int> operatorPrec = {
    {'*', 5}, {'/', 5},
    {'+', 4}, {'-', 4},
    {'<', 3}, {'>', 3},
    {'=', 2},
    {'&', 1},
    {'|', 0},
};

class Node{
    public:
        virtual void print() = 0;
};

class VarNode : public Node{
    public:
        void print() override {
            cout << terminal << " ";
        }

        VarNode(char t){
            terminal = t;
        }
        char terminal;
};

class OpNode : public Node{
    public:
        void print() override {
            cout << "( ";
            left->print();
            cout << op << " ";
            right->print();
            cout << ") ";
        }

        OpNode(char oper, Node* l, Node* r){
            op = oper;
            left = l;
            right = r;
        }

        Node* left;
        Node* right;
        char op;
};

class UnOpNode : public Node{
    public:
        void print() override {
            cout <<  "( " << op << " ";
            expr->print();
            cout << ") ";
        }

        UnOpNode(char oper, Node* e){
            op = oper;
            expr = e;
        }
    
    Node* expr;
    char op;
};

Node* P(string& str, int& idx);
Node* E(int p, string& str, int& idx);

void expectToken(char& token, char target, int& idx){
    if (token != target){
        cerr << "Expected " << target << ", Received " << token << endl;
        return;
    }
    idx++;
}

Node* E(int p, string& str, int& idx){
    Node* a = P(str, idx);
    if (a == nullptr) return nullptr;

    char op = str[idx];
    int maxPrec = 5;
    while (operatorPrec.find(op) != operatorPrec.end() && p <= operatorPrec.at(op) && operatorPrec.at(op) <= maxPrec){
        //cout << "E: " << op << endl;
        idx++;
        Node* b = E(operatorPrec.at(op)+1, str, idx);
        if (b == nullptr) return nullptr;

        a = new OpNode(op, a, b);
        maxPrec = operatorPrec.at(op);

        op = str[idx];
    }

    return a;
}

Node* P(string& str, int& idx){
    //cout << "P: " << str[idx] << endl;
    //unary ops
    if (str[idx] == '-' || str[idx] == '~'){
        char c = str[idx];
        idx++;
        Node* e = E(5, str, idx);
        return new UnOpNode(c, e);
    
    }
    //parenthesis priority (evaluate within first before merging)
    if (str[idx] == '('){
        idx++;
        Node* e = E(0, str, idx);
        expectToken(str[idx], ')', idx);
        return e;
    }
    if (str[idx] >= 'a' && str[idx] <= 'z'){
        char c = str[idx];
        idx++;
        return new VarNode(c);
    }
    return nullptr;
}

int main(){
    string str;
    cin >> str;

    cout << str << endl;
    
    int i = 0;
    Node* tree = E(0, str, i);

    if (tree == nullptr){
        cout << "Error in parsing" << endl;
    } else {
        tree->print();
    }

    return 0;
}