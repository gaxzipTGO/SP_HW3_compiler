#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace std ;

class GetTokenMachine {
    protected: fstream fin ;
    protected: string token = "" ;
    protected: bool end = false ;
    protected: char nextChar = '\0' ; // used to cheak
    protected: char bufferDelimiter = '\0' ; // used to save delimiter

    public: GetTokenMachine(string file) {
        try {
            this->fin.open(file) ;
            if ( !fin ) {
                throw invalid_argument("the file not open")  ;
            }
        }
        catch ( exception &e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ; 
            throw invalid_argument(e.what())  ;
        }
    }

    protected: bool IsDelimiter(char ch) {
        if ( ch == '\"' || ch == '\'' || ch == '.' || ch == ',' || ch == '*' ||
            ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' ||
            ch == '^' || ch == '&' || ch == '(' || ch == ')' || ch == '[' ||
            ch == ']' || ch == '{' || ch == '}' || ch == '|' || ch == ';' ||
            ch == ':' || ch == ';' || ch == '/' || ch == '?' || ch == '<' || 
            ch == '=' || ch == '>' || ch == '+' || ch == '-' ) {
            return true ;
        }
        return false ;
    }

    protected: bool GetFileToken( fstream &file,string & token ) {
        // if read delimite we need to save the token and return
        bool isEmpty = false ;
        if ( bufferDelimiter != '\0' ) {
            token = token + string(1,bufferDelimiter) ;
            bufferDelimiter = '\0' ;
            return isEmpty ;
        } // this means our buffer have a delimiter we need to use it first 
        file.get(nextChar) ;
        while ( nextChar != '\t' &&nextChar !=' ' && nextChar != '\n' && ! IsDelimiter(nextChar) && ! file.eof() ) {
            token = token + string(1,nextChar) ;
            file.get(nextChar) ;
        }
        if ( IsDelimiter(nextChar) ) {
            bufferDelimiter = nextChar ;
        } // if nextChar is delimiter, it will leave while and go to here

        if ( ! file.eof() ) {
            return isEmpty  ;
        }
        else return true ;

    }

    public: void Test() {
        char continueTime = 'y' ;
        while ( continueTime == 'y' && ! end ) {
            end = GetFileToken(fin,this->token) ;
            if ( token.length() > 0 ) {
                cout << token << endl ;
                token = "" ;             
            }
            //cout << "do yo countinue?" << endl ;
            //cin >> continueTime ; 
        } 
        cin >> token ;
    }

    public: bool GetNextToken(string &Out_token) {
        /*
        the function is we can get the token but we need to chiose Out_token != "" 
        */
        token = "" ;
        if ( ! end ) {
            do {
                end = GetFileToken(fin,token) ;
                if( end == true ) {
                    Out_token = token ; 
                    return false ;
                }         
            } while ( token.length() == 0 && nextChar != '\n') ;
            Out_token = token ;
            return true ;
        }
        return false ;         
    }

    public: char ReturnNextChar() {
        return nextChar ;
    }

    public: bool IsEnterChar() {
        if( nextChar == '\n' || end ) { 
            return true ;
        }
        else return false ;
    }

} ;

/*
用法為:
    string fileName ;
    cin >> fileName ;
    string token ;
    bool end ;
    GetTokenMachine machine1( fileName ) ;
    do {
        end = machine1.GetNextToken(token) ; // end is a bool means the page is EOF if end is false
        if ( token != "" ) {
            // here we can put some choise to get token like's string
            cout << token << ' ';
        } // the compare is very important!!! that can cheak the token does read something   
        if ( machine1.IsEnterChar() ) {
            cout << '\n' ;
        }     
    } while ( end ) ;   // this while is we can loading all token of page
*/

class Table {
    protected: int tokenPage = 0 ;
    protected: string token[100] ;
    protected: int tokenAmount = 0 ;

    public: Table( int tokenPage ) {
        this->tokenPage = tokenPage ;
    }

    public: int GetTablePage() {
        return this->tokenPage ;        
    }

    public: void PushNewToken( string token ) {
        this->token[tokenAmount] = token ;
        tokenAmount ++ ;
    }

    public: void PrintAllToken(){
        try {
            for ( int i = 0 ; i < tokenAmount ; i ++ ) {
                cout << this->token[i] << endl ;
            }
        }
        catch ( exception &e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;
            throw invalid_argument( e.what() ) ;
        }
    }

    public: void SaveOnePageTokenFormTable( string fileName ) {
        try {
            string token ;
            bool end ;
            GetTokenMachine machine1( fileName ) ;
            do {
                end = machine1.GetNextToken( token ) ; // end is a bool means the page is EOF if end is false
                if ( token != "" ) {
                    // here we can put some choise to get token like's string
                    this->PushNewToken( token ) ;
                } // the compare is very important!!! that can cheak the token does read something    
            } while ( end ) ;   // this while is we can loading all token of page
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }

    }

} ;

struct Line_of_Token {
    vector<string> token_of_oneLine ;
} ;

class Program {

    vector<Line_of_Token> oneLine_of_Program ;

    public: void SaveProgramFormInput_txt( string fileName ) {
        try {
            string token ;
            bool end ;
            Line_of_Token tempToken_of_line ;
            GetTokenMachine machine1( fileName ) ;
            do {
                end = machine1.GetNextToken( token ) ; // end is a bool means the page is EOF if end is false
                if ( token != "" ) {
                    // here we can put some choise to get token like's string
                    cout << token << " " ;
                    tempToken_of_line.token_of_oneLine.push_back(token) ;
                } // the compare is very important!!! that can cheak the token does read something
                if ( machine1.IsEnterChar() ) {

                    oneLine_of_Program.push_back( tempToken_of_line ) ;
                    tempToken_of_line.token_of_oneLine.clear() ;
                    cout << '\n' ;
                }    
            } while ( end ) ;   // this while is we can loading all token of page
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }        
    } 

} ;
struct Error {
    int line = 0 ;
    string errorMessage = "";
} ;

class SyntaxAnalysis {
    vector<Error> error ;
    bool SyntaxAnalysisThisLine( auto program ) {
        return true ;
    }

} ;

int main(int argc,char **argv) {
    try {
        Table delimiter_table( 1 ) ;
        Table command_table( 2 ) ;
        delimiter_table.SaveOnePageTokenFormTable("Table1.txt") ;
        command_table.SaveOnePageTokenFormTable( "Table2.txt" ) ;
        cout << "this is from table : " << delimiter_table.GetTablePage() << endl << endl ;
        delimiter_table.PrintAllToken() ;
        cout << "this is from table : " << command_table.GetTablePage() << endl << endl ;
        command_table.PrintAllToken() ;
        Program program ;
        program.SaveProgramFormInput_txt("e1.txt") ;

    }
    catch ( exception& e ) {
        cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
    }
}