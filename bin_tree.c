#include<stdio.h>
#include<stdlib.h>




#define FALSE 0
#define TRUE  1


#define RET_OK            0
#define NODE_ALREADY_EXIT 1
#define INPUT_ERROR       2
#define ALLOC_ERROR       3
#define LOGIC_ERROR       4



typedef unsigned int KEY;

typedef struct node{

    KEY key;
	struct node *parent;
	struct node *left;
	struct node *right;
}BTNODE,*pBTNODE;


typedef enum{
    PRE_SEQ    =0,
    MID_SEQ    =1,
    NEXT_SEQ     ,

}TRAVE_SEQ;

#define ARRY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))


KEY src_node_key[]={8,4,12,2,6,10,14,1,3,5,7,9,11,13,15};


static pBTNODE find_node(pBTNODE root,KEY key);
static int key_in_tree(pBTNODE root,KEY key);
static pBTNODE alloc_node(KEY key);
static int insert_node_to_tree(pBTNODE root, KEY insert_key);
static pBTNODE get_next_node(pBTNODE node);
static int  instead_node(pBTNODE old , pBTNODE new);
static int del_node(pBTNODE *proot,KEY key);
static int travl_tree(pBTNODE root,TRAVE_SEQ seq);




static pBTNODE find_node(pBTNODE root,KEY key)
{
    pBTNODE get_node = NULL;
    /*todo */
    pBTNODE node = root;
    if(!root){
        printf("Err:%s:tree is empty\n",__FUNCTION__);
        return NULL;
    }
    while(node){
        if(key < node->key){
            node = node->left;
        }else if (key > node->key){
            node = node->right;
        }else{
            get_node = node;
            break;
        }
    }
    return get_node;

}

static int key_in_tree(pBTNODE root,KEY key)
{
    if(!root){
        printf("Err:%s:tree is empty\n",__FUNCTION__);
        return -INPUT_ERROR;
    }
    return  ((find_node(root,key) == NULL) ? FALSE:TRUE);
}


static pBTNODE alloc_node(KEY key)
{
    pBTNODE p = malloc(sizeof(BTNODE));
    if(p == NULL){
        return NULL;
    }
    p->key    = key;
    p->left   = NULL;
    p->right  = NULL;
    p->parent = NULL;

    return p;
}


static int insert_node_to_tree(pBTNODE root, KEY insert_key)
{
    int ret          = RET_OK;
    pBTNODE parent   = NULL;
    pBTNODE temp     = NULL;
    pBTNODE new_node = NULL;

    new_node = alloc_node(insert_key);
    if(new_node == NULL){
        printf("Err:%s:alloc new node is NULL\n",__FUNCTION__);
        return -ALLOC_ERROR;
    }
    new_node->key = insert_key;

    parent = root;

    while(parent){
        temp = parent;
        if(insert_key > parent->key){
            parent = parent->right;
        }else{
            parent = parent->left;
        }
    }
    /*temp is the node to insert*/
    if(insert_key > temp->key){
        temp->right = new_node;
    }else if (insert_key < temp->key){
        temp->left = new_node;
    }else{
        printf("Err:%s:insert_key:%d is already in the tree\n",__FUNCTION__,insert_key);
        return -NODE_ALREADY_EXIT;
    }

    new_node->parent = temp;

    printf("key:%d node:%p\n",new_node->key,new_node);

    return ret;
}

static pBTNODE get_next_node(pBTNODE node)
{
    pBTNODE next_node = NULL;
    /*left and right node is exit*/
    if(node->right->left == NULL){
        next_node = node->right;
    }else{
        next_node = node->right;
        while(next_node->left){
            next_node = next_node->left;
        }
    }
    return next_node;
}


static int  instead_node(pBTNODE old , pBTNODE new)
{
    if(old == NULL){
        printf("Err:%s:input old is NULL\n",__FUNCTION__);
        return -INPUT_ERROR;
    }

    if(old->parent){
        /*old is not root*/
        if(old == old->parent->left){
            old->parent->left = new;
        }else if (old == old->parent->right){
            old->parent->right= new;
        }else{
            printf("BUG:instead_node\n");
        }

    }

    if(new){
        new->parent = old->parent;
    }
    return RET_OK;
}

