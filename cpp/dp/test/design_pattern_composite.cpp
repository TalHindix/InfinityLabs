/*****************************************************************************
 * Exercise:    Design Pattern - Composite
 * Date:        12/11/2025
 * Developer:   Tal Hindi
 * Reviewer:
 * Status:
 *****************************************************************************/

#include <iostream> /* cout */
#include <vector>   /* vector */

using namespace std;

static size_t g_indent = 0;

/************************ FileSystem ***************************************/
class FileSystem
{
private:
    const string m_name;

public:
    virtual void Print() const = 0;
};

/***************************************************************/

class File: public FileSystem
{
public:
    void Print() const;
};


void File::Print() const
{
    for(size_t i =0; i < g_indent; ++i)
    {
        cout << "   " << endl;
    }
}

/******************************************************************** */

class Directory: public FileSystem
{
private:
    vector<FileSystem*> m_elements;
public:
    void Print() const;
    void AddEntry(FileSystem* element);
    void RemoveEntry(FileSystem* element);
};


void Directory::Print() const
{

}

void Directory::AddEntry(FileSystem* element)
{

}

void Directory::RemoveEntry(FileSystem* element)
{

}

int main(void)
{
    cout << "Create a file system..." << endl;

    return 0;
}
