#include <memory>
#include <string>

class AVL;

class AVLNode {
 public:
  AVLNode(int key);
  AVLNode(int key, std::weak_ptr<AVLNode> parent);
  bool IsLeaf() const;
  bool HasLeftChild() const;
  bool HasRightChild() const;


 private:
  int key_;
  std::weak_ptr<AVLNode> parent_;
  std::shared_ptr<AVLNode> left_;
  std::shared_ptr<AVLNode> right_;
//  size_t height_;   // to store the height
//  size_t balanceFactor_;    // to store the balance factor

  friend AVL;
}; // class AVLNode

class AVL {
 public:
  AVL();

  void Insert(int key);
  bool Find(int key) const;
  std::string JSON() const;
  size_t size() const;
  bool empty() const;
  int getHeight(std::shared_ptr<AVLNode> &temp) const;  // get height of the node
  int balanceFactor(std::shared_ptr<AVLNode> &temp) const;  // get balance factor of the node


      private:
  void balance(std::shared_ptr<AVLNode> &node); // check if the node is balanced
  void leftRotate(std::shared_ptr<AVLNode> &node);  // execute left rotation
  void rightRotate(std::shared_ptr<AVLNode> &node); // execute right rotation



  std::shared_ptr<AVLNode> root_;
  size_t size_;
}; // class AVL