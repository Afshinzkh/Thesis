

#include "helper.h"

int readFile( std::string data)
{
    using namespace std;
    using namespace boost;
    // string data("data.csv");

    ifstream in(data.c_str());
    if (!in.is_open()) return 1;

    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    string line;

    while (getline(in,line))
    {
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());

        // vector now contains strings from one row, output to cout here
        copy(vec.begin(), vec.end(), ostream_iterator<string>(cout, "|"));

        cout << "\n----------------------" << endl;
    }
}
