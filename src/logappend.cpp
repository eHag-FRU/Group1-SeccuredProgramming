#include <iostream>
#include <regex>


/*
Command syntax

logappend -T <timestamp> -K <token> (-E <employee-name> | -G <guest-name>) (-A | -L) [-R <room-id>] <log>


logappend -B <file>

*/

using std::cout; using std::endl; using std::regex; using std::regex_match;

void sanatize(int argumentCount, char* arguments[]) {
    //char* sanatizedOutput[];
    //Make the regex matcher
    regex timeStampDash("-T",  std::regex_constants::ECMAScript);

    //Time stamp check
    //Check if it matches
    if (regex_match(arguments[1], timeStampDash)) {
        cout << "-T MATCH!" << endl;
    }




}


bool validCommandLineArgumentFlags(char**) {
    //Check for two situations
    return false;
}

void batchRunner(char*) {
    //Batch file runner
    cout << "In Batch runner" << endl;
}

int main(int argc, char* argv[]) {
    sanatize(argc, argv);


    //Print out the args, see what splitting it does
   for (int i = 0; i < argc; i++) {
	cout << argv[i] << endl;
   }

        //Batch runner
        if (argc == 3) {
            //Check for the -B
            cout << "argv[1]: " << argv[1] << endl;
            cout << "argv[1] == \"-B\": " << (argv[1] == "-B") << endl;

            if (argv[1] == "-B") {
                batchRunner(argv[2]);
            } else{
                //Not valid command line arguments
                cout << "Invalid" << endl;
                return 255;
            }
        } else if (argc == 10 || argc == 11) {
           //Not valid command line arguments
            // cout << "Invalid" << endl;
            // return 255; 

            //Longer command used
            //validCommandLineArgumentFlags(argv);
        }
    return 0;
}