static int del_node(pBTNODE *proot,KEY key)
{
    int ret = RET_OK;
    pBTNODE del_node = NULL;
    pBTNODE next_node = NULL;
    pBTNODE temp_node = NULL;
    pBTNODE root = NULL;

    if((!proot)&&(!*proot)){
         printf("Err:%s:tree is empty\n",__FUNCTION__);
         return -INPUT_ERROR;
    }

    root = *proot;

    if(key_in_tree(root,key) == FALSE)
    {
        /*key is not in the tree*/
        printf("Err:%s:key:%d is not in the tree\n",__FUNCTION__,key);
        return -NODE_ALREADY_EXIT;
    }
    /*case1 : no childret */
    del_node = find_node(root,key);
    if(!del_node){
        printf("key is not in the tree\n");
        return RET_OK;
    }
    printf("find del key:%d node:%p\n",del_node->key,del_node);

    if((del_node->left == NULL) && (del_node->right == NULL)){
        instead_node(del_node,NULL);

    }else if((del_node->left == NULL)&&(del_node->right != NULL)){
        /*only has right node*/
        instead_node(del_node,del_node->right);

    }else if((del_node->left != NULL)&&(del_node->right == NULL)){
        /*only has left node*/
        instead_node(del_node,del_node->left);
    }else{
        next_node = get_next_node(del_node);
        printf("find next_node key:%d next_node:%p\n",next_node->key,next_node);
        /*this node both has left and right node*/
        if(del_node->right == next_node){
            /*next node is del_node->right*/
            instead_node(del_node,next_node);
            next_node->left = del_node->left;
            del_node->parent = next_node;


        }else{
            /*next node is not del_node->right*/

            instead_node(next_node,next_node->right);
            instead_node(del_node,next_node);
            next_node->left = del_node->left;
            next_node->right= del_node->right;
            del_node->left->parent = next_node;
            del_node->right->parent = next_node;
        }
    }
    /*next_node is root,change root*/
    if((next_node) &&(next_node->parent == NULL)){

        *proot = next_node;
    }

    free(del_node);

    return RET_OK;

}

/*mid trave*/
static int travl_tree(pBTNODE root,TRAVE_SEQ seq)
{

    if(!root){
        return RET_OK;
    }

    if(seq == MID_SEQ){
        //printf("   key:%d node:%p",root->key,root);
        printf(" %d",root->key);
        travl_tree(root->left,seq);
        travl_tree(root->right, seq);
    }else if(seq == PRE_SEQ){
        travl_tree(root->left, seq);
        //printf("   key:%d node:%p",root->key,root);
        printf(" %d",root->key);
        travl_tree(root->right, seq);
    }else if(seq == NEXT_SEQ){
        travl_tree(root->right, seq);
        //printf("   key:%d node:%p",root->key,root);
        printf(" %d",root->key);
        travl_tree(root->left, seq);
    }

    return RET_OK;

}


/*
* root key=1
* insert[5,4,7,]
*/




int main()
{
    int i = 0;
    int ret = RET_OK;
    pBTNODE root= alloc_node(src_node_key[0]);
    for(i=1;i<(ARRY_SIZE(src_node_key)); i++){
      ret = insert_node_to_tree(root,src_node_key[i]);
      if(ret){
        printf("Err:%s:insert_node_to_tree\n",__FUNCTION__);
        return -LOGIC_ERROR;
      }
    }

    printf("serch tree\n");

    ret = travl_tree(root,PRE_SEQ);
    if(ret){
        printf("Err:%s:travl_tree\n",__FUNCTION__);
        return -1;
    }
    printf("\n");

    printf("del root key:%d\n",root->key);
    ret = del_node(&root,root->key);
    if(ret){
        printf("Err:%s:del_node\n",__FUNCTION__);
        return -1;
    }
    ret = travl_tree(root,PRE_SEQ);
    if(ret){
        printf("Err:%s:travl_tree\n",__FUNCTION__);
        return -1;
    }

    printf("\n");
    printf("new root key:%d\n",root->key);

    printf("del key=12 \n");
    ret = del_node(&root,12);
    if(ret){
       printf("Err:%s:del_node\n",__FUNCTION__);
       return -1;
    }
    ret = travl_tree(root,PRE_SEQ);
    if(ret){
        printf("Err:%s:travl_tree\n",__FUNCTION__);
        return -1;
    }
    printf("\n");

    printf("del key=11 \n");
    ret = del_node(&root,11);
    if(ret){
       printf("Err:%s:del_node\n",__FUNCTION__);
       return -1;
    }
    ret = travl_tree(root,PRE_SEQ);
    if(ret){
        printf("Err:%s:travl_tree\n",__FUNCTION__);
        return -1;
    }
    printf("\n");

    printf("del key=6 \n");
    ret = del_node(&root,6);
    if(ret){
        printf("Err:%s:del_node\n",__FUNCTION__);
        return -1;
    }
    ret = travl_tree(root,PRE_SEQ);
    if(ret){
        printf("Err:%s:travl_tree\n",__FUNCTION__);
        return -1;
    }
    printf("\n");

    printf("del root key:%d\n",root->key);
    ret = del_node(&root,root->key);
    if(ret){
        printf("Err:%s:del_node\n",__FUNCTION__);
        return -1;
    }
    ret = travl_tree(root,PRE_SEQ);
    if(ret){
        printf("Err:%s:travl_tree\n",__FUNCTION__);
        return -1;
    }

    printf("\n");
    printf("new root key:%d\n",root->key);





    return 0;
}




















































