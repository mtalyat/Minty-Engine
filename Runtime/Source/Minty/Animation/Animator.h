#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/FSM/FiniteStateMachine.h"

namespace Minty
{
	/// <summary>
	/// Holds data to create a new Animator.
	/// </summary>
	struct AnimatorBuilder
	{
		UUID id;

		/// <summary>
		/// The finite state machine within the Animator.
		/// </summary>
		FSM fsm;
	};

	class Animator
		: public Asset
	{
	private:
		FSM m_fsm;

	public:
		Animator(AnimatorBuilder const& builder)
			: Asset(builder.id)
			, m_fsm(builder.fsm)
		{}

		void set_variable(String const& name, Int const value)
		{
			m_fsm.set_variable(name, value);
		}

		Int get_variable(String const& name) const
		{
			return m_fsm.get_variable(name);
		}

		UUID update();

		FSM& get_fsm()
		{
			return m_fsm;
		}

		AssetType get_type() const override { return AssetType::Animator; }

		static Owner<Animator> create(AnimatorBuilder const& builder = {}) { return Owner<Animator>(builder); }
	};
}