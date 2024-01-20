#pragma once

#include "M_Object.h"
#include "M_ISerializable.h"
#include "M_AnimationBuilder.h"
#include <vector>

namespace minty
{
	class Animation
		: public Object
	{
	private:
		float _frameTime;

		std::vector<ID> _frames;

	public:
		/// <summary>
		/// Creates an empty animation.
		/// </summary>
		Animation();

		/// <summary>
		/// Creates an animation with the given frames.
		/// </summary>
		/// <param name="frames">A list of Sprite IDs.</param>
		/// <param name="frameTime">The time to elapse per frame.</param>
		Animation(AnimationBuilder const& builder);

		float get_frame_time() const;

		size_t get_frame_count() const;

		ID get_frame(size_t const index) const;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}