#include "BINWORK.h"

#define MAX 4
#define MIN 2

struct btreeNode {
    int val[MAX + 1], count;
    int row[MAX + 1];
    btreeNode* link[MAX + 1];
};

class BTree {
public:
    btreeNode* root=NULL;


    /* creating new node */
    btreeNode* createNode(int val,int row, btreeNode* child) {
        btreeNode* newNode = new btreeNode;
        newNode->val[1] = val;
        newNode->row[1] = row;
        newNode->count = 1;
        newNode->link[0] = root;
        newNode->link[1] = child;
        return newNode;
    }

    /* Places the value in appropriate position */
    void addValToNode(int val,int row, int pos, btreeNode* node, btreeNode* child) {
        int j = node->count;
        while (j > pos) {
            node->val[j + 1] = node->val[j];
            node->row[j + 1] = node->row[j];
            node->link[j + 1] = node->link[j];
            j--;
        }
        node->val[j + 1] = val;
        node->row[j + 1] = row;
        node->link[j + 1] = child;
        node->count++;
    }

    /* split the node */
    void splitNode(int val,int row, int* pval,int* prow, int pos, btreeNode* node, btreeNode* child, btreeNode** newNode) {
        int median, j;

        if (pos > MIN)
            median = MIN + 1;
        else
            median = MIN;

        *newNode = new btreeNode;
        j = median + 1;
        while (j <= MAX) {
            (*newNode)->val[j - median] = node->val[j];
            (*newNode)->row[j - median] = node->row[j];
            (*newNode)->link[j - median] = node->link[j];
            j++;
        }
        node->count = median;
        (*newNode)->count = MAX - median;

        if (pos <= MIN) {
            addValToNode(val,row, pos, node, child);
        }
        else {
            addValToNode(val,row, pos - median, *newNode, child);
        }
        *pval = node->val[node->count];
        *prow = node->row[node->count];
        (*newNode)->link[0] = node->link[node->count];
        node->count--;
    }

    /* sets the value val in the node */
    int setValueInNode(int val,int row, int* pval,int* prow, btreeNode* node, btreeNode** child) {

        int pos;
        if (!node) {
            *pval = val;
            *prow = row;
            *child = NULL;
            return 1;
        }

        if (val < node->val[1]) {
            pos = 0;
        }
        else {
            for (pos = node->count;
                (val < node->val[pos] && pos > 1); pos--);
            if (val == node->val[pos]) {
                cout << "Duplicates not allowed\n";
                return 0;
            }
        }
        if (setValueInNode(val,row, pval, prow, node->link[pos], child)) {
            if (node->count < MAX) {
                addValToNode(*pval,*prow, pos, node, *child);
            }
            else {
                splitNode(*pval,*prow, pval, prow, pos, node, *child, child);
                return 1;
            }
        }
        return 0;
    }

    /* insert val in B-Tree */
    void insertion(int val,int row) {
        int flag, i,j;
        btreeNode* child;

        flag = setValueInNode(val, row, &i,&j, root, &child);
        if (flag)
            root = createNode(i,j, child);
    }

    /* copy successor for the value to be deleted */
    void copySuccessor(btreeNode* myNode, int pos) {
        btreeNode* dummy;
        dummy = myNode->link[pos];

        for (; dummy->link[0] != NULL;)
            dummy = dummy->link[0];
        myNode->val[pos] = dummy->val[1];
        myNode->row[pos] = dummy->row[1];

    }

    /* removes the value from the given node and rearrange values */
    void removeVal(btreeNode* myNode, int pos) {
        int i = pos + 1;
        while (i <= myNode->count) {
            myNode->val[i - 1] = myNode->val[i];
            myNode->row[i - 1] = myNode->row[i];
            myNode->link[i - 1] = myNode->link[i];
            i++;
        }
        myNode->count--;
    }

    /* shifts value from parent to right child */
    void doRightShift(btreeNode* myNode, int pos) {
        btreeNode* x = myNode->link[pos];
        int j = x->count;

        while (j > 0) {
            x->val[j + 1] = x->val[j];
            x->row[j + 1] = x->row[j];
            x->link[j + 1] = x->link[j];
        }
        x->val[1] = myNode->val[pos];
        x->row[1] = myNode->row[pos];
        x->link[1] = x->link[0];
        x->count++;

        x = myNode->link[pos - 1];
        myNode->val[pos] = x->val[x->count];
        myNode->row[pos] = x->row[x->count];
        myNode->link[pos] = x->link[x->count];
        x->count--;
        return;
    }

    /* shifts value from parent to left child */
    void doLeftShift(btreeNode* myNode, int pos) {
        int j = 1;
        btreeNode* x = myNode->link[pos - 1];

        x->count++;
        x->val[x->count] = myNode->val[pos];
        x->row[x->count] = myNode->row[pos];
        x->link[x->count] = myNode->link[pos]->link[0];

        x = myNode->link[pos];
        myNode->val[pos] = x->val[1];
        myNode->row[pos] = x->row[1];
        x->link[0] = x->link[1];
        x->count--;

        while (j <= x->count) {
            x->val[j] = x->val[j + 1];
            x->row[j] = x->row[j + 1];
            x->link[j] = x->link[j + 1];
            j++;
        }
        return;
    }

