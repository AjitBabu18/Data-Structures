#!/usr/bin/env python3



class ABNode:
    """Single node in an ABTree.

    Each node contains keys and children
    (with one more children than there are keys).
    We also store a pointer to node's parent (None for root).
    """
    def __init__(self, keys = None, children = None, parent = None):
        self.keys = keys if keys is not None else []
        self.children = children if children is not None else []
        self.parent = parent

    def find_branch(self, key):
        """ Try finding given key in this node.

        If this node contains the given key, returns (True, key_position).
        If not, returns (False, first_position_with_key_greater_than_the_given).
        """
        i = 0
        while (i < len(self.keys) and self.keys[i] < key):
            i += 1

        return (i < len(self.keys) and self.keys[i] == key, i)

    def insert_branch(self, i, key, child):
        """ Insert a new key and a given child between keys i and i+1."""
        self.keys.insert(i, key)
        self.children.insert(i + 1, child)

class ABTree:
    """A class representing the whole ABTree."""
    def __init__(self, a, b):
        assert a >= 2 and b >= 2 * a - 1, "Invalid values of a, b: {}, {}".format(a, b)
        self.a = a
        self.b = b
        self.root = ABNode(children=[None])

    def find(self, key):
        """Find a key in the tree.

        Returns True if the key is present, False otherwise.
        """
        node = self.root
        while node:
            found, i = node.find_branch(key)
            if found: return True
            node = node.children[i]
        return False

    def split_node(self, node, size):
        """Helper function for insert

        Split node into two nodes such that original node contains first _size_ children.
        Return new node and the key separating nodes.
       
        """
        size=int(size)
        new= ABNode(keys=node.keys[size+1:], parent=node.parent)
        
        new.children=node.children[size+1:]
        new.keys=node.keys[size+1:]
        key=node.keys[size]
        node.children=node.children[:size+1]
        node.keys=node.keys[:size]
        if (new.children[0]!=None):
            for child in new.children:
                child.parent=new
        return new,key
        # TODO: Implement and use in insert method
        raise NotImplementedError

    def insert(self, key):
        """Add a given key to the tree, unless already present."""
        # TODO: Implement
        
        node= self.root
        
        if self.find(key):
            return
        else:
            found,i=node.find_branch(key)
            #handling root
            if ((not found) and i==0 and len(node.keys)==0):
                self.root=ABNode(keys=[key], children=[None,None])
                return

        #if(found)
        # Traversing to the desired node
        while(node !=None and node.children[i]!= None):
            if (not found and len(node.children)>=i):
                node=node.children[i]
                found,i=node.find_branch(key)
                
        node.insert_branch(i,key,None) 
        #length=len(node.keys)
        
        #Splitting and propagating above
        while (len(node.keys)>=self.b):
            new,key=self.split_node(node,(self.b-1)/2)

            #Handling the root case
            if(node==self.root):
                new_root=ABNode(keys=[key], children=[node,new])
                #new_root.children.append(node)
                node.parent=new_root
                #new_root.children.append(new)
                #new_root.keys.append(key)
                new.parent=new_root
                self.root= new_root
                return

            else:
                parent= node.parent
                found,i=parent.find_branch(key)
                parent.insert_branch(i,key,new)
                node=parent
                #length=len(node.keys)
        return
        #raise NotImplementedError
