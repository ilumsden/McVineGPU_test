#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

#include <cuda_runtime.h>

#include "tests.hpp"
#include "SystemVars.hpp"

using namespace std;

const int num_keys = 2;
float atten;

int main(int argc, char **argv)
{
    if (argc == 1 || (string(argv[1]) == "-h" || string(argv[1]) == "--help"))
    {
        printf("Usage: ./TestSuite [flags]\n\nFlags:\n    -h/--help: Prints help info\n    -ti/--testIds [Ids]: Flag for specifying the IDs for the tests you want to run.\n\nTest IDs:\n    0: Test for Triangle Intersection\n");
        return 0;
    }
    if (string(argv[1]) != "-ti" && string(argv[1]) != "--testIds")
    {
        printf("Invalid argument: %s\n\n", argv[1]);
        printf("Usage: ./TestSuite [flags]\n\nFlags:\n    -h/--help: Prints help info\n    -ti/--testIds [Ids]: Flag for specifying the IDs for the tests you want to run.\n\nTest IDs:\n    0: Test for Triangle Intersection\n");
        return -1;
    }
    if (argc == 2)
    {
        printf("The -ti/--testIds flag requires IDs.\n");
        printf("Usage: ./TestSuite [flags]\n\nFlags:\n    -h/--help: Prints help info\n    -ti/--testIds [Ids]: Flag for specifying the IDs for the tests you want to run.\n\nTest IDs:\n    0: Test for Triangle Intersection\n");
        return -2;
    }
    vector<int> ids;
    bool invalid = false;
    for (int i = 2; i < argc; i++)
    {
        int num = stoi(argv[i]);
        if (num >= num_keys || num < 0)
        {
            printf("Invalid ID: %i\n", num);
            invalid = true;
        }
        else
        {
            ids.push_back(num);
        }
    }
    if (invalid)
    {
        printf("\n");
        printf("Usage: ./TestSuite [flags]\n\nFlags:\n    -h/--help: Prints help info\n    -ti/--testIds [Ids]: Flag for specifying the IDs for the tests you want to run.\n\nTest IDs:\n    0: Test for Triangle Intersection\n");
        return -3;
    }
    sort(ids.begin(), ids.end());
    vector<int>::iterator it = unique(ids.begin(), ids.end());
    ids.resize(distance(ids.begin(), it));
    atten = 0.01;
    for (int i : ids)
    {
        switch (i)
        {
            case 0: assert( test_intersectTriangle() == true ); break;
            case 1: assert( test_Vec3() == true ); break;
            // More cases will be added later.
            default: throw domain_error("Invalid ID got to execution"); return -4;
        }
    }
    return 0;
}