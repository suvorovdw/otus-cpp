#include "print_ip.h"

#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>

static void expect_eq(const std::string& got, const std::string& expected)
{
    if (got != expected)
    {
        std::cerr << "EXPECTED:\n" << expected << "\nGOT:\n" << got << '\n';
        std::abort();
    }
}

int main()
{
    {
        std::ostringstream os;
        my::print_ip(char(-1), os);
        os << '\n';
        my::print_ip(short(0), os);
        os << '\n';
        my::print_ip(int(2130706433), os);
        os << '\n';
        my::print_ip(long(8875824491850138409), os);
        os << '\n';

        const std::string expected =
            (sizeof(long) == 8)
                ? "255\n0.0\n127.0.0.1\n123.45.67.89.101.112.131.41\n"
                : "255\n0.0\n127.0.0.1\n101.112.131.41\n";
        expect_eq(os.str(), expected);
    }

    {
        std::ostringstream os;
        my::print_ip(std::string{"trololo"}, os);
        expect_eq(os.str(), "trololo");
    }

    {
        std::ostringstream os;
        my::print_ip(std::vector{1, 2, 3, 4}, os);
        expect_eq(os.str(), "1.2.3.4");
    }

    {
        std::ostringstream os;
        my::print_ip(std::list<std::string>{"print", "from", "list"}, os);
        expect_eq(os.str(), "print.from.list");
    }

    {
        std::ostringstream os;
        my::print_ip(std::make_tuple(1, 2, 3, 4), os);
        expect_eq(os.str(), "1.2.3.4");
    }

    return 0;
}

