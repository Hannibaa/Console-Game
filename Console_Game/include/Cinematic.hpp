#pragma once
#include "Console_utility_1.hpp"


namespace cgu {

    namespace cin {  // cinematic


        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 
        //    Motion definition
        //    
        // 
        //////////////////////////////////////////////////////////////////////////////////////////////////////////

        class IMotion {
        protected:
            cgu::fPoint2d   m_speed;
            cgu::fRect      m_field;
            float           m_gravity;
        public:
            IMotion(cgu::fPoint2d speed = { 1.f, 1.f }
                , cgu::fRect   field = { 1.f, 1.f , 144.f, 34.f })
                :m_speed{ speed }
                , m_field{ field }
                , m_gravity{ 9.81f }
            {}

            void set_speed(float vx, float vy) {
                m_speed.x = vx;
                m_speed.y = vy;
            }

            cgu::fPoint2d get_speed() const {
                return m_speed;
            }

            void set_bounds(float x, float y, float dx, float dy) {
                m_field.x = x;
                m_field.y = y;
                m_field.dx = dx;
                m_field.dy = dy;
            }

            void set_bounds(const cgu::fRect& frect) {
                set_bounds(frect.x, frect.y, frect.dx, frect.dy);
            }

            cgu::fRect get_bounds() const {
                return m_field;
            }
        };

        class IControle {
        protected:
            short key_up{ VK_UP };
            short key_down{ VK_DOWN };
            short key_right{ VK_RIGHT };
            short key_left{ VK_LEFT };
            short key_fire;
            short key_up_1;
            short key_down_1;

        public:

            void set_controle_direction_key(const short(&controle)[4]) {
                key_up    = controle[0];
                key_down  = controle[1];
                key_right = controle[2];
                key_left  = controle[3];
            }

            void set_auxillary_key(const int(&controle)[3]) {
                key_fire = controle[0];
                key_up_1 = controle[1];
                key_down_1 = controle[2];
            }
        };

        // Motion 2d
        class Motion : public IMotion, public IControle {
        public:
            template<typename Tsprite>
            void operator()(Tsprite* ptr, float Dt) {
                cgu::fPoint2d  p = ptr->get_position();
                cgu::fPoint2d  dim = ptr->get_dimension();

                if (KeyPressed(key_down)) {
                    p.y += m_speed.y * Dt;
                }
                //p.y += f * m_speed.y / 2.5f;

                if (KeyPressed(key_up)) {
                    p.y -= m_speed.y * Dt;
                }

                if (KeyPressed(key_left)) {
                    p.x -= m_speed.x * Dt;
                }

                if (KeyPressed(key_right)) {
                    p.x += m_speed.x * Dt;
                }

                // ajust speed 
                if (KeyReleased(key_up_1)) {
                    m_speed.x += 0.1f;
                    m_speed.y += 0.1f;
                }
                if (KeyReleased(key_down_1)) {
                    m_speed.x -= 0.1f;
                    m_speed.y -= 0.1f;
                }

                p.clamped({ float(m_field.x), float(m_field.dx) - dim.x}, {float(m_field.y) , float(m_field.dy) - dim.y});

                ptr->set_position(p);
            }
        };


        // Generale Free Motion in box

        class FreeMotion : public cgu::cin::IMotion {

            float v1{ 1.0f };
            float v2{ 1.0f };

        public:

            template<typename TObj>
            void operator()(TObj* ptr, float Dt)
            {
                cgu::fPoint2d  p = ptr->get_position();
                cgu::fPoint2d  dim = ptr->get_dimension();

                p.x += m_speed.x * Dt * v1;
                p.y += m_speed.y * Dt * v2;

                if (p.x < m_field.x  || p.x > m_field.x + m_field.dx - dim.x + 2.f) v1 *= -1.f;
                if (p.y < m_field.y  || p.y > m_field.y + m_field.dy - dim.y + 2.f) v2 *= -1.f;

                p.clamped(m_field.x, m_field.x + m_field.dx + 1.f,
                    m_field.y, m_field.y + m_field.dy + 1.f);

                ptr->set_position(p);

            }

			void reverse(int _case = 0) {
				switch (_case) {
				case 0:
					v1 = -v1;
					v2 = -v2;
					break;
				case 1:
                    v1 = -v1;
                    break;
                case 2:
                    v2 = -v2;
                    break;
                default:
                    break;
				}

			}

        };

        // with free functions
        class FFreeMotion : public IMotion {

            using FunctionReal = std::function<float(float)>;

            FunctionReal  _X;
            FunctionReal  _Y;
            float         _t{0};

        public:
            FFreeMotion(FunctionReal X, FunctionReal Y)
                :_X(X)
                , _Y(Y)
            {}

            void initial_point(float t0) {
                _t = t0;
            }

            template<typename TMotionable>
            void operator()(TMotionable* ptr, float Dt) {
                cgu::fPoint2d  p = ptr->get_position();
                cgu::fPoint2d  dim = ptr->get_dimension();

                _t += Dt;
                p.x = _X(_t);
                p.y = _Y(_t);

                p.clamped(m_field.x, m_field.x + m_field.dx, m_field.y, m_field.y + m_field.dy);

                ptr->set_position(p);

            }
        };


    }  // end namespace Cinematic

}  // end namespace cgu