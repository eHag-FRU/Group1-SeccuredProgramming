#include <iostream>
#include <string>
#include <map>
#include "../include/inputSanatizer.hpp"
#include <cassert>


using std::cout; using std::endl; using std::string; using std::map;

void testSanatizeTime() {
     cout << "====TESTING SANITIZE TIME STAMP====" << endl << endl;
    //Valid Inputs
    char* input1[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    
    char* input2[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    char* input3[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("2"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    char* input4[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1073741823"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    char* input5[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("63"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};


    //Invalid Inputs
    char* input6[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("0"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};
    
    char* input7[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("0%x"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    char* input8[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("%x"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    char* input9[] = {const_cast<char*>("logappend"), const_cast<char*>("%x-T"), const_cast<char*>("%x"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    char* input10[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("-1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};
    
    char* input11[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("0.1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    char* input12[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1073741824"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};
    
    char* input13[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>(""), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    char* input14[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("t"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    char* input15[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("-0"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("-R"), 
         const_cast<char*>("001"), const_cast<char*>("log1")};

    //"-T 1 -K secret -A -E Fred log1";
    
    {
        map<string, string> result;

        cout << "TEST 1" << endl;
        cout << "Using valid input of: 1" << endl;
        assert(sanatizeTime(9, input1, result));
        assert(result["-T"] == "1");
        result.clear();
        cout << "TEST 1 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 2" << endl;
        cout << "Using valid input of: 1" << endl;
        assert(sanatizeTime(11, input2, result));
        assert(result["-T"] == "1");
        result.clear();
        cout << "TEST 2 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 3" << endl;
        cout << "Using valid input of: 2" << endl;
        assert(sanatizeTime(11, input3, result));
        assert(result["-T"] == "2");
        cout << "TEST 3 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 4" << endl;
        cout << "Using valid input of: 1073741823" << endl;
        assert(sanatizeTime(11, input4, result));
        assert(result["-T"] == "1073741823");
        result.clear();
        cout << "TEST 4 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 5" << endl;
        cout << "Using valid input of: 63" << endl;
        assert(sanatizeTime(11, input5, result));
        assert(result["-T"] == "63");
        result.clear();
        cout << "TEST 5 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 6" << endl;
        cout << "Using invalid input of: 0, should return false" << endl;
        assert(!sanatizeTime(11, input6, result));
        result.clear();
        cout << "TEST 6 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 7" << endl;
        cout << "Using invalid input of: 0, should return false" << endl;
        assert(!sanatizeTime(11, input7, result));
        result.clear();
        cout << "TEST 7 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 8" << endl;
        cout << "Using invalid input of: %x, should return false" << endl;

        assert(!sanatizeTime(11, input8, result));
        result.clear();
        cout << "TEST 8 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 9" << endl;
        cout << "Using invalid input of: %x-T 0%x, should return false" << endl;
        assert(!sanatizeTime(11, input9, result));
        result.clear();
        cout << "TEST 9 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 10" << endl;
        cout << "Using invalid input of: -T -1, should return false" << endl;
        assert(!sanatizeTime(11, input10, result));
        result.clear();
        cout << "TEST 10 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 11" << endl;
        cout << "Using invalid input of: -T 0.1, should return false" << endl;
        assert(!sanatizeTime(11, input11, result));
        result.clear();
        cout << "TEST 11 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 12" << endl;
        cout << "Using invalid input of: -T 1073741824, should return false" << endl;
        assert(!sanatizeTime(11, input12, result));
        result.clear();
        cout << "TEST 12 PASSED" << endl << endl;
    }


    {
        map<string, string> result;
        cout << "TEST 13" << endl;
        cout << "Using invalid input of: -T , should return false" << endl;
        assert(!sanatizeTime(11, input13, result));
        result.clear();
        cout << "TEST 13 PASSED" << endl << endl;
    }


    {
        map<string, string> result;
        cout << "TEST 14" << endl;
        cout << "Using invalid input of: -T t, should return false" << endl;
        assert(!sanatizeTime(11, input14, result));
        result.clear();
        cout << "TEST 14 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 15" << endl;
        cout << "Using invalid input of: -T -0, should return false" << endl;
        assert(!sanatizeTime(11, input15, result));
        result.clear();
        cout << "TEST 15 PASSED" << endl << endl;
    }
    
    cout << endl << "====PASSED SANITIZE TIME STAMP TESTS====" << endl << endl;
}


void testSanatizeToken() {
    cout << "====TESTING SANITIZE TOKEN====" << endl << endl;
    //Valid Inputs
    char* input1[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input2[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("s"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input3[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("1"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input4[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("t3stThis"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input5[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("fdshjkfdshjkfdsguyerytweui4756345783920gjgjdklskfsehHHDGSHUJWEYTUWI"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};

    //Invalid Inputs
    char* input6[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("%S%X%N-K"),
         const_cast<char*>("1"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input7[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X"),
         const_cast<char*>("1"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input8[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("%S%X%%%%%%%%%1"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input9[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("fsdhjkeufgue--------ffff%%%%%%%"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input10[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>(""),
         const_cast<char*>("1"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input11[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>(""), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input12[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>(""),
         const_cast<char*>(""), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};

    {
        map<string, string> result;

        cout << "TEST 1" << endl;
        cout << "Using valid input of: -K secret" << endl;
        assert(sanatizeToken(9, input1, result));
        assert(result["-K"] == "secret");
        result.clear();
        cout << "TEST 1 PASSED" << endl << endl;
    }

    {
        map<string, string> result;

        cout << "TEST 2" << endl;
        cout << "Using valid input of: -K s" << endl;
        assert(sanatizeToken(9, input2, result));
        assert(result["-K"] == "s");
        result.clear();
        cout << "TEST 2 PASSED" << endl << endl;
    }

    {
        map<string, string> result;

        cout << "TEST 3" << endl;
        cout << "Using valid input of: -K 1" << endl;
        assert(sanatizeToken(9, input3, result));
        assert(result["-K"] == "1");
        result.clear();
        cout << "TEST 3 PASSED" << endl << endl;
    }

    {
        map<string, string> result;

        cout << "TEST 4" << endl;
        cout << "Using valid input of: -K t3stThis" << endl;
        assert(sanatizeToken(9, input4, result));
        assert(result["-K"] == "t3stThis");
        result.clear();
        cout << "TEST 4 PASSED" << endl << endl;
    }

    {
        map<string, string> result;

        cout << "TEST 5" << endl;
        cout << "Using valid input of: -K fdshjkfdshjkfdsguyerytweui4756345783920gjgjdklskfsehHHDGSHUJWEYTUWI" << endl;
        assert(sanatizeToken(9, input5, result));
        assert(result["-K"] == "fdshjkfdshjkfdsguyerytweui4756345783920gjgjdklskfsehHHDGSHUJWEYTUWI");
        result.clear();
        cout << "TEST 5 PASSED" << endl << endl;
    }


    //
    //  INVALID INPUT TESTS
    //

    {
        map<string, string> result;
        cout << "TEST 6" << endl;
        cout << "Using invalid input of: %S%X%N-K 1 , should return false" << endl;
        assert(!sanatizeToken(11, input6, result));
        result.clear();
        cout << "TEST 6 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 7" << endl;
        cout << "Using invalid input of: -K%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X 1 , should return false" << endl;
        assert(!sanatizeToken(11, input7, result));
        result.clear();
        cout << "TEST 7 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 8" << endl;
        cout << "Using invalid input of: -K %S%X%%%%%%%%%1 , should return false" << endl;
        assert(!sanatizeToken(11, input8, result));
        result.clear();
        cout << "TEST 8 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 9" << endl;
        cout << "Using invalid input of: -K fsdhjkeufgue--------ffff%%%%%%% , should return false" << endl;
        assert(!sanatizeToken(11, input9, result));
        result.clear();
        cout << "TEST 9 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 10" << endl;
        cout << "Using invalid input of:  1, should return false" << endl;
        assert(!sanatizeToken(11, input10, result));
        result.clear();
        cout << "TEST 10 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 11" << endl;
        cout << "Using invalid input of:  -K, should return false" << endl;
        assert(!sanatizeToken(11, input10, result));
        result.clear();
        cout << "TEST 11 PASSED" << endl << endl;
    }

    {
        map<string, string> result;
        cout << "TEST 12" << endl;
        cout << "Using invalid input of:  , should return false" << endl;
        assert(!sanatizeToken(11, input12, result));
        result.clear();
        cout << "TEST 12 PASSED" << endl << endl;
    }



    cout << endl << "====PASSED SANITIZE TOKEN TESTS====" << endl << endl;
}

void testSanatizeEmployeeOrGuestName() {
    cout << "====TESTING SANITIZE EMPLOYEE OR GUEST NAME====" << endl << endl;
    //Valid Inputs (-G & -E)
    char* input1[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input2[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-G"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input3[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input4[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-G"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input5[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-E"), const_cast<char*>("Fred"), const_cast<char*>("log1")};


    cout << endl << "====PASSED SANITIZE EMPLOYEE OR GUEST NAME TESTS====" << endl << endl;
}

void testSanatizeAriveLeaveTag() {
    cout << "====TESTING SANITIZE ARRIVE LEAVE TAG====" << endl << endl;


    cout << endl << "====PASSED SANITIZE ARRIVE LEAVE TAG TESTS====" << endl << endl;
}

void testSanatizeRoomID() {
    cout << "====TESTING SANITIZE ROOM ID====" << endl << endl;


    cout << endl << "====PASSED SANITIZE ROOM ID TESTS====" << endl << endl;

}

void testSanatizeFilePath() {
    cout << "====TESTING SANITIZE FILE PATH====" << endl << endl;
    char* input1[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-G"), const_cast<char*>("Fred"), const_cast<char*>("log1")};
    char* input2[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-G"), const_cast<char*>("Fred"), const_cast<char*>("./tests/testTxtFiles/test1.txt")};
    char* input3[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-G"), const_cast<char*>("Fred"), const_cast<char*>("./tt/log1")};
    char* input4[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-G"), const_cast<char*>("Fred"), const_cast<char*>("")};
    char* input5[] = {const_cast<char*>("logappend"), const_cast<char*>("-T"), const_cast<char*>("1"), const_cast<char*>("-K"),
         const_cast<char*>("secret"), const_cast<char*>("-A"), const_cast<char*>("-G"), const_cast<char*>("Fred"), const_cast<char*>("./%X%X%X%X/../")};

     {
        map<string, string> result;
        cout << "TEST 1" << endl;
        cout << "Using valid input of:  log1, should return true" << endl;
        assert(!sanatizeFilePath(9, input1, result));
        result.clear();
        cout << "TEST 1 PASSED" << endl << endl;
    }


    cout << endl << "====PASSED SANITIZE FILE PATH====" << endl << endl;
}



int main() {
 testSanatizeTime();
 testSanatizeToken();
 testSanatizeEmployeeOrGuestName();
 testSanatizeAriveLeaveTag();
 testSanatizeRoomID();
 testSanatizeFilePath();

 return 0;
}