#include <iostream>
#include "Console_utility_1.hpp"
#include "MyLib/Console_Library/escape_code.h"


#include "MyLib/geometry2d_utility.h"

/*
         test function IsCloser()
*/

int main() {

    cu::fPoint2d  p(10.f, 10.f), p2(12.f, 11.5f);

    print_ << "morm of p2: " << geom2d::Norm(p2) << end_;
    print_ << "norm of p : " << geom2d::Norm(p.x, p.y) << end_;
    print_ << "distance p - p2 : " << geom2d::Norm(p - p2) << end_;
    print_ << " is closer to " << geom2d::IsCloser(p, p2, 3.5f) << end_;


    wait_;
    return 0;
}
