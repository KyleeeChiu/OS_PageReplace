#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <queue>
#include <sstream>

using namespace std ;

#define MAX_LINE 1024
#define MAX_LEN 10

vector <int> process ;

struct Type {
  string page ;
  int count ;
};

struct Info {
  string page ;
  vector <Type> state ;
  bool fault ;       // true : is page fault
};

struct Page {
  int frame ;
  int replace ;
  int fault ;
  vector <Info> info ;
};

class Replacement {
  vector <Page> type ;
  int frame ;
  string pages ;
public:  
	bool Find ( string find, vector <Type> set, int & pos ) {
	  for ( int i = 0 ; i < set.size() ; i++ ) {
	    if ( find == set.at( i ).page ) {
	      pos = i ;
	      return true ;
      } // if
    } // for
    
    pos = -1 ;
    return false ;
  } // Find()
	
	string String( char ch ) {
	  string str;
    stringstream ss ;
    ss << ch ;
    ss >> str ; 
    return str ;
  } // String() 
	
	void FIFO() {
	  for ( int i = 0 ; i < pages.size() ; i++ ) {
	    Info info ;
	    Type temp ;
	    info.page = String( pages[i] ) ;
	    if ( i == 0 ) {
	      info.fault = true ;
	      type.at( 0 ).fault++ ;
	      temp.page = info.page ;
	      temp.count = 0 ;
	      info.state.push_back( temp ) ;
      } // if
      else {
        vector <Type> before = type.at( 0 ).info.at( i-1 ).state ;
        info.state.assign( before.begin(), before.end() ) ;
        
        int pos = -1 ;
        if ( Find( info.page, info.state, pos ) ) {
          info.fault = false ;
        } // if
        else {
          info.fault = true ;
          type.at( 0 ).fault++ ;
          if ( info.state.size() >= frame ) {
            type.at( 0 ).replace++ ;
            info.state.erase( info.state.begin() ) ;
          } // if
          
          temp.page = info.page ;
          temp.count = 0 ;
          info.state.push_back( temp ) ;
        } // else
      } // else
      
      type.at( 0 ).info.push_back( info ) ;
    } // for
  } // FIFO() ;
	
  void LRU() {
    for ( int i = 0 ; i < pages.size() ; i++ ) {
	    Info info ;
	    info.page = String( pages[i] ) ;
	    Type temp ;
	    if ( i == 0 ) {
	      info.fault = true ;
	      type.at( 1 ).fault++ ;
	      temp.page = info.page ;
	      temp.count = 0 ;
	      info.state.push_back( temp ) ;
      } // if
      else {
        vector <Type> before = type.at( 1 ).info.at( i-1 ).state ;
        info.state.assign( before.begin(), before.end() ) ;
        
        int pos = -1 ;
        if ( Find( info.page, info.state, pos ) ) {
          info.fault = false ;
          Type find = info.state.at( pos ) ;
          info.state.erase( info.state.begin()+pos ) ;
          info.state.push_back( find ) ;
        } // if
        else {
          info.fault = true ;
          type.at( 1 ).fault++ ;
          if ( info.state.size() >= frame ) {
            type.at( 1 ).replace++ ;
            info.state.erase( info.state.begin() ) ;
          } // if
          
          temp.page = info.page ;
          temp.count = 0 ;
          info.state.push_back( temp ) ;
        } // else
      } // else
      
      type.at( 1 ).info.push_back( info ) ;
    } // for 
  } // LRU()
  
  void LFU_FI() {
    for ( int i = 0 ; i < pages.size() ; i++ ) {
	    Info info ;
	    Type temp ;
	    info.page = String( pages[i] ) ;
	    if ( i == 0 ) {
	      info.fault = true ;
	      type.at( 2 ).fault++ ;
	      temp.page = info.page ;
	      temp.count = 0 ;
	      info.state.push_back( temp ) ;
      } // if
      else {       
        vector <Type> before = type.at( 2 ).info.at( i-1 ).state ;
        info.state.assign( before.begin(), before.end() ) ;
        
        int pos = -1 ;
        if ( Find( info.page, info.state, pos ) ) {
          info.fault = false ;
          info.state.at( pos ).count++ ;
        } // if
        else {
          info.fault = true ;
          type.at( 2 ).fault++ ;
          if ( info.state.size() >= frame ) {
            type.at( 2 ).replace++ ;
            int get = 0, smallest = 999 ;
            for ( int j = 0 ; j < info.state.size() ; j++ ) {
              if ( info.state.at( j ).count < smallest ) {
                get = j ;
                smallest = info.state.at( j ).count ;
              } // if 
            } // for
            info.state.erase( info.state.begin()+get ) ;
          } // if
          
          temp.page = info.page ;
          temp.count = 0 ;
          info.state.push_back( temp ) ;
        } // else
      } // else
      
      type.at( 2 ).info.push_back( info ) ;
    } // for
  } // LFU_FI()
  
