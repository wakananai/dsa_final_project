#ifndef TRIE
#define TRIE
template<typename T,char L='0',char R='z'>
class trie{
    private:
        
    public:
	//
	struct node{
            node *tr[R-L+1],*pa;
            T data;
            int ref;
            bool is;
            node():ref(0),is(0){
                for(int i=0;i<=R-L;i++)tr[i]=NULL;
            }
            ~node(){
                for(int i=0;i<=R-L;i++){
                    if(tr[i])delete tr[i];
                }
            }
        }root;
        int _size;
	
	
	
	//
        class point_iterator{
            friend trie;
            private:
               
            public:
				node *p;//
                point_iterator(){p=NULL;}
                inline operator bool (){return p?1:0;}
                inline T &operator *(){return p->data;}
                inline void operator =(T d){p->data=d;}
				inline void operator =(node* ptr){p=ptr;}
        };
        inline void clear(){
            for(int i=0;i<=R-L;i++)if(root.tr[i]){
                delete root.tr[i],root.tr[i]=NULL;
            }
        }
        trie(){root.ref=0;clear();_size=0;}
        ~trie(){clear();}
        inline int size(){return _size;}
        inline point_iterator find(const char *s){
            point_iterator it;
            node *now=&root;
            for(int i=0;s[i];i++){
                if(!now->tr[s[i]-L])return it;
                now=now->tr[s[i]-L];
            }
            if(now->is)it.p=now;
            return it;
        }
        inline point_iterator insert(const char *s,T data){
            point_iterator it=find(s);
            if(it){it=data;return it;}
            node *now=&root;
            _size++;
            for(int i=0;s[i];i++){
                if(!now->tr[s[i]-L]){
                    now->tr[s[i]-L]=new node;
                    now->tr[s[i]-L]->pa=now;
                }
                now=now->tr[s[i]-L],now->ref++;
            }
            now->data=data;now->is=1;
            it.p=now;
            return it;
        }
        inline T &operator[](const char *s){
            point_iterator it=find(s);
            if(!it)it=insert(s,T());
            return it.p->data;
        }
        inline bool erase(const char *s){
            if(!find(s))return 0;
            node *now=&root;_size--;
            for(int i=0;s[i];i++){
                if(i&&!now->ref){
                    now->pa->tr[s[i-1]-L]=NULL;
                    delete now;return 1;
                }
                now=now->tr[s[i]-L],now->ref--;
            }
            now->is=0;
            return 1;
        }
};
#endif