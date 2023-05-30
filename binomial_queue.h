#ifndef BINOMIAL_QUEUE_H
#define BINOMIAL_QUEUE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include "dsexceptions.h"
#include "quadratic_probing.h"
using namespace std;

// Binomial queue class
//
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// deleteMin( )           --> Return and remove smallest item
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinomialQueue
{
  public:

    //insertNoMerge function:
    //-create a modified merge functions
    //where we added all the changes in textbook question 6.35
    //a) terminate merging if no tree left in H2, and carry=nullptr
    bool insertNoMerge(const Comparable & x)
    {
      //if position table already contains x:
        if(position_table_.Contains(x)) return false;
        //add x within position table and return true
        else
        {
          BinomialQueue oneItem{ x };
          position_table_.Insert(x, oneItem.theTrees[0]);
          ModifiedMerge(oneItem);
          return true;
        }
    }

    //Modified merge function for part 3:
    //a) terminate merging if no tree left in H2, and carry=nullptr
    //b)small always merge with large tree
    void ModifiedMerge(BinomialQueue & rhs)
    {
      if( this == &rhs )    // Avoid aliasing problems
            return;

        currentSize += rhs.currentSize;

        if( currentSize > capacity( ) )
        {
            int oldNumTrees = theTrees.size( );
            int newNumTrees = max( theTrees.size( ), rhs.theTrees.size( ) ) + 1;
            theTrees.resize( newNumTrees );
            for( int i = oldNumTrees; i < newNumTrees; ++i )
                theTrees[ i ] = nullptr;
        }

        BinomialNode *carry = nullptr;
        for( int i = 0, j = 1; j <= currentSize; ++i, j *= 2 )
        {
            BinomialNode *t1 = theTrees[ i ];
            BinomialNode *t2 = i < rhs.theTrees.size( ) ? rhs.theTrees[ i ] : nullptr;

            //6.35 part1:terminate merging if no tree left in H2, and carry=nullptr
            if(carry == nullptr && t2 == nullptr) break;

            int whichCase = t1 == nullptr ? 0 : 1;
            whichCase += t2 == nullptr ? 0 : 2;
            whichCase += carry == nullptr ? 0 : 4;

            switch( whichCase )
            { //remove some test cases to make more efficient
              case 2: /* Only rhs */
                theTrees[ i ] = t2;
                rhs.theTrees[ i ] = nullptr;
                break;
              case 4: /* Only carry */
                theTrees[ i ] = carry;
                carry = nullptr;
                break;
              case 3: /* this and rhs */
                carry = combineTrees( t1, t2 );
                theTrees[ i ] = rhs.theTrees[ i ] = nullptr;
                break;
              case 5: /* this and carry */
                carry = combineTrees( t1, carry );
                theTrees[ i ] = nullptr;
                break;
            }
        }

        for( auto & root : rhs.theTrees )
            root = nullptr;
        rhs.currentSize = 0;
    }//end modified merge

    //change (d)
    //return true if x is in position_table, else false
    bool Find(const Comparable &x)
    {
      if(position_table_.Contains(x)) return true;
      else return false; 
    }//end bool find

    //change (e)
    //check whether x in position_table_.
    //return false if item not in position_table_.
    //otherwise removes x from both hashtable and binomial queue
    //Note: removal involves use of parent pointers. removal may change values of some elements in ht
    //findmin function to find min table in hash table, 
    bool Remove(const Comparable & x)
    {
      //if x not in position table return false
      if(!position_table_.Contains(x)) return false; 
      position_table_.Remove(x);
      return true;
    }

    BinomialQueue( ) : theTrees( DEFAULT_TREES )
    {
        for( auto & root : theTrees )
            root = nullptr;
        currentSize = 0;
    }

    BinomialQueue( const Comparable & item ) : theTrees( 1 ), currentSize{ 1 }
      { theTrees[ 0 ] = new BinomialNode{ item, nullptr, nullptr }; }

    BinomialQueue( const BinomialQueue & rhs )
      : theTrees( rhs.theTrees.size( ) ),currentSize{ rhs.currentSize }
    { 
        for( int i = 0; i < rhs.theTrees.size( ); ++i )
            theTrees[ i ] = clone( rhs.theTrees[ i ] );
    }

    BinomialQueue( BinomialQueue && rhs )
      : theTrees{ std::move( rhs.theTrees ) }, currentSize{ rhs.currentSize }
    { 
    }

    ~BinomialQueue( )
      { makeEmpty( ); }

    
    /**
     * Deep copy.
     */
    BinomialQueue & operator=( const BinomialQueue & rhs )
    {
        BinomialQueue copy = rhs;
        std::swap( *this, copy );
        return *this;
    }
        
    /**
     * Move.
     */
    BinomialQueue & operator=( BinomialQueue && rhs )
    {
        std::swap( currentSize, rhs.currentSize );
        std::swap( theTrees, rhs.theTrees );
        
        return *this;
    }
    
    /**
     * Return true if empty; false otherwise.
     */
    bool isEmpty( ) const
      { return currentSize == 0; }

    /**
     * Returns minimum item.
     * Throws UnderflowException if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };

        return theTrees[ findMinIndex( ) ]->element;
    }
    
    /**
     * Change return type to bool: check
     * Search within position_table_ for x: 
     * If x is in the position_table_ already, return false:
     * Otherwise, x will be inserted and true will be returned.
     */
    //change (c)
    bool insert(const Comparable &x)
      {//start bool insert 
        //if position table already contains x:
        if(position_table_.Contains(x)) return false;
        //add x within position table and return true
        else
        {
          BinomialQueue oneItem{ x };
          position_table_.Insert(x, oneItem.theTrees[0]);
          merge(oneItem);
          return true;
        }
      }//end bool insert

    /**
     * Insert item x into the priority queue; allows duplicates.
     */
    void Insert( Comparable && x )
      { BinomialQueue oneItem{ std::move( x ) }; merge( oneItem ); }
    
    /**
     * Remove the smallest item from the priority queue.
     * Throws UnderflowException if empty.
     */
    void deleteMin( )
    {
        Comparable x;
        deleteMin( x );
    }

    /**
     * Remove the minimum item and place it in minItem.
     * Throws UnderflowException if empty.
     */
    void deleteMin( Comparable & minItem )
    {
        if( isEmpty( ) )
            throw UnderflowException{ };

        int minIndex = findMinIndex( );
        minItem = theTrees[ minIndex ]->element;

        BinomialNode *oldRoot = theTrees[ minIndex ];
        BinomialNode *deletedTree = oldRoot->leftChild;
        delete oldRoot;

        // Construct H''
        BinomialQueue deletedQueue;
        deletedQueue.theTrees.resize( minIndex );
        deletedQueue.currentSize = ( 1 << minIndex ) - 1;
        for( int j = minIndex - 1; j >= 0; --j )
        {
            deletedQueue.theTrees[ j ] = deletedTree;
            deletedTree = deletedTree->nextSibling;
            deletedQueue.theTrees[ j ]->nextSibling = nullptr;
        }

        // Construct H'
        theTrees[ minIndex ] = nullptr;
        currentSize -= deletedQueue.currentSize + 1;

        merge( deletedQueue );
    }

    /**
     * Make the priority queue logically empty.
     */
    void makeEmpty( )
    {
        currentSize = 0;
        for( auto & root : theTrees )
            makeEmpty( root );
    }

    /**
     * Merge rhs into the priority queue.
     * rhs becomes empty. rhs must be different from this.
     * Exercise 6.35 needed to make this operation more efficient.
     */
    void merge( BinomialQueue & rhs )
    {
        if( this == &rhs )    // Avoid aliasing problems
            return;

        currentSize += rhs.currentSize;

        if( currentSize > capacity( ) )
        {
            int oldNumTrees = theTrees.size( );
            int newNumTrees = max( theTrees.size( ), rhs.theTrees.size( ) ) + 1;
            theTrees.resize( newNumTrees );
            for( int i = oldNumTrees; i < newNumTrees; ++i )
                theTrees[ i ] = nullptr;
        }

        BinomialNode *carry = nullptr;
        for( int i = 0, j = 1; j <= currentSize; ++i, j *= 2 )
        {
            BinomialNode *t1 = theTrees[ i ];
            BinomialNode *t2 = i < rhs.theTrees.size( ) ? rhs.theTrees[ i ] : nullptr;

            int whichCase = t1 == nullptr ? 0 : 1;
            whichCase += t2 == nullptr ? 0 : 2;
            whichCase += carry == nullptr ? 0 : 4;

            switch( whichCase )
            {
              case 0: /* No trees */
              case 1: /* Only this */
                break;
              case 2: /* Only rhs */
                theTrees[ i ] = t2;
                rhs.theTrees[ i ] = nullptr;
                break;
              case 4: /* Only carry */
                theTrees[ i ] = carry;
                carry = nullptr;
                break;
              case 3: /* this and rhs */
                carry = combineTrees( t1, t2 );
                theTrees[ i ] = rhs.theTrees[ i ] = nullptr;
                break;
              case 5: /* this and carry */
                carry = combineTrees( t1, carry );
                theTrees[ i ] = nullptr;
                break;
              case 6: /* rhs and carry */
                carry = combineTrees( t2, carry );
                rhs.theTrees[ i ] = nullptr;
                break;
              case 7: /* All three */
                theTrees[ i ] = carry;
                carry = combineTrees( t1, t2 );
                rhs.theTrees[ i ] = nullptr;
                break;
            }
        }

        for( auto & root : rhs.theTrees )
            root = nullptr;
        rhs.currentSize = 0;
    }    


  private:
    struct BinomialNode
    {
        Comparable    element;
        BinomialNode *leftChild;
        BinomialNode *nextSibling;
        //change (b)
        BinomialNode *parent; 

        BinomialNode( const Comparable & e, BinomialNode *lt, BinomialNode *rt )
          : element{ e }, leftChild{ lt }, nextSibling{ rt } { }
        
        BinomialNode( Comparable && e, BinomialNode *lt, BinomialNode *rt )
          : element{ std::move( e ) }, leftChild{ lt }, nextSibling{ rt } { }
    };

    const static int DEFAULT_TREES = 1;
    //Change (a)
    HashTable<Comparable, BinomialNode*> position_table_;
    vector<BinomialNode *> theTrees;  // An array of tree roots
    int currentSize;                  // Number of items in the priority queue
    
    /**
     * Find index of tree containing the smallest item in the priority queue.
     * The priority queue must not be empty.
     * Return the index of tree containing the smallest item.
     */
    int findMinIndex( ) const
    {
        int i;
        int minIndex;

        for( i = 0; theTrees[ i ] == nullptr; ++i )
            ;

        for( minIndex = i; i < theTrees.size( ); ++i )
            if( theTrees[ i ] != nullptr &&
                theTrees[ i ]->element < theTrees[ minIndex ]->element )
                minIndex = i;

        return minIndex;
    }

    /**
     * Return the capacity.
     */
    int capacity( ) const
      { return ( 1 << theTrees.size( ) ) - 1; }

    /**
     * Return the result of merging equal-sized t1 and t2.
     */
    BinomialNode * combineTrees( BinomialNode *t1, BinomialNode *t2 )
    {
        if( t2->element < t1->element )
            return combineTrees( t2, t1 );
        t2->nextSibling = t1->leftChild;
        t1->leftChild = t2;
        //change (e): update parent pointer
        t2->parent = t1;
        t1->parent = nullptr;
        return t1;
    }

    /**
     * Make a binomial tree logically empty, and free memory.
     */
    void makeEmpty( BinomialNode * & t )
    {
        if( t != nullptr )
        {
            makeEmpty( t->leftChild );
            makeEmpty( t->nextSibling );
            delete t;
            t = nullptr;
        }
    }

    /**
     * Internal method to clone subtree.
     */
    BinomialNode * clone( BinomialNode * t ) const
    {
        if( t == nullptr )
            return nullptr;
        else
            return new BinomialNode{ t->element, clone( t->leftChild ), clone( t->nextSibling ) };
    }
};

#endif