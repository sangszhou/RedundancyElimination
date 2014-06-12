#include <iostream>
#include <map>
using namespace std;

class ListNode1 {
public:
	ListNode1(int _key, int _val) {
		key = _key;
		val = _val;
		pre = next = NULL;
	}

	ListNode1 *pre;
	int key;
	int val;
	ListNode1 *next;
};

class LRUCache{
public:
    LRUCache(int capacity) {
        this->capacity = capacity;
        head = NULL;
    	size = 0;
    }
    
    int get(int key) {
        if(mp.count(key) == 0)
        	return -1;
        int value = mp[key]->val;
        set(key, value);
        return value;
    }
    
    void set(int key, int value) {
        if(capacity == 1) {
        	mp.clear();
        	head = new ListNode1(key, value);
        	mp[key] = head;

        	return;
        }
        if(mp.count(key) > 0) {
        	ListNode1 *curNode = mp[key];
        	curNode->val = value;
        	
            if(curNode == head)
                return;

        	curNode->pre->next = curNode->next;
        	curNode->next->pre = curNode->pre;

        	head->pre->next = curNode;
        	curNode->pre = head->pre;
        	curNode->next = head;
        	head->pre = curNode;
        	head = curNode;
        	
        	return;
        }

        if(size < capacity) {
            
        	size ++;
        	ListNode1 *newNode = new ListNode1(key, value);
            mp[key] = newNode;
        	if(size == 1) {
        		newNode->pre = newNode;
        		newNode->next = newNode;
        		head = newNode;
        		return;
        	}
        	head->pre->next = newNode;
        	newNode->pre = head->pre;
        	newNode->next = head;
        	head->pre = newNode;
        	head = newNode;
        	return;
        }
        // size >= capacity
        ListNode1 *tail = head->pre;
        mp.erase(mp.find(tail->key));
        mp[key] = tail;
        tail->key = key;
        tail->val = value;
        head = tail;

    }
private:
	int capacity;
	int size;
	map<int, ListNode1*> mp;
	ListNode1 *head;
};
