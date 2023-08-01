//
// Created by leonf on 25/07/2023.
//

#ifndef BD2_BTREE_H
#define BD2_BTREE_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <queue>
#include <vector>
#include <fstream>
class Node {
public:
    bool is_leaf;
    std::vector<int> keys;
    std::vector<Node*> children;
    Node(bool leaf = false) : is_leaf(leaf) {}
};

class BPlusTree {
public:
    int ORDER; // Orden del árbol B+Tree
    std::unordered_map<int, std::string> data; // Mapa para almacenar los datos
    //typedef Node<int> Node; // Typedef para simplificar el uso de BPlusTreeNode<KeyType>
    // Clase para representar los nodos del árbol

    Node* root; // Puntero al nodo raíz
public:
    BPlusTree(int order) : ORDER(order), root(nullptr) {}

    void insert(const int& key, const std::string& value) {
        if (root == nullptr) {
            root = new Node(true); // Crear el nodo raíz como hoja si el árbol está vacío
            root->keys.push_back(key);
            data[key] = value;
        } else {
            insertNonFull(root, key, value);
        }
    }

    // Función auxiliar para insertar en un nodo no lleno
    void insertNonFull(Node* node, const int& key, const std::string& value) {
        int i = node->keys.size() - 1;

        if (node->is_leaf) {
            // Si el nodo es una hoja, insertamos el valor en la posición correcta
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            node->keys.insert(node->keys.begin() + i + 1, key);
            data[key] = value;
        } else {
            // Si el nodo no es una hoja, encontramos el hijo apropiado y lo insertamos allí
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;

            if (node->children[i]->keys.size() == ORDER - 1) {
                // Si el hijo está lleno, lo dividimos antes de insertar
                splitChild(node, i);
                if (key > node->keys[i]) {
                    i++;
                }
            }

            insertNonFull(node->children[i], key, value);
        }
    }

    // Función auxiliar para dividir un hijo de un nodo
    void splitChild(Node* parentNode, int childIndex) {
        Node* childNode = parentNode->children[childIndex];
        Node* newNode = new Node(childNode->is_leaf);
        parentNode->keys.insert(parentNode->keys.begin() + childIndex, childNode->keys[ORDER / 2]);
        parentNode->children.insert(parentNode->children.begin() + childIndex + 1, newNode);

        for (int i = ORDER / 2 + 1; i < ORDER; ++i) {
            newNode->keys.push_back(childNode->keys[i]);
        }

        childNode->keys.erase(childNode->keys.begin() + ORDER / 2, childNode->keys.end());

        if (!childNode->is_leaf) {
            for (int i = ORDER / 2 + 1; i < ORDER + 1; ++i) {
                newNode->children.push_back(childNode->children[i]);
            }

            childNode->children.erase(childNode->children.begin() + ORDER / 2 + 1, childNode->children.end());
        }
    }

