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
        if ( ch == '\"' || ch == '\'' || /*ch == '.' ||*/ ch == ',' || ch == '*' ||
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
    } // constructor the class, give it table number

    public: int GetTablePage() {
        return this->tokenPage ;        
    } 

    public: string* GetToken() {
        return this->token ;
    }

    public: void PushNewToken( string token ) {
        try {
            this->token[tokenAmount] = token ;
            tokenAmount ++ ;
        }
        catch ( exception &e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;
            throw invalid_argument( e.what() ) ;
        }
    } // push a new token to this table 

    public: void PushNewTokenByIndex( string token, int &index ) {
        try {
            while ( this->token[index] != "" ) {
                index ++ ;
                if ( index == 100 ) {
                    index = 0 ;
                }
            }
            this->token[index] = token ;
        }
        catch ( exception &e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;
            throw invalid_argument( e.what() ) ;
        }        
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
    } // print everyToken in this table

    public: string GetIndex( int i ) {
        try {
            return token[i] ;
        }
        catch ( exception &e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;
            throw invalid_argument( e.what() ) ;
        }
        
    } // according the index return token

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

    } // accordding the fileName to read total Token, every token will stand one array

    public: int GetTokenIndex( string token ) {
        for ( int i = 0 ; i < 100 ; i ++ ) {
            if ( token == this->token[i] ) {
                return i ;
            }
        }
        return -1 ;
    }


} ;

class Table_Five : public Table{
    protected: int subroutine[100] ;
    protected: int type[100] ;
    protected: int pointer[100] ;

    public: Table_Five(int tokenPage) : Table(tokenPage){
      
    }

    public: void SetType( int type, int position ) {
        this->type[position] = type ;
    }

    public: void SetPointer( int pointer, int position ) {
        this->pointer[position] = pointer ;
    }

};


Table delimiter_table( 1 ) ;
Table command_table( 2 ) ;
Table integer_table( 3 ) ;
Table realnumber_table( 4 ) ;
Table_Five identifier_table( 5 ) ;

class Token{
    /*
        一個token 可以根據string知道自己的位置並且儲存下來
    */
    protected: Table* table ;
    protected: int page_of_table ;
    protected: int number_of_table ;

    protected: bool IsDelimiter(string str) {
        if ( str == "\'" || str == "\"" || /*str == "." ||*/ str == "," || str == "*" ||
            str == "!" || str == "@" || str == "#" || str == "$" || str == "%" ||
            str == "^" || str == "&" || str == "(" || str == ")" || str == "[" ||
            str == "]" || str == "{" || str == "}" || str == "|" || str == ";" ||
            str == ":" || str == ";" || str == "/" || str == "?" || str == "<" || 
            str == "=" || str == ">" || str == "+" || str == "-" ) {
            return true ;
        }
        return false ;
    }

    protected: bool IsIntegerOrRealNumber( string str ) {
        try{
            stoi(str,nullptr,10) ;
            for ( char ch : str ) {
                if ( ch == '.' ) {
                    this->page_of_table = 4 ;
                    this->table = &realnumber_table ;
                    return true ;
                }
            }
            this->page_of_table = 3 ;
            this->table = &integer_table ;
            return true ;
        }
        catch(exception &e) {
            return false ;
        }
    }
  
    protected: bool IsThisTable( string token, Table table ) {
        for ( int i = 0 ; i < 100 ; i ++ ) {
            if ( token == table.GetIndex(i) ) {
                return true ;
            }
        }
        return false ;
    }

    public: Token( string token ) {
        if ( IsDelimiter(token) ) {
            this->page_of_table = 1 ;
            this->table = &delimiter_table ;
        }
        else if ( IsThisTable(token,command_table) ) {
            this->page_of_table = 2 ;
            this->table = &command_table ;
        }
        else if( ! IsIntegerOrRealNumber( token ) ) {
            this->page_of_table = 5 ;
            this->table = &identifier_table ;
        }

    }