    /* merge nodes */
    void mergeNodes(btreeNode* myNode, int pos) {
        int j = 1;
        btreeNode* x1 = myNode->link[pos], * x2 = myNode->link[pos - 1];

        x2->count++;
        x2->val[x2->count] = myNode->val[pos];
        x2->row[x2->count] = myNode->row[pos];
        x2->link[x2->count] = myNode->link[0];

        while (j <= x1->count) {
            x2->count++;
            x2->val[x2->count] = x1->val[j];
            x2->row[x2->count] = x1->row[j];
            x2->link[x2->count] = x1->link[j];
            j++;
        }

        j = pos;
        while (j < myNode->count) {
            myNode->val[j] = myNode->val[j + 1];
            myNode->row[j] = myNode->row[j + 1];
            myNode->link[j] = myNode->link[j + 1];
            j++;
        }
        myNode->count--;
        free(x1);
    }

    /* adjusts the given node */
    void adjustNode(btreeNode* myNode, int pos) {
        if (!pos) {
            if (myNode->link[1]->count > MIN) {
                doLeftShift(myNode, 1);
            }
            else {
                mergeNodes(myNode, 1);
            }
        }
        else {
            if (myNode->count != pos) {
                if (myNode->link[pos - 1]->count > MIN) {
                    doRightShift(myNode, pos);
                }
                else {
                    if (myNode->link[pos + 1]->count > MIN) {
                        doLeftShift(myNode, pos + 1);
                    }
                    else {
                        mergeNodes(myNode, pos);
                    }
                }
            }
            else {
                if (myNode->link[pos - 1]->count > MIN)
                    doRightShift(myNode, pos);
                else
                    mergeNodes(myNode, pos);
            }
        }
    }

    /* delete val from the node */
    int delValFromNode(int val, btreeNode* myNode) {
        int pos, flag = 0;
        if (myNode) {
            if (val < myNode->val[1]) {
                pos = 0;
                flag = 0;
            }
            else {
                for (pos = myNode->count;
                    (val < myNode->val[pos] && pos > 1); pos--);
                if (val == myNode->val[pos]) {
                    flag = 1;
                }
                else {
                    flag = 0;
                }
            }
            if (flag) {
                if (myNode->link[pos - 1]) {
                    copySuccessor(myNode, pos);
                    flag = delValFromNode(myNode->val[pos],  myNode->link[pos]);
                    if (flag == 0) {
                        cout << "Given data is not present in B-Tree\n";
                    }
                }
                else {
                    removeVal(myNode, pos);
                }
            }
            else {
                flag = delValFromNode(val, myNode->link[pos]);
            }
            if (myNode->link[pos]) {
                if (myNode->link[pos]->count < MIN)
                    adjustNode(myNode, pos);
            }
        }
        return flag;
    }

    /* delete val from B-tree */
    void deletion(int val, btreeNode* myNode) {
        btreeNode* tmp;
        if (!delValFromNode(val, myNode)) {
            cout << "Given value is not present in B-Tree\n";
            return;
        }
        else {
            if (myNode->count == 0) {
                tmp = myNode;
                myNode = myNode->link[0];
                free(tmp);
            }
        }
        root = myNode;
        return;
    }

    /* search val in B-Tree */
    int searching(int val, int* pos, btreeNode* myNode) {
        if (!myNode) {
            return -1;
        }

        if (val < myNode->val[1]) {
            *pos = 0;
        }
        else {
            for (*pos = myNode->count;
                (val < myNode->val[*pos] && *pos > 1); (*pos)--);
            if (val == myNode->val[*pos]) {
                cout << "Given data is Found\n";
                return myNode->row[*pos];
            }
        }
        searching(val, pos, myNode->link[*pos]);
        return -1;
    }

    /* B-Tree Traversal */
    void traversal(btreeNode* myNode, int l) {
        int i;
        if (myNode) {
            for (i = 0; i < myNode->count; i++) {
                traversal(myNode->link[i],l+1);
                for (int i = 1; i <= l; i++) cout << "   ";
                cout << myNode->val[i + 1] << '|' << myNode->row[i + 1]<<'\n';
            }
            traversal(myNode->link[i],l+1);
        }
    }

    //void printTree(node* w, int l)//Вывод дерева слева направо
    //{
    //    if (w != NULL)
    //    {
    //        printTree(w->right, l + 1);
    //        for (int i = 1; i <= l; i++) cout << "   ";
    //        cout << w->key << ' ' << w->row << endl;
    //        printTree(w->left, l + 1);
    //    }
    //}
};

int main() {
    BTree bt;
    int val, code, opt, curPos=0;
    string surname;
    record r;
    cout << "1-Add record\n2-Delete record\n3-Print tree\n4-Find record\n0-exit\n";

    //Имя бинарного файла в котором хранится вся информация
    string binName = "data";
    //Poggers.txt файл с исходными записями, можно изменить название
    CreateBinFile("Poggers.txt", binName);

    vector<record> R = outBinFile(binName);

    for (int i = 0; i < R.size(); ++i) {
        bt.insertion(R[i].key, i);
        curPos++;
    }
    while (true) {
        
        cin >> opt;
        cout << endl;
        switch (opt) {
        case 1:
            cout << "Enter your input:";
            cin >> val>>code>>surname;
            bt.insertion(val,curPos);
            if (bt.searching(val, &opt, bt.root) == -1) break;
            r.key = val;
            r.code = code;
            surname.copy(r.data, surname.size() + 1);
            addRecord(binName, r);
            break;
        case 2:
            cout << "Enter the element to delete:";
            cin >> val;
            code = bt.searching(val, &opt, bt.root);
            if (code != -1) {
                deleteRecord(binName, code);
            }
            bt.deletion(val, bt.root);

            break;
        case 3:
            bt.traversal(bt.root,0);
            break;
        case 4:       
            cout << "Enter the element to search:";
            cin >> val;
            bt.searching(val, &opt, bt.root);
            break;
        case 0:
            return 0;
        }
        cout << "Done!\n";
    }

    //
}
