#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "sha.h"
#include "crypto.h"
 struct block{
     unsigned char prevHash[SHA256_DIGEST_LENGTH];
     int blockData;
     struct block *link; 
 }*head;
 void addBlock(int);
 void delBlock(int);
 void verifyChain();
 void alterNthBLock(int,int);
 void hackChain();
 int hashCompare(unsigned char*,unsigned char*);
 void hashPrinter();
 unsigned char* toString(struct block);
 void printBlock();
 void printALLBlocks();
 //SHA256(source,size,dest)..returns unsigned char *
 void addBlock(int data)
 {
     if(head==NULL) //checking if block is empty
     {
         head=malloc(sizeof(struct block));
         SHA256("",sizeof(""),head->prevHash);  //if it is empty we use a blank string to calculate hash
         head->blockData=data;
         return;
     }
     struct block *currentBlock=head;
     while(currentBlock->link)
        currentBlock=currentBlock->link;
    struct block *newBlock=malloc(sizeof(struct block));
    currentBlock->link=newBlock;
    newBlock->blockData=data;
    SHA256(toString(*currentBlock),sizeof(*currentBlock),newBlock->prevHash);
 }
 void verifyChain()
 {  int count=1;
     if(head==NULL)
     {
         printf("Blockchain is empty");
         return;
     }
     struct block *curr=head->link,*prev=head;
  
     while(curr)
     {
         printf("%d\t{%d]\t",count++,curr->blockData);
         hashPrinter(SHA256(toString(*prev),sizeof(*prev),NULL));   //calculate hash of prev block
         printf(" - ");
         hashPrinter(curr->prevHash,SHA256_DIGEST_LENGTH);
         if(hashCompare(SHA256(toString(*prev),sizeof(*prev),NULL),curr->prevHash)) //compare it with prevHash variable of current block
            printf("Verified!\n");
        else
            printf("Verification Failed!\n");
        prev=curr;
        curr=curr->link;
     }

 }
 void delBlock(int k)
{ int count=1;
    struct block *curr=head,*prev,*temp;
    if(curr==NULL)
    {
        printf("The block doesnt exist\n");
        return;
    }
    while(count!=k)
    {
         if(curr->link==NULL && count!=k)
         {
              printf("Block does not exist\n");
              return;
    }
    else if(count==k)
        break;
    count++;
     temp->blockData=curr->blockData;
        temp->link=curr->link;
        curr->link=NULL;
        
       
         
    
    printf("Before: ");
        printBlock(curr);
    printf("\nAfter: ");
        printBlock(temp);
    printf("\n");
}}

 void alterNthBlock(int n,int newData)
 {  int count=1;
     struct block *curr=head;
     if(curr==NULL)
     {
         printf("Nth block does not exist!\n");
         return;
     }
     while(count!=n)
     {
        if(curr->link==NULL && count!=n)
        {
            printf("Nth block does not exist\n");
            return;
        }
        else if(count==n)
            break;
        curr=curr->link;
        count++;
     }
     printf("Before: ");
        printBlock(curr);
    curr->blockData=newData;
    printf("\nAfter: ");
        printBlock(curr);
    printf("\n");
 }
 void hackChain()
 {
     struct block *curr=head,*prev;
     if(curr==NULL)
     {
         printf("BlockChain is emplty!");
         return;
     }
     while(1)
     {
         prev=curr;
         curr=curr->link;
         if(curr==NULL)
            return;
        if(!hashCompare(SHA256(toString(*prev),sizeof(*prev),NULL),curr->prevHash))
        {
            hashPrinter(
                SHA256(toString(*prev),sizeof(*prev),curr->prevHash),
                SHA256_DIGEST_LENGTH
            );
            printf("\n");

        }
     }
 }
unsigned char* toString(struct block b)
{
    unsigned char *str=malloc(sizeof(unsigned char)*sizeof(b));
    memcpy(str,&b,sizeof(b));
    return str;
}
void hashPrinter(unsigned char hash[],int length)
{
    for(int i=0;i<length;i++)
        printf("%02x",hash[i]);
}
int hashCompare(unsigned char *str1,unsigned char *str2)
{
    for(int i=0;i<SHA256_DIGEST_LENGTH;i++)
        if(str1[i]!=str2[i])
            return 0;
        return 1;
}
void printBlock(struct block *b)
{
    printf("%p]t",b);
    hashPrinter(b->prevHash,sizeof(b->prevHash));
    printf("\t{%d]\t",b->blockData);
    printf("%p\n",b->link);
}
void printALLBlocks()
{
    struct block *curr=head;
    int count=0;
    while(curr)
    {
        printBlock(curr);
        curr=curr->link;
    }
}

void main()
{
    int c=-1,n,r,k;
    printf("1)addBlock\n2)add n random block\n3)alterNthBLock\n4)deleteBlock\n5)printALLBlocks\n6)verifyChain\n7)hackChain\n");
    while(c!=0)
    {
            printf("Choice: ");
            scanf("%d",&c);
            switch(c)
            {
                case 1:
                    printf("Enter data: ");
                    scanf("%d",&n);
                    addBlock(n);
                    break;
                case 2:
                    printf("How many blocks to enter?: ");
                    scanf("%d",&n);
                    for(int i=0;i<n;i++)
                    {
                        r=rand()%(n*10);
                        printf("Entering: %d\n",r);
                        addBlock(r);
                    }
                    break;
                case 3:
                    printf("Which block to alter?: ");
                    scanf("%d",&n);
                    printf("Enter value: ");
                    scanf("%d",&r);
                    alterNthBlock(n,r);
                    break;
                case 4:
                    printf("Which block do you want to delete?");
                    scanf("%d",&k);
                    delBlock(k);
                    break;
                case 5:
                    printALLBlocks();
                    break;
                case 6:
                    verifyChain();
                    break;
                case 7:
                    hackChain();
                    break;
                default:
                    printf("Wrong choice!\n");
                    break;
            }

    }
}
//gcc blockchian.c -o blockchain.c.o -lcrypto
//./blockchain.c.o