    public: Token() {
        this->page_of_table = 0 ;
        this->number_of_table = 0 ;
    } 



    public: void FindTokenPosition( string token_string ) {
        try {
            int position =  this->table->GetTokenIndex( token_string ) ;
            if ( position >= 0 ) {
                number_of_table = position ;
            }
            else {
                int ascii = 0 ;
                for ( char ch : token_string ) {
                    ascii = ascii +ch ;
                }
                int hash_key = ascii % 100 ;
                table->PushNewTokenByIndex(token_string, hash_key) ;
                number_of_table = hash_key ;
            }
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    public: int GetPage() {
        try {
            return this->page_of_table ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    public: int GetNumber() {
        try {
            return this->number_of_table ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    public: Table* GetTable() {
        try {
            return table ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    public: Table_Five* GetTable_Five() {
        try{
            return (Table_Five*) table ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    public: void PrintTable() {
        cout << "( " << GetPage() << ", " << GetNumber() << " )" ;
    }
} ;

class Token_Six {
    private: Token* token[4] ;
    private: int page ;
    private: int count ;    
    public: Token_Six() {
        page = 6 ;
        count = 0 ;
        for ( int i = 0 ; i <= 3 ; i ++ ) {
            token[i] = nullptr ;
        }
    }

    public: void push_Token(Token* token) {
        this->token[count] = token ;
        count ++ ;
    }

    public: void PrintAllToken() {
        cout << "( " ;
        for ( int i = 0 ; i < 3 ; i ++ ) {
            if ( token[i] != nullptr ) {
                token[i]->PrintTable() ;
            }
            cout << ",  " ;
        }
        if ( token[3] != nullptr ) token[3]->PrintTable() ;
        cout << " )" ;
    }
} ;

class Table_Six {
    private: vector<Token_Six> opcode ; 

    public: void push_Table( Token_Six token_six ) {
        opcode.push_back( token_six ) ;
    }

    public: void insert_begin_Table( Token_Six token_six ) {
        opcode.insert( opcode.begin(),token_six ) ;
    }

    public: void PrintAllOPCode() {
        for ( auto token : opcode ) {
            token.PrintAllToken() ;
            cout << endl ;
        }
    }

    public: void Clear() {
        this->opcode.clear() ;
    }

} opcode_table ;

class Line_of_Token {
    public: vector<Token> token_of_oneLine ;
    private: int syntaxError = 0 ;

    public: void PrintThisLine() {
        for ( Token token : token_of_oneLine ) {
            cout << token.GetTable()->GetIndex(token.GetNumber()) << ", " ; 
        }
        cout << endl ;
    }

    public: void clear() {
        token_of_oneLine.clear() ;
    }

    public: int size() {
        return token_of_oneLine.size() ;
    }

    public: Token GetTokenByIndex( int i ) {
        return token_of_oneLine[i] ;
    }

    public: Token* Get_Token_OF_OneLine_Pointer(int i) {
        return &token_of_oneLine[i] ;
    } // 這樣可以直接用pointer來接 recursive比較方便
} ;

struct Error {
    int line = 0 ;
    string errorMessage = "";
} ;

class SyntaxAnalysis {

    private: bool cheakIdentifier(Token* cheaked_statement_ptr) {
        try {
            if ( cheaked_statement_ptr->GetTable() == &identifier_table ) {
                return true ;
            }
            else {
                return false ;
            }
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    private: bool cheakComment(Token* cheaked_statement_ptr) {
        try {
            if ( cheaked_statement_ptr->GetPage() == 1 && cheaked_statement_ptr->GetNumber() == 1 ) {
                return true ;
            }     
            else {
                return false ;
            }
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }   
    }

    private: bool cheakProgram(Token* cheaked_statement_ptr) {
        try {
            if ( cheakIdentifier(cheaked_statement_ptr) ) {
                if ( cheakComment( cheaked_statement_ptr+1 ) ) {
                    return true ;
                }
            }
            return false ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    private: int GetDataType( Token* cheaked_statement_ptr ) {
        try {
            if ( cheaked_statement_ptr->GetPage() == 2 ) {
                int token_number = cheaked_statement_ptr->GetNumber() ;
                if ( token_number == 26 ) {
                    return 1 ;
                }
                else if ( token_number == 2 ) {
                    return 2 ;
                }
                else if ( token_number == 27 ) {
                    return 3 ;
                }
                else if ( token_number == 14 ) {
                    return 4 ;
                }
                else if ( token_number == 15 ) {
                    return 5 ;
                }
                else if ( token_number == 22 ) {
                    return 6 ;
                }
                else return 0 ;
            }
            return 0 ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    private: bool cheakComma( Token* cheaked_statement_ptr ) {
        try {
            if ( cheaked_statement_ptr->GetPage() == 1 && cheaked_statement_ptr->GetNumber() == 11 ) {
                return true ;
            }
            return false ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;            
        }
    }

    private: bool cheakColon( Token* cheaked_statement_ptr ) {
        try {
           if ( cheaked_statement_ptr->GetPage() == 1  && cheaked_statement_ptr->GetNumber() == 12 ) {
            return true ;
           }
           return false ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    private: bool cheakLeftBrack( Token* cheaked_statement_ptr ) {
        try{
            if ( cheaked_statement_ptr->GetPage() == 1 && cheaked_statement_ptr->GetNumber() == 2 ) {
                return true ;
            }
            return false ;
        }
        catch( exception& e ) {
            cerr << e.what() << " , in line : " << __LINE__ << endl ;
            throw e.what() ;
        }
    }


    private: bool cheakRightBrack( Token* cheaked_statement_ptr ) {
        try{
            if ( cheaked_statement_ptr->GetPage() == 1 && cheaked_statement_ptr->GetNumber() == 3 ) {
                return true ;
            }
            return false ;
        }
        catch( exception& e ) {
            cerr << e.what() << " , in line : " << __LINE__ << endl ;
            throw e.what() ;
        }
    }

    private: bool cheakNumber( Token* cheaked_statement_ptr  ) {
        try{
            if ( cheaked_statement_ptr->GetPage() == 3 ) {
                return true ;
            }
            return false ;
        }
        catch( exception& e ) {
            cerr << e.what() << " , in line : " << __LINE__ << endl ;
            throw e.what() ;
        }
    }

    private: bool cheakVariableName( Token* cheaked_statement_ptr, int dataType ) {
        try {
            if ( cheakIdentifier( cheaked_statement_ptr ) ) {
                Token* temp_ptr = cheaked_statement_ptr ;
                cheaked_statement_ptr ++ ;
                if ( ! cheakComment(cheaked_statement_ptr) ) {
                    if ( cheakComma(cheaked_statement_ptr) ) {
                        cheaked_statement_ptr ++ ;
                        if ( cheakVariableName(cheaked_statement_ptr, dataType) ) {
                            temp_ptr->GetTable_Five()->SetType(dataType,temp_ptr->GetNumber()) ;
                            Token_Six token_six = Token_Six() ;
                            token_six.push_Token(temp_ptr) ;
                            opcode_table.insert_begin_Table(token_six) ;
                            return true ;
                        }
                        else {
                            return false ;
                        }
                    }
                    else {
                        return false ;
                    }
                }
                else {
                        temp_ptr->GetTable_Five()->SetType(dataType,temp_ptr->GetNumber()) ;
                        Token_Six token_six = Token_Six() ;
                        token_six.push_Token(temp_ptr) ;
                        opcode_table.insert_begin_Table(token_six) ;                    
                    return true ;
                }
                 
            }
            return false ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " , in line : " << __LINE__ << endl ;
            throw e.what() ;
        }
    }

    private: bool cheakVariable(Token* cheaked_statement_ptr) {
        try {
            int dataType = GetDataType(cheaked_statement_ptr) ;
            if ( dataType != 0 ) {
                cheaked_statement_ptr ++ ;
                if ( cheakColon(cheaked_statement_ptr) ) {
                    cheaked_statement_ptr ++ ;
                    return cheakVariableName( cheaked_statement_ptr, dataType ) ;
                }
            }
            return false ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }
    }

    private: bool cheakNumberName( Token* &cheaked_statement_ptr ) {
        Token* head_ptr = cheaked_statement_ptr ;
        if ( cheakNumber(cheaked_statement_ptr) ) {
            cheaked_statement_ptr ++ ;
            if ( ! cheakRightBrack( cheaked_statement_ptr ) ) {
                if ( cheakComma( cheaked_statement_ptr ) ) {
                    cheaked_statement_ptr ++ ;
                    return cheakNumberName(cheaked_statement_ptr) ;
                }
                else {
                    cheaked_statement_ptr = head_ptr ;
                    return false ;
                }
            }
            return true ;
        }
        cheaked_statement_ptr = head_ptr ;
        return false ;
    }

    private: bool cheakArrayName( Token* &cheaked_statement_ptr, int dataType ) {
        Token* head_ptr = cheaked_statement_ptr ;
        if ( cheakIdentifier(cheaked_statement_ptr) ) {
            Token* temp_ptr = cheaked_statement_ptr ;
            cheaked_statement_ptr ++ ;
            if ( cheakLeftBrack(cheaked_statement_ptr) ) {
                cheaked_statement_ptr ++ ;
                if (cheakNumberName( cheaked_statement_ptr ) ) {
                    return true ;
                }
                else {
                    cheaked_statement_ptr = head_ptr ;
                    return false ;
                }
            }
        }
        cheaked_statement_ptr = head_ptr ;
        return false ;
    }

    private: bool cheakArray( Token* cheaked_statement_ptr, int dataType ) {
        try {
            Token* temp_ptr = cheaked_statement_ptr ;
            if ( cheakArrayName(cheaked_statement_ptr,dataType) ) {
                cheaked_statement_ptr ++ ;
                if ( !cheakComment(cheaked_statement_ptr) ) {
                    if ( cheakComma(cheaked_statement_ptr) ) {
                        cheaked_statement_ptr ++ ;
                        if ( cheakArray( cheaked_statement_ptr,dataType ) ){
                            temp_ptr->GetTable_Five()->SetType(5,temp_ptr->GetNumber()) ;
                            Token_Six token_six = Token_Six() ;
                            token_six.push_Token(temp_ptr) ;
                            opcode_table.insert_begin_Table(token_six) ;
                            return true ;
                        }
                    }
                    else {
                        return false ;
                    }
                }
                else {
                    temp_ptr->GetTable_Five()->SetType(5,temp_ptr->GetNumber()) ;
                    Token_Six token_six = Token_Six();
                    token_six.push_Token(temp_ptr) ;
                    opcode_table.insert_begin_Table(token_six) ;
                    return true ;
                }
            }
            return false ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }          
    }

    private: bool cheakDimsion(Token* cheaked_statement_ptr) {
        try {
            int dataType = GetDataType( cheaked_statement_ptr ) ;
            if ( dataType != 0 ) {
                cheaked_statement_ptr ++ ;
                if ( cheakColon(cheaked_statement_ptr) ) {
                    cheaked_statement_ptr ++ ;
                    return cheakArray( cheaked_statement_ptr, dataType ) ;
                }
            }
            return false ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }        
    }

    public: bool cheakLabel( Token* cheaked_statement_ptr ) {
        try {
            Token* temp_ptr = cheaked_statement_ptr ;
            if ( cheakIdentifier(cheaked_statement_ptr) ) {
                cheaked_statement_ptr ++ ;
                if ( ! cheakComment( cheaked_statement_ptr ) ) {
                    if ( cheakComma( cheaked_statement_ptr ) ) {
                        if ( cheakLabel(cheaked_statement_ptr+1) ) {
                            Token_Six token_six = Token_Six();
                            token_six.push_Token(temp_ptr) ;
                            opcode_table.insert_begin_Table(token_six) ;  
                            return true ;                          
                        }
                    }
                }
                else {
                    Token_Six token_six = Token_Six();
                    token_six.push_Token(temp_ptr) ;
                    opcode_table.insert_begin_Table(token_six) ;
                    return true ;                    
                }
            }
            return false ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }         
    }

    public: bool cheakGTO( Token* cheaked_statement_ptr ) {
        try {
            if ( cheakIdentifier(cheaked_statement_ptr) ) {
                return true ;
            }
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }        
    }

    public: bool SyntaxAnalysisStatement( Line_of_Token &statement ) {

        try {
            Token* line_token_ptr = statement.Get_Token_OF_OneLine_Pointer(0) ;
            if ( line_token_ptr->GetPage() == 2  && line_token_ptr->GetNumber() == 21 ) {
                return cheakProgram(line_token_ptr+1) ;
            }
            else if ( line_token_ptr->GetPage() == 2  && line_token_ptr->GetNumber() == 25 ) {
                return cheakVariable(line_token_ptr+1) ;  
            }
            else if ( line_token_ptr->GetPage() == 2  && line_token_ptr->GetNumber() == 4 ) {
                return cheakDimsion(line_token_ptr+1) ;
            }
            else if ( line_token_ptr->GetPage() == 2 && line_token_ptr->GetNumber() == 15 ) {
                return cheakLabel(line_token_ptr+1) ;
            }
            else if ( line_token_ptr->GetPage() == 2 && line_token_ptr->GetNumber() == 11 ) {
                return cheakGTO(line_token_ptr+1) ;
            }
            else return false ;
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        } 
    }

} syntaxAnalysis ;

class Program {
    private: vector<Line_of_Token> oneLine_of_Program ;

    public: void SaveProgramFormInput_txt( string fileName ) {
        try {
            string token_string ;
            bool end ;
            Line_of_Token tempToken_of_line ;
            Line_of_Token tempToken_of_statement ;
            GetTokenMachine machine1( fileName ) ;
            do {
                end = machine1.GetNextToken( token_string ) ; // end is a bool means the page is EOF if end is false
                if ( token_string != "" ) {
                    // here we can put some choise to get token like's string
                    cout << token_string << ":=" ;
                    Token token = Token( token_string ) ;
                    token.FindTokenPosition(token_string) ;
                    cout << "(" << token.GetPage() << ", " << token.GetNumber() << ") " ;
                    tempToken_of_line.token_of_oneLine.push_back(token) ;
                    tempToken_of_statement.token_of_oneLine.push_back(token) ;
                } // the compare is very important!!! that can cheak the token does read something
                if ( machine1.IsEnterChar() ) {
                    oneLine_of_Program.push_back( tempToken_of_line ) ;
                    tempToken_of_line.clear() ;
                    cout << '\n' ;
                }
                if ( token_string == ";" ) {
                    cout << endl ;
                    cout << syntaxAnalysis.SyntaxAnalysisStatement(tempToken_of_statement) << endl ;
                    opcode_table.PrintAllOPCode() ;
                    opcode_table.Clear() ;
                    tempToken_of_statement.clear() ;
                }    
            } while ( end ) ;   // this while is we can loading all token of page
        }
        catch ( exception& e ) {
            cerr << e.what() << " ," << "in line : " << __LINE__ << endl ;    
            throw  e.what() ;
        }        
    } 

    public: vector<Token> GetIndexLine( int i ) {
        return oneLine_of_Program[i].token_of_oneLine ;
    }    

    

} ;


class Compiler {

    vector<Error> error ;
    
} ;


int main(int argc,char **argv) {
    
    try {
        delimiter_table.PushNewToken("") ;
        command_table.PushNewToken("") ;    
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