  void MFU_FI() {
    for ( int i = 0 ; i < pages.size() ; i++ ) {
	    Info info ;
	    Type temp ;
	    info.page = String( pages[i] ) ;
	    if ( i == 0 ) {
	      info.fault = true ;
	      type.at( 3 ).fault++ ;
	      temp.page = info.page ;
	      temp.count = 0 ;
	      info.state.push_back( temp ) ;
      } // if
      else {       
        vector <Type> before = type.at( 3 ).info.at( i-1 ).state ;
        info.state.assign( before.begin(), before.end() ) ;
        
        int pos = -1 ;
        if ( Find( info.page, info.state, pos ) ) {
          info.fault = false ;
          info.state.at( pos ).count++ ;
        } // if
        else {
          info.fault = true ;
          type.at( 3 ).fault++ ;
          if ( info.state.size() >= frame ) {
            type.at( 3 ).replace++ ;
            int get = 0, biggest = -1 ;
            for ( int j = 0 ; j < info.state.size() ; j++ ) {
              if ( info.state.at( j ).count > biggest ) {
                get = j ;
                biggest = info.state.at( j ).count ;
              } // if 
            } // for
            info.state.erase( info.state.begin()+get ) ;
          } // if
          
          temp.page = info.page ;
          temp.count = 0 ;
          info.state.push_back( temp ) ;
        } // else
      } // else
      
      type.at( 3 ).info.push_back( info ) ;
    } // for
  } // MFU_FI()
  
  void LFU_LRU() {
    for ( int i = 0 ; i < pages.size() ; i++ ) {
	    Info info ;
	    Type temp ;
	    info.page = String( pages[i] ) ;
	    if ( i == 0 ) {
	      info.fault = true ;
	      type.at( 4 ).fault++ ;
	      temp.page = info.page ;
	      temp.count = 0 ;
	      info.state.push_back( temp ) ;
      } // if
      else {       
        vector <Type> before = type.at( 4 ).info.at( i-1 ).state ;
        info.state.assign( before.begin(), before.end() ) ;
        
        int pos = -1 ;
        if ( Find( info.page, info.state, pos ) ) {
          info.fault = false ;
          Type find = info.state.at( pos ) ;
          find.count++ ;
          info.state.erase( info.state.begin()+pos ) ;
          info.state.push_back( find ) ;
        } // if
        else {
          info.fault = true ;
          type.at( 4 ).fault++ ;
          if ( info.state.size() >= frame ) {
            type.at( 4 ).replace++ ;
            int get = 0, smallest = 999 ;
            for ( int j = 0 ; j < info.state.size() ; j++ ) {
              if ( info.state.at( j ).count < smallest ) {
                get = j ;
                smallest = info.state.at( j ).count ;
              } // if 
            } // for
            info.state.erase( info.state.begin()+get ) ;
          } // if
          
          temp.page = info.page ;
          temp.count = 0 ;
          info.state.push_back( temp ) ;
        } // else
      } // else
      
      type.at( 4 ).info.push_back( info ) ;
    } // for
  } // LFU_LRU()
  