    // Función para imprimir el árbol B+Tree por niveles
    void printLevels() {
        if (root == nullptr) {
            std::cout << "El árbol está vacío." << std::endl;
            return;
        }

        // Implementar el recorrido BFS aquí para imprimir los niveles del árbol
    }
    void print() {
        if (root == nullptr) {
            std::cout << "El árbol está vacío." << std::endl;
            return;
        }

        std::queue<Node*> nodeQueue;
        nodeQueue.push(root);

        while (!nodeQueue.empty()) {
            int currentLevelSize = nodeQueue.size();
            for (int i = 0; i < currentLevelSize; ++i) {
                Node* currentNode = nodeQueue.front();
                nodeQueue.pop();

                std::cout << "[ ";
                for (const auto& key : currentNode->keys) {
                    std::cout << key << " ";
                }
                std::cout << "] ";

                if (!currentNode->is_leaf) {
                    for (Node* child : currentNode->children) {
                        nodeQueue.push(child);
                    }
                }
            }
            std::cout << std::endl;
        }
    }
    void print(std::ostream &os) {
        if (root == nullptr) {
            os << "El árbol está vacío." << std::endl;
            return;
        }

        std::queue<Node*> nodeQueue;
        nodeQueue.push(root);

        while (!nodeQueue.empty()) {
            int currentLevelSize = nodeQueue.size();
            for (int i = 0; i < currentLevelSize; ++i) {
                Node* currentNode = nodeQueue.front();
                nodeQueue.pop();

                for (size_t j = 0; j < currentNode->keys.size(); ++j) {
                    os << "Key: " << currentNode->keys[j] << " Value: " << data[currentNode->keys[j]] << std::endl;
                }

                if (!currentNode->is_leaf) {
                    for (Node* child : currentNode->children) {
                        nodeQueue.push(child);
                    }
                }
            }
        }
    }
    Node* search(const int& key) {
        if (root == nullptr) {
            return nullptr; // Árbol vacío, la clave no se encuentra
        }

        Node* currentNode = root;
        while (true) {
            int i = 0;
            while (i < currentNode->keys.size() && key > currentNode->keys[i]) {
                i++;
            }

            if (i < currentNode->keys.size() && key == currentNode->keys[i]) {
                return currentNode; // Clave encontrada, devolver el nodo actual
            }

            if (currentNode->is_leaf) {
                return nullptr; // Nodo hoja, la clave no se encuentra
            } else {
                currentNode = currentNode->children[i]; // Descendemos al siguiente nivel
            }
        }
        return nullptr;
    }
    bool remove(const int& key) {
        if (root == nullptr) {
            return false; // Árbol vacío, no se puede eliminar nada
        }

        bool removed = deleteKey(root, key);

        // Si el nodo raíz quedó vacío después de la eliminación, lo actualizamos
        if (root->keys.empty()) {
            Node* newRoot = root->children[0];
            delete root;
            root = newRoot;
        }

        return removed;
    }
    bool deleteKey(Node* currentNode, const int& key) {
        int i = 0;
        while (i < currentNode->keys.size() && key > currentNode->keys[i]) {
            i++;
        }

        if (i < currentNode->keys.size() && key == currentNode->keys[i]) {
            // Caso 1: La clave se encuentra en el nodo actual (nodo hoja)
            currentNode->keys.erase(currentNode->keys.begin() + i);
            data.erase(key);
            return true;
        }

        if (currentNode->is_leaf) {
            // Caso 2: La clave no se encuentra y el nodo es una hoja
            return false;
        } else {
            // Caso 3: La clave no se encuentra, descendemos al siguiente nivel
            Node* childNode = currentNode->children[i];
            bool removed = deleteKey(childNode, key);

            if (childNode->keys.size() < ORDER / 2) {
                // Si después de eliminar, el hijo quedó con menos de la mitad de las claves,
                // realizamos combinaciones o redistribuciones
                if (i > 0 && currentNode->children[i - 1]->keys.size() > ORDER / 2) {
                    // Redistribución con el nodo hermano izquierdo
                    redistributeLeft(currentNode, i);
                } else if (i < currentNode->children.size() - 1 && currentNode->children[i + 1]->keys.size() > ORDER / 2) {
                    // Redistribución con el nodo hermano derecho
                    redistributeRight(currentNode, i);
                } else {
                    // Combinación con el nodo hermano
                    combineNodes(currentNode, i);
                }
            }

            return removed;
        }
    }
    void redistributeLeft(Node* parentNode, int index){
        Node* leftNode = parentNode->children[index - 1];
        Node* currentNode = parentNode->children[index];

        currentNode->keys.insert(currentNode->keys.begin(), parentNode->keys[index - 1]);
        parentNode->keys[index - 1] = leftNode->keys.back();
        leftNode->keys.pop_back();

        if (!currentNode->is_leaf) {
            currentNode->children.insert(currentNode->children.begin(), leftNode->children.back());
            leftNode->children.pop_back();
        }
    }
    void redistributeRight(Node* parentNode, int index) {
        Node* rightNode = parentNode->children[index + 1];
        Node* currentNode = parentNode->children[index];

        currentNode->keys.push_back(parentNode->keys[index]);
        parentNode->keys[index] = rightNode->keys.front();
        rightNode->keys.erase(rightNode->keys.begin());

        if (!currentNode->is_leaf) {
            currentNode->children.push_back(rightNode->children.front());
            rightNode->children.erase(rightNode->children.begin());
        }
    }
    void combineNodes(Node* parentNode, int index) {
        Node* leftNode = parentNode->children[index - 1];
        Node* rightNode = parentNode->children[index];
        leftNode->keys.push_back(parentNode->keys[index - 1]);
        for (const auto& key : rightNode->keys) {
            leftNode->keys.push_back(key);
        }
        parentNode->keys.erase(parentNode->keys.begin() + index - 1);
        parentNode->children.erase(parentNode->children.begin() + index);
        delete rightNode;
    }

};
#endif //BD2_BTREE_H