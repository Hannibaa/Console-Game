#pragma once
#include <unordered_map>
#include "sprite_v_3.h"
#include "image_text.h"



namespace cgu {

	using Image = cgu::ascii::AsciiImage;
	using VecImages = std::vector<Image>;
	using MapAnimation = std::unordered_map<std::string, VecImages>;

	class AnimatedSprite : public cgu::ISprite {

		MapAnimation       a_mapAnm;      // map of  animation like, walking, flying , firing
		VecImages          a_images;	  // 
		int                a_dimX{1};	  // 
		int                a_dimY{1};	  // 
		int                a_size{};	  // 
		float              a_speed;		  // 
		float              a_next{};	  // 
		float              a_index{};	  // 

		void init_images() {

		}

	public:
		AnimatedSprite()
			:AnimatedSprite(10,10)
		{}

		AnimatedSprite(int x, int y)
			:ISprite(std::wstring(),x,y)
		{}

		void set_dimension(int dimx, int dimy) {
			a_dimX = dimx;
			a_dimY = dimy;
		}

		void set_speed(float speed) {
			a_speed = speed;
		}

		void add_image(const std::wstring& image, int dimx, int dimy) {
			a_images.emplace_back(image, dimx, dimy);
			cgu::ascii::AsciiAjust(a_images[a_images.size() - 1]);
			a_size = a_images.size();
		}

		void add_image(const std::wstring& image, const std::string& animation, int dimx, int dimy) {
			a_mapAnm[animation].emplace_back(image, dimx, dimy);
			cgu::ascii::AsciiAjust(a_mapAnm[animation][a_mapAnm[animation].size() - 1]);
		}

		virtual cgu::iRect  get_bounds() const override {
			return cgu::iRect{};
		}

		virtual cgu::fPoint2d get_dimension() const override {
			return cgu::fPoint2d{};
		}

		////////////////////// animate without drawing methode
		void animate( float Dt) {
			a_next += a_speed * Dt;
			a_index = int(a_next) % a_size;
			a_dimX = a_images[a_index].dx;
			a_dimY = a_images[a_index].dy;
		}

		void reset_index() {
			a_next = 0.f;
			a_index = 0;
		}

		void animate(const std::string& name_animation, float Dt) {
			a_next += a_speed * Dt;
			a_index = int(a_next) % (a_mapAnm[name_animation].size());
			a_dimX = a_mapAnm[name_animation][a_index].dx;
			a_dimY = a_mapAnm[name_animation][a_index].dy;
		}

		virtual void draw() const override {
			if (!a_images.empty())
			cgu::ascii::draw_image(a_images[a_index], cgu::console, _pos.x, _pos.y);
		}

		void draw(const std::string& name_animation)  {
			if (!a_mapAnm.empty())
				cgu::ascii::draw_image(a_mapAnm[name_animation][a_index], cgu::console
					, _pos.x, _pos.y);
		}

		void draw(int index) const {
			cgu::ascii::draw_image(a_images[index], cgu::console, _pos.x, _pos.y);
		}

	};



}