#include "cs537.h"
#include "request.h"
#include "threads.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;


int fill = 0;
int use = 0;
int count = 0;
int max=0;

void getargs(int *port, int *threads, int *buffers, int argc, char *argv[])
{
    if (argc != 4) { 
	    fprintf(stderr, "Usage: %s <port> <threads> <buffers>\n", argv[0]);
	    exit(1);
    }
    
    *port = atoi(argv[1]);
    *threads = atoi(argv[2]);
    *buffers = atoi(argv[3]); 
    
    if (*port < 2000) {
      fprintf(stderr, "error: port must be >= 2000");
      exit(1);
    }

    if (*threads < 1) {
      fprintf(stderr, "error: threads must be > 0");
      exit(1);
    } 
    
    if (*buffers < 1) {
      fprintf(stderr, "error: buffers must be > 0");
      exit(1);
    }
}


#define KEY_RANGE 10000    


// doubly-linked list with a reference to key:

struct CacheListNode{
	int val;
	int tokey;
	struct CacheListNode* next;
    struct CacheListNode* prev;	
};

// basic variables for the LRU cache:

struct CacheListNode* map[ KEY_RANGE];      //should be a hash table, but single array works well here.

struct CacheListNode* head;
struct CacheListNode* tail;
int maxsize;
int size;

// add new element or update recently used element in the map and at the end of the list
void add(int n,int key){
	size++;
    struct CacheListNode* temp=(struct CacheListNode*)malloc(sizeof(struct CacheListNode*));
	temp->val=n;
	temp->tokey=key;
	temp->prev=tail;
	temp->next=NULL;
	tail->next=temp;
    tail=temp;
	map[key]=temp;
	return;
}

// delete an element at specified position: 

void deletenode(struct CacheListNode* temp){
	size--;
	map[temp->tokey]=NULL;
	if(temp!=tail){
        temp->prev->next=temp->next;
	    temp->next->prev=temp->prev;
	}else{
		tail=temp->prev;
		tail->next=NULL;
	}
	free (temp);
	return;
}

// create the cache:

void lruCacheInit() {  
    head=(struct CacheListNode*)malloc(sizeof(struct CacheListNode*));
	tail=head;
	maxsize=KEY_RANGE;
	size=0;
 memset(&map,0, KEY_RANGE);
}

// destroy the cache:

void lruCacheFree() {
    memset(&map,0, KEY_RANGE);
    while(head->next)
      deletenode(head->next);
return;
}

// get the value for a specified key:
int key;
int get() {
   if (map[key]==NULL) return -1;
   int value=map[key]->val; 
   deletenode(map[key]);
   add(value,key);
   map[key]=tail;
   return value;
}

// set a value for a specified key :

void put(int value) {
 if (map[key]==NULL && size==maxsize)
    deletenode(head->next);	 
 if (map[key]!=NULL)
    deletenode(map[key]);
  add(value,key);
 return;
}

void producer(int connfd) {
  pthread_mutex_lock(&mutex);
  while (count > 0) 
    pthread_cond_wait(&empty, &mutex);
  put(connfd);
  pthread_cond_signal(&full);
  pthread_mutex_unlock(&mutex); 
}

void *consumer (void *arg) { 
  while (1) { 
  pthread_mutex_lock(&mutex);
  while (count == 0) 
    pthread_cond_wait(&full, &mutex);
  count++;
  int fd = get();
  pthread_cond_signal(&empty);
  count--;
  pthread_mutex_unlock(&mutex); 
  requestHandle(fd);
  Close(fd);
  } 
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    int threads, buffers;
    struct sockaddr_in clientaddr;

    getargs(&port, &threads, &buffers, argc, argv );
    
    int i;
    for (i = 0; i < threads; i++) {
      pthread_t worker;
      pthread_create(&worker, NULL, consumer, NULL); 
    }
    
    max = buffers;
    
    listenfd = Open_listenfd(port);
    lruCacheInit();
    while (1) {
	    clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
      	    producer(connfd);   
    }

lruCacheFree();
}
