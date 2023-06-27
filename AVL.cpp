#include "AVL.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <queue>
#include <math.h>

#include "json.hpp"


AVLNode::AVLNode(int key) :
    key_(key),
    parent_(std::weak_ptr<AVLNode>()),
    left_(nullptr),
    right_(nullptr){}

AVLNode::AVLNode(int key, std::weak_ptr<AVLNode> parent) :
    key_(key),
    parent_(parent),
    left_(nullptr),
    right_(nullptr){}

bool AVLNode::IsLeaf() const {
    return left_ == nullptr && right_ == nullptr;
}

bool AVLNode::HasLeftChild() const {
    return left_ != nullptr;
}

bool AVLNode::HasRightChild() const {
    return right_ != nullptr;
}

AVL::AVL() : root_(nullptr), size_(0) {}

void AVL::Insert(int key) {
    if (root_ == nullptr) {
        root_ = std::make_shared<AVLNode>(key);
        size_++;
        return;
    }
    std::shared_ptr<AVLNode> currentNode = root_, lastNode = nullptr;
    while (currentNode != nullptr) {
        lastNode = currentNode;
        currentNode = (key < currentNode->key_) ?
                      currentNode->left_ : currentNode->right_;
    }
    if (key < lastNode->key_) {
        lastNode->left_ = std::make_shared<AVLNode>(key, lastNode);
    } else {
        lastNode->right_ = std::make_shared<AVLNode>(key, lastNode);
    }
    size_++;
    balance(lastNode);  // since the above code is insertion for BST, so for AVL, will need to re-balance the tree after insertion

}

void AVL::balance(std::shared_ptr<AVLNode> &node){
    while(node != nullptr){ // when the node is not NULL
//        node->height_ = getHeight(node);  // get the height of the node
        int BF = balanceFactor(node);   // get the balance factor of the node
        if (BF > 1) {   // if BF > 1, i.e. the node is right unbalanced
            if (getHeight(node->right_->left_) > getHeight(node->right_->right_)) {
                // check the direction of rotation, if the height of rl > the height of rr, then need RL rotation
                rightRotate(node->right_);  //  right-rotate first
            }
            leftRotate(node); // for LL rotation, just left-rotate; for RL rotation, left-rotate after right-rotation
        }

        if (BF < -1) {   // if BF < -1, i.e. the node is left unbalanced
            if (getHeight(node->left_->right_) > getHeight(node->left_->left_)) {
                // check the direction of rotation, if the height of lr > the height of ll, then need LR rotation
                leftRotate(node->left_);    // left-rotate first
            }
            rightRotate(node);  // for RR rotation, just right-rotate; for LR rotation, right-rotate after left-rotation
        }
        node = node->parent_.lock();    // here -1<BF<1, so the node is balances, check its parent
    }

}


void AVL::leftRotate(std::shared_ptr<AVLNode> &node){   // left rotation
    // Save a pointer to the right child of the current node
    std::shared_ptr<AVLNode> right_child = node->right_;
    node->right_ = right_child->left_;  // now the right child of the node is its original right-child's left child
    right_child->left_ = node;  // the new left child of the original right child is the current node

    if (node->right_ != nullptr){   // if the current node has a right child
        node->right_->parent_ = node;   // set its parent as the current node
    }
    right_child->parent_ = node->parent_;   // now that exchange the current node and its right child
    node->parent_ = right_child;    // reset the parent of the current node as its original right child

    // Update the root pointer if necessary
    if (root_ == node) {    // if the node is root, then it has no parent
        root_ = right_child;    // just need to reset root as the right_child
    }
    // if the node is not root, then set their parent-child relation
    else if (right_child->parent_.lock()->left_ == node){
        right_child->parent_.lock()->left_ = right_child;   // build the relationship from parent of the current node to right-child
    }else{
        right_child->parent_.lock()->right_ = right_child;  // build the relationship from parent of the current node to right-child
    }
}

void AVL::rightRotate(std::shared_ptr<AVLNode> &node){  // right rotation
    // Save a pointer to the left child of the current node
    std::shared_ptr<AVLNode> left_child = node->left_;

    node->left_ = left_child->right_; // now the left child of the node is its original left-child's right child
    left_child->right_ = node;  // the new left child of the original right child is the current node

    if (node->left_!= nullptr){ // if the current node has a left child
        node->left_->parent_ = node;    // set its parent as the current node
    }
    left_child->parent_ = node->parent_;    // now that exchange the current node and its left child
    node->parent_ = left_child; // reset the parent of the current node as its original left child

    // Update the root pointer if necessary
    if (root_ == node) {    // if the node is root, then it has no parent
        root_ = left_child; // just need to reset root as the left_child
    }
    // if the node is not root, then set their parent-child relation
    else if (left_child->parent_.lock()->left_ == node){
        left_child->parent_.lock()->left_ = left_child;  // build the relationship from parent of the current node to left-child
    } else{
        left_child->parent_.lock()->right_ = left_child;    // build the relationship from parent of the current node to left-child
    }
}

int AVL::getHeight(std::shared_ptr<AVLNode> &temp) const{   // get the height of the node
    if (temp == nullptr){   // if the node is empty
        return -1;  // then the height is -1
    } else {
        return std::max(getHeight(temp->left_), getHeight(temp->right_)) + 1;   // recursively execute this function to get the height of the node
    }
}

int AVL::balanceFactor(std::shared_ptr<AVLNode> &temp) const{   // compute the balance factor of the node
    return (getHeight(temp->right_) - getHeight(temp->left_));  // BF = right_height - left_height
}

size_t AVL::size() const {
    return size_;
}

bool AVL::empty() const {
    return size_ == 0;
}

bool AVL::Find(int key) const {
    std::shared_ptr<AVLNode> currentNode = root_;
    while (currentNode != nullptr) {
        if (currentNode->key_ == key) {
            return true;
        }
        currentNode = (key < currentNode->key_) ?
                      currentNode->left_ : currentNode->right_;
    }
    return false;
}

std::string AVL::JSON() const {
    nlohmann::json result;
    std::queue< std::shared_ptr<AVLNode> > nodes;
    if (root_ != nullptr) {
        result["root"] = root_->key_;
        nodes.push(root_);
        while (!nodes.empty()) {
            auto v = nodes.front();
            nodes.pop();
            std::string key = std::to_string(v->key_);
            if (v->left_ != nullptr) {
                result[key]["left"] = v->left_->key_;
                nodes.push(v->left_);
            }
            if (v->right_ != nullptr) {
                result[key]["right"] = v->right_->key_;
                nodes.push(v->right_);
            }
            if (v->parent_.lock() != nullptr) {
                result[key]["parent"] = v->parent_.lock()->key_;
            } else {
                result[key]["root"] = true;
                result["height"] = getHeight(v);
            }
            result[key]["height"] = getHeight(v);
            result[key]["balance factor"] = balanceFactor(v);
        }
    }
    result["size"] = size_;
    return result.dump(2) + "\n";
}
