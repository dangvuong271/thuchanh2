
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <cctype>

using namespace std;

class Node {
public:
    virtual ~Node() = default;
    virtual bool evaluate(const map<char, bool> &values) const = 0;
};

class VariableNode : public Node {
    char name;
public:
    explicit VariableNode(char name) : name(name) {}
    bool evaluate(const map<char, bool> &values) const override {
        if (values.find(name) == values.end()) {
            throw invalid_argument("Bi?n " + string(1, name) + " không có giá tr?.");
        }
        return values.at(name);
    }
};

class OperatorNode : public Node {
    char op;
    unique_ptr<Node> left, right;
public:
    OperatorNode(char op, unique_ptr<Node> left, unique_ptr<Node> right = nullptr)
        : op(op), left(move(left)), right(move(right)) {}

    bool evaluate(const map<char, bool> &values) const override {
        if (op == '¬') {
            return !left->evaluate(values);
        } else if (op == '?') {
            return left->evaluate(values) && right->evaluate(values);
        } else if (op == '?') {
            return left->evaluate(values) || right->evaluate(values);
        } else if (op == '?') {
            return !left->evaluate(values) || right->evaluate(values);
        } else {
            throw invalid_argument("Toan tu khong hop le: " + string(1, op));
        }
    }
};

unique_ptr<Node> parseExpression(const string &expression, size_t &pos) {
    unique_ptr<Node> result;

    while (pos < expression.size()) {
        char c = expression[pos];

        if (c == ' ') {
            ++pos;
            continue;
        } else if (isalnum(c)) {
            result = make_unique<VariableNode>(c);
            ++pos;
        } else if (c == '¬') {
            ++pos;
            auto operand = parseExpression(expression, pos);
            result = make_unique<OperatorNode>('¬', move(operand));
        } else if (c == '(') {
            ++pos;
            result = parseExpression(expression, pos);
        } else if (c == ')') {
            ++pos;
            return result;
        } else if (c == '?' || c == '?' || c == '?') {
            char op = c;
            ++pos;
            auto right = parseExpression(expression, pos);
            result = make_unique<OperatorNode>(op, move(result), move(right));
        } else {
            throw invalid_argument("Ky tu khong hop le trong bieu thuc: " + string(1, c));
        }
    }

    return result;
}

bool isValidExpression(const string &expression) {
    int balance = 0;
    for (char c : expression) {
        if (c == '(') {
            ++balance;
        } else if (c == ')') {
            --balance;
            if (balance < 0) return false;
        } else if (!isalnum(c) && c != ' ' && c != '¬' && c != '?' && c != '?' && c != '?' && c != '(' && c != ')') {
            return false;
        }
    }
    return balance == 0;
}

int main() {
    string expression;
    map<char, bool> values;

    cout << "Nhap bieu thuc logic: ";
    getline(cin, expression);

    if (!isValidExpression(expression)) {
        cout << "Bieu thuc khong hop le!." << endl;
        return 1;
    }

    size_t pos = 0;
    unique_ptr<Node> root;
    try {
        root = parseExpression(expression, pos);
    } catch (const exception &e) {
        cout << "Loi phan tich cu phap: " << e.what() << endl;
        return 1;
    }

    cout << "Nhap cac gia tri cho bien:" << endl;
    char var;
    bool val;
    while (cin >> var >> val) {
        values[var] = val;
    }

    try {
        bool result = root->evaluate(values);
        cout << "Ket qua bieu thuc: " << (result ? "True" : "False") << endl;
    } catch (const exception &e) {
        cout << "Loi!: " << e.what() << endl;
    }

    return 0;
}

