#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class ComplexNode : public enable_shared_from_this<ComplexNode> 
{
public:
    string name;
    vector<shared_ptr<ComplexNode>> children;

    ComplexNode(const string& nodeName) : name(nodeName) 
    {
        cout << "Node " << name << " created.\n";
    }

    ~ComplexNode() 
    {
        cout << "Node " << name << " destroyed.\n";
    }

    void addChild(const shared_ptr<ComplexNode>& child) 
    {
        children.push_back(child);
        cout << "Added child " << child->name << " to node " << name << ".\n";
        printRefCount(child);
    }

    void removeChild(const shared_ptr<ComplexNode>& child) 
    {
        for (auto it = children.begin(); it != children.end(); ++it) 
        {
            if (*it == child) 
            {
                children.erase(it);
                cout << "Removed child " << child->name << " from node " << name << ".\n";
                printRefCount(child);
                return;
            }
        }
    }

    void traverse(int depth = 0) 
    {
        for (int i = 0; i < depth; ++i) cout << "  ";
        cout << name << "\n";
        for (const auto& child : children) 
        {
            child->traverse(depth + 1);
        }
    }

    shared_ptr<ComplexNode> findNode(const string& nodeName) 
    {
        if (name == nodeName) 
        {
            return shared_from_this();
        }
        for (const auto& child : children) 
        {
            auto found = child->findNode(nodeName);
            if (found) 
            {
                return found;
            }
        }
        return nullptr;
    }

private:
    void printRefCount(const shared_ptr<ComplexNode>& node) 
    {
        cout << "Node " << node->name << " reference count: " << node.use_count() << "\n";
    }
};

int main() 
{
    auto root = make_shared<ComplexNode>("root");
    auto child1 = make_shared<ComplexNode>("child1");
    auto child2 = make_shared<ComplexNode>("child2");
    auto grandchild1 = make_shared<ComplexNode>("grandchild1");

    root->addChild(child1);
    root->addChild(child2);
    child1->addChild(grandchild1);

    cout << "\nStructure traversal:\n";
    root->traverse();

    cout << "\nFinding nodes:\n";
    auto foundNode = root->findNode("grandchild1");
    if (foundNode) 
    {
        cout << "Node " << foundNode->name << " found.\n";
    }
    else 
    {
        cout << "Node not found.\n";
    }

    cout << "\nRemoving a child node:\n";
    child1->removeChild(grandchild1);

    cout << "\nStructure traversal after removal:\n";
    root->traverse();
}
