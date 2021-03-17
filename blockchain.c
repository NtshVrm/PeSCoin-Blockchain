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
void verifyChain();
void alterNthBLock(int,int);
void reconfigChain();
int hashCompare(unsigned char*,unsigned char*);
void hashPrinter();
unsigned char* toString(struct block);
void printBlock();
void printALLBlocks();
//SHA256(source,size,dest)..returns unsigned char *
void addBlock(int data)
{
    if(head==NULL)
    {
        head=malloc(sizeof(struct block));
        SHA256("",sizeof(""),head->prevHash);
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
{   int count=1;
    if(head==NULL)
    {
        printf("Blockchain is empty");
        return;
    }
    struct block *curr=head->link,*prev=head;

    while(curr)
    {
        printf("%d\t{%d}\t",count++,curr->blockData);
        hashPrinter(SHA256(toString(*prev),sizeof(*prev),NULL));
        printf(" - ");
        hashPrinter(curr->prevHash,SHA256_DIGEST_LENGTH);
        if(hashCompare(SHA256(toString(*prev),sizeof(*prev),NULL),curr->prevHash)) 
            printf("\tVerified!\n");
        else
            printf("\tVerification Failed!\n");
        prev=curr;
        curr=curr->link;
    }

}
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
void reconfigChain()
{
    struct block *curr=head,*prev;
    if(curr==NULL)
    {
        printf("BlockChain is empty!");
        return;
    }
    while(curr!=NULL)
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
            printf("Chain successfuly reconfigured!");
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
    hashPrinter(b->prevHash,sizeof(b->prevHash));
    printf("\t{%d}\t\n",b->blockData);
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
int main()
{
    int n,r,c=-1;
    // printf("1)addBlock\n2)add n random blocks\n3)alterNthBLock\n4)Print Chains\n5)Verify Chain\n6)Reconfigure Chain\n0)Exit :( \n");
    while(c!=0)
    {
            printf("\n\n1)addBlock\n2)add n random blocks\n3)alterNthBLock\n4)Print Chains\n5)Verify Chain\n6)Reconfigure Chain\n0)Exit :( \n");

            printf("\nEnter your choice: ");
            scanf("%d",&c);
            switch(c)
            {
                case 0:
                    printf("\nBye ;_;\n");
                    break;
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
                    printALLBlocks();
                    break;
                case 5:
                    verifyChain();
                    break;
                case 6:
                    reconfigChain();
                    break;
                default:
                    printf("Invalid choice, please try again!\n");
                    c=-1;
                    break;
            }

    }
    return 0;
}