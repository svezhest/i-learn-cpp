#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and fucking working clang++!"};
    
    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;

    bool first = true;
    cout << (first = false, true) << '\n';
    cout << first;

    return 0;
}
