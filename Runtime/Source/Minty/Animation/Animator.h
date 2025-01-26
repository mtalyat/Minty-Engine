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

	/// <summary>
	/// Controls the state and animations of an object using a finite state machine.
	/// </summary>
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

		/// <summary>
		/// Sets the value of a variable within the state machine. If no variable with the given name exists, a new variable is created.
		/// </summary>
		/// <param name="name">The name of the variable to be set.</param>
		/// <param name="value">The new value to for the variable.</param>
		void set_variable(String const& name, Int const value)
		{
			m_fsm.set_variable(name, value);
		}

		/// <summary>
		/// Gets the value of a variable within the state machine.
		/// </summary>
		/// <param name="name">The name of the variable to get.</param>
		/// <returns>The value of the variable, or 0 if the variable does not exist.</returns>
		Int get_variable(String const& name) const
		{
			return m_fsm.get_variable(name);
		}

		/// <summary>
		/// Updates the state of this Animator based on the values of the variables.
		/// </summary>
		/// <returns>The UUID that corresponds to the Animation that is now playing.</returns>
		UUID update();

		/// <summary>
		/// Gets the Finite State Machine of this Animator.
		/// </summary>
		/// <returns>The internal FSM.</returns>
		FSM& get_fsm()
		{
			return m_fsm;
		}

		AssetType get_asset_type() const override { return AssetType::Animator; }

		static Owner<Animator> create(AnimatorBuilder const& builder = {}) { return Owner<Animator>(builder); }
	};
}