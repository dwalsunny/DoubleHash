/*****************************************
 * UW User ID:  uwuserid
 * Submitted for ECE 250
 * Semester of Submission:  (Winter|Spring|Fall) 20NN
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "Exception.h"
#include "ece250.h"

enum state { EMPTY, OCCUPIED, DELETED };

template <typename Type>
class Double_hash_table {
	private:
		int count;
		int power;
		int array_size;
		int mask;
		Type *array;
		state *occupied;

		int h1( Type const & ) const;
		int h2( Type const & ) const;

	public:
		Double_hash_table( int = 5 );
		~Double_hash_table();
		int size() const;
		int capacity() const;
		double load_factor() const;
		bool empty() const;
		bool member( Type const & ) const;
		Type bin( int ) const;

		void print() const;

		void insert( Type const & );
		bool erase( Type const & );
		void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Double_hash_table<T> const & );
};

// Constructor for the hash_table, initally sets each entry in the occupied array to
// empty. Also initalizes member variables
template <typename Type>
Double_hash_table<Type>::Double_hash_table( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
mask( array_size - 1 ),
array( new Type[array_size] ),
occupied( new state[array_size] ) {
    
	for ( int i = 0; i < array_size; ++i ) {
		occupied[i] = EMPTY;
	}
}


//Destructor, deletes the array holding the hash values along with the array
// that contains information if the bin is occupied, empty, or has a deleted value
template <typename Type>
Double_hash_table<Type>::~Double_hash_table<Type>()
{
    delete [] array;
    delete [] occupied;
}

//Hash function that determines bin number
// Bin value determined by modding the value being placed by the hash_table max capacity
template <typename Type >
int Double_hash_table<Type>::h1(Type const &obj) const
{
    int val = static_cast<int>(obj);
    int bin = val % capacity();
    
    while(bin < 0)
    {
        bin = bin + capacity();
    }
    
    return bin;
}

// Second hash function determines the jump value
// Jump value determined by dividing the value being placed in the hash_table by the
// max capacity and then being modded by the max capacity.
template <typename Type >
int Double_hash_table<Type>::h2(Type const &obj) const
{
    int val = static_cast<int>(obj);
    int jump =  (val/capacity()) % capacity();
    
    while(jump < 0)
    {
        jump = jump + capacity();
    }
    
    if((jump % 2) == 0)
    {
        ++jump;
    }
    
    return jump;
}

// Returns the number of occupied bins in the hash table
template <typename Type >
int Double_hash_table<Type>::size() const
{
    return count;
}

// Returns the max capacity of the hash table
template <typename Type >
int Double_hash_table<Type>::capacity() const
{
    return array_size;
}

// Returns the load factor, which is essentially the number of occupied bins
// divided by the max capacity
template <typename Type >
double Double_hash_table<Type>::load_factor() const
{
    return static_cast<double>(size()/capacity());
}

// Returns true/false depending on the number of occupied bins in the hash table
// if 0 items in hash table then its empty; else false
template <typename Type >
bool Double_hash_table<Type>::empty() const
{
    return (size() == 0);
}

// Returns true/false depending on if the value being passed in is in the hash table
// Initially calculates bin and jump value, then checks each bin the value should be in for by incrementing the jump value to the bin. At max it will have to iterate through the array, but shouldn't in most cases
template <typename Type >
bool Double_hash_table<Type>::member(Type const &obj) const
{
    int bin = h1(obj);
    int jump = h2(obj);
    
    for(int i = 0; i < capacity(); ++i)
    {
        if(array[bin] == obj && occupied[bin] == OCCUPIED)
        {
            return true;
        }
        
        bin = bin + jump;
        bin = bin % array_size;
    }
    return false;
}

// Returns the value in a certain bin
template <typename Type >
Type Double_hash_table<Type>::bin(int n) const
{
    return array[n];
}

// Inserts a value into the hash_table as long as its not full
// Similar to member, calculates initial bin and jump values and checks each bin spot
// the value can go in and places the value in that bin as long as its empty or deleted.
template <typename Type >
void Double_hash_table<Type>::insert( Type const &obj)
{
    if(size() == capacity())
    {
        throw overflow();
    }
    
    int bin = h1(obj);
    int jump = h2(obj);
    
    for(int i = 0; i < capacity(); ++i)
    {
        if(array[bin] == obj && occupied[bin] == OCCUPIED)
        {
            break;
        }

        else if(occupied[bin] == EMPTY || occupied[bin] == DELETED)
        {
            ++count;
            array[bin] = obj;
            occupied[bin] = OCCUPIED;
            break;
        }
        
        bin = bin + jump;
        bin = bin % capacity();
    }
}

// Returns true/false depending if the value being erased is actually removed from the
// hash table. Initally calculates bin and jump value of the item, iterates the array
//checking the bin values it should be in by incrementing the jump value to it. If the
// value is found I set that spot in the hash table to deleted and finish.

template <typename Type >
bool Double_hash_table<Type>::erase(Type const &obj)
{
    int bin = h1(obj);
    int jump = h2(obj);
    
    for(int i = 0; i < capacity(); ++i)
    {
        if(array[bin] == obj && occupied[bin] == OCCUPIED)
        {
            --count;
            occupied[bin] = DELETED;
            return true;
        }
        
        bin = bin + jump;
        bin = bin % capacity();
    }
    
    return false;
}

// Iterates through the hash table and sets each bin to empty
template <typename Type >
void Double_hash_table<Type>::clear()
{
    for(int i = 0; i < capacity(); ++i)
    {
        occupied[i] = EMPTY;
    }
    count = 0;
}



// Your implementation here
template <typename T>
std::ostream &operator<<( std::ostream &out, Double_hash_table<T> const &hash ) {
	for ( int i = 0; i < hash.capacity(); ++i ) {
		if ( hash.occupied[i] == EMPTY ) {
			out << "- ";
		} else if ( hash.occupied[i] == DELETED ) {
			out << "x ";
		} else {
			out << hash.array[i] << ' ';
		}
	}

	return out;
}

#endif
