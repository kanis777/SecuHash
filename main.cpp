#include<iostream>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<cstdlib>
#include<iomanip>
#include<math.h>
#include<vector>

using namespace std;

struct node
{
    int hashd;
    struct node*lchild;
    struct node*rchild;
    node(int data)
    {
        this->hashd=data;
    }
};

class hashes
{
public:
    int findhash(long long int newhashd)
    {
        return (newhashd)%9;
    }

    void printnodehashes(vector<node*> v)
    {
        for(int i=0;i<v.size();i++)
        {
            cout<<v[i]->hashd<<setw(5);
        }
        cout<<"\n";
    }
};


class merkletree
{
    struct node*root;
    int level;
public:
    merkletree(vector<node*> blocks,hashes h)
    {
        vector<node*> nodes;
        level=1;
        while(blocks.size()!=1)
        {
            h.printnodehashes(blocks);
            int n=0;
            if(blocks.size()%2!=0)
            {
                for(int i=0;i<blocks.size();i=i+2)
                {
                // when it does not reach the end of the vector blocks
                    if(i!=blocks.size()-1)
                    {
                        nodes.push_back(new node(h.findhash(blocks[i]->hashd + blocks[i+1]->hashd)));
                        nodes[n]->lchild=blocks[i];
                        nodes[n]->rchild=blocks[i+1];
                        n++;
                    }
                    else
                    {
                        nodes.push_back(new node(h.findhash(blocks[i]->hashd + blocks[i]->hashd)));
                    }
                }

            }
            else
            {
                for(int i=0;i<blocks.size();i=i+2)
                {
                    // when it does not reach the end of the vector blocks
                    if(i!=blocks.size()-1)
                    {
                        nodes.push_back(new node(h.findhash(blocks[i]->hashd + blocks[i+1]->hashd)));
                        nodes[n]->lchild=blocks[i];
                        nodes[n]->rchild=blocks[i+1];
                        n++;
                    }
                }
            }
            cout<<"\n";
            level++;
            blocks=nodes;
            nodes.clear();
        }
        this->root=blocks[0];
    }

    struct node* getroot()
    {
        return this->root;
    };

    void printtree(struct node *n)
    {
        if(n!=NULL)
        {
            if(n->lchild!=NULL)
            {
                printtree(n->lchild);
            }
            if(n->rchild!=NULL)
            {
                printtree(n->rchild);
            }
            cout<<n->hashd<<setw(5);
        }
    }

    void verifytransaction(long long int tid,int nt)
    {
       long long int t;
       int flag=0;
       ifstream in("transaction.txt");
       if(!in)
       {
           cerr<<"Errror opening file\n";
       }
       else
       {
           for(int i=0;i<nt;i++)
           {
               in>>t;
               if(t==tid)
               {
                   cout<<"\n----------------------------TRANSACTION FOUND----------------------------- \n";
                   flag=0;
                   break;
               }
               else
               {
                   flag++;
               }
           }
       }
       if(flag!=0)
       {
           cout<<"\nTransaction Not Found (No Such Transaction Made)\n";
       }

    }

    void postorder(struct node*n)
    {
        if(n!=NULL)
        {
            postorder(n->lchild);
            postorder(n->rchild);
            cout<<n->hashd<<setw(5);
        }

    }

    void inorder(struct node*n)
    {
        if(n!=NULL)
        {
            inorder(n->lchild);
            cout<<n->hashd<<setw(5);
            inorder(n->rchild);
        }
    }

    void preorder(struct node*n)
    {
        if(n!=NULL)
        {
            cout<<n->hashd<<setw(5);
            preorder(n->lchild);
            preorder(n->rchild);
        }
    }

    void deletetree(struct node *n)
    {
        if(n!=NULL)
        {
            deletetree(n->lchild);
            deletetree(n->rchild);
            n=NULL;
            delete n;
        }
    }

    ~merkletree()
    {
        deletetree(root);
        cout<<"Tree Deleted"<<endl;
    }
};

int main()
{
    vector<node*> leaves;
    hashes h;
    string line;
    int nt=0;
    ifstream in("transaction.txt");
    if(!in)
    {
        cerr<<"Error Opening File\n";
        exit(1);
    }
    else
    {
        while(in.peek()!=EOF)
        {
            getline(in,line);
            nt++;
        }
    }
    in.seekg(0,ios::beg);
    for(int i=0;i<nt;i++)
    {
        long long int t=0;
        in>>t;
        leaves.push_back(new node(h.findhash(t)));
    }
    in.close();

    for(int i=0;i<leaves.size();i++)
    {
        leaves[i]->lchild=NULL;
        leaves[i]->rchild=NULL;
    }

    //creating an object of class merkeltree -parameterized constructor
    merkletree *hashtree=new merkletree(leaves,h);
    cout<<(hashtree->getroot())->hashd<<endl;
    cout<<"\n";
    hashtree->printtree(hashtree->getroot());
    cout<<"\n";
    cout<<"-----------------------------------------------------------------------------------\n";
    cout<<"                            POSTORDER TRAVERSAL         \n";
    hashtree->postorder(hashtree->getroot());
    cout<<"\n";

    cout<<"                            INORDER TRAVERSAL           \n";
    hashtree->inorder(hashtree->getroot());
    cout<<"\n";

    cout<<"                            PREORDER TRAVERSAL          \n";
    hashtree->preorder(hashtree->getroot());
    cout<<"\n\n\n";

    cout<<"----------------------------Search For A Particular Transaction---------------------\n\n";
    long long int tid;
    cout<<"Enter Transaction ID to Search : ";
    cin>>tid;
    hashtree->verifytransaction(tid,nt);

    for(int k=0;k<leaves.size();k++)
    {
        delete leaves[k];
    }
    return 0;
}
