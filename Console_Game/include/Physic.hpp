#pragma once
#include "Console_utility_1.hpp"
/*
     Physic 
     1. collision detection
     2. 
*/


namespace cgu {

	namespace  phy {   // namespace physics


        class Collision {
        public:
            template<typename TSprite>
            bool is_collid(TSprite& t) {
                cgu::fPoint2d pt = t.get_position();
                cgu::fPoint2d dimt = t.get_dimension();

                for (int i = 0; i != dimt.x; ++i)
                {
                    for (int j = 0; j != dimt.y; ++j)
                    {
                        int n = i + j * dimt.x;
                        if (t[n] != L' ')
                        {
                            if (cgu::console[pt.x + i + (pt.y + j) * cgu::console.get_length()] != L' ')
                            {
                                return true;
                                break;
                            }
                        }
                    }
                }

                return false;
            }
        };


	}  // end namespace physics



}  // namespace cgu