  void MFU_LRU() {
    for ( int i = 0 ; i < pages.size() ; i++ ) {
	    Info info ;
	    Type temp ;
	    info.page = String( pages[i] ) ;
	    if ( i == 0 ) {
	      info.fault = true ;
	      type.at( 5 ).fault++ ;
	      temp.page = info.page ;
	      temp.count = 0 ;
	      info.state.push_back( temp ) ;
      } // if
      else {       
        vector <Type> before = type.at( 5 ).info.at( i-1 ).state ;
        info.state.assign( before.begin(), before.end() ) ;
        
        int pos = -1 ;
        if ( Find( info.page, info.state, pos ) ) {
          info.fault = false ;
          Type find = info.state.at( pos ) ;
          find.count++ ;
          info.state.erase( info.state.begin()+pos ) ;
          info.state.push_back( find ) ;
        } // if
        else {
          info.fault = true ;
          type.at( 5 ).fault++ ;
          if ( info.state.size() >= frame ) {
            type.at( 5 ).replace++ ;
            int get = 0, biggest = -1 ;
            for ( int j = 0 ; j < info.state.size() ; j++ ) {
              if ( info.state.at( j ).count > biggest ) {
                get = j ;
                biggest = info.state.at( j ).count ;
              } // if 
            } // for
            info.state.erase( info.state.begin()+get ) ;
          } // if
          
          temp.page = info.page ;
          temp.count = 0 ;
          info.state.push_back( temp ) ;
        } // else
      } // else
      
      type.at( 5 ).info.push_back( info ) ;
    } // for
  } // MFU_LRU()
  
	
public:
  bool readF( string fileName ) {
    FILE *infile = NULL ;							// a file handle	
		char * name = new char[MAX_LINE+1] ;
		fileName = fileName + ".txt" ;
		infile = fopen( fileName.c_str(), "r" ) ;		// open the file
		if ( infile == NULL ) {
			cout << endl << "### " << fileName << " does not exist! ###" << endl ;
			return false ;
		}
		else {
		  char * read = new char[MAX_LINE] ;
			fscanf( infile, "%d", &frame ) ;
			fscanf( infile, "%s", read ) ;
			pages.assign( read ) ;
    } // else
    
    fclose( infile ) ;
    return true ;
  } // readF()
  
  void make() {
    for ( int i = 0 ; i < 6 ; i++ ) {
      Page temp ;
      temp.frame = frame ;
      temp.fault = 0 ;
      temp.replace = 0 ;
      type.push_back( temp ) ;
    } // for
    
    FIFO() ;
    LRU() ;
    LFU_FI() ;
    MFU_FI() ;
    LFU_LRU() ;
    MFU_LRU() ;
  } // make()
  
  
  void writeF( string fileName ) {
  	FILE * outfile ;			
		outfile = fopen( fileName.c_str(), "w" ) ;
		for ( int i = 0 ; i < 6 ; i++ ) {
		  if ( i == 0 )
		    fprintf( outfile, "--------------FIFO-----------------------\n" ) ;
		  else if ( i == 1 )
		    fprintf( outfile, "\n--------------LRU-----------------------\n" ) ;
		  else if ( i == 2 )
		    fprintf( outfile, "\n--------------Least Frequently Used Page Replacement-----------------------\n" ) ;
		  else if ( i == 3 )
		    fprintf( outfile, "\n--------------Most Frequently Used Page Replacement -----------------------\n" ) ;
		  else if ( i == 4 )
		    fprintf( outfile, "--------------Least Frequently Used LRU Page Replacement-----------------------\n" ) ;
		  else if ( i == 5 )
		    fprintf( outfile, "\n--------------Most Frequently Used LRU Page Replacement -----------------------\n" ) ;
      
      for ( int j = 0 ; j < pages.size() ; j++ ) {
        Info temp = type.at( i ).info.at( j ) ;
        fprintf( outfile, "%s	", temp.page.c_str() ) ;
        for ( int k = temp.state.size()-1 ; k >= 0  ; k-- ) 
          fprintf( outfile, "%s", temp.state.at( k ).page.c_str() ) ;
        
        if ( temp.fault )
          fprintf( outfile, "	F" ) ;
        fprintf( outfile, "\n" ) ;
      } // for
      
      fprintf( outfile, "Page Fault = %d  Page Replaces = %d  ", type.at( i ).fault, type.at( i ).replace ) ;
      fprintf( outfile, "Page Frames = %d\n", frame ) ;
    } // for
    
		fclose( outfile ) ;
	} 
  
  void clearUp() {
    type.clear() ;
  }
};



int main( void ) {
	string command;
	Replacement replace ;
	do {
		string fileName ;
		cout << endl << "*** CPU Scheduling ***" ;
		cout << endl << "* 0. EXIT            *" ;
		cout << endl << "* 1. Read File       *" ;
		cout << endl << "**********************" ;
		cout << endl << "Input a choice(0, 1): " ;
		cin >> command ;
		cin.ignore( MAX_LEN, '\n' ) ;
		if ( command.compare( "1" ) == 0 ) {
			replace.clearUp() ;
			cout << endl << "Input a file name : " ;
			cin >> fileName ;
			if ( replace.readF( fileName ) ) {
				replace.make() ; 
				fileName = "out_"+ fileName + ".txt" ;
        replace.writeF( fileName ) ;					
			}
		}
		else if ( command.compare( "0" ) == 0 )	
			break ;
		else
			cout << endl << "Command does not exist!" << endl ;	
	} while ( true );
	return 0 ;
}
