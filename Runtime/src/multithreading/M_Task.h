#pragma once

#include "core/M_Base.h"
#include <vector>
#include <unordered_map>
#include <future>
#include <functional>
#include <chrono>

namespace minty
{
	/// <summary>
	/// Represents one Task, which is an operation of data on another thread.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class Task
	{
	private:
		std::future<T> task;

	public:
		/// <summary>
		/// Creates a new Task with the given functionality.
		/// </summary>
		/// <param name="func"></param>
		Task(std::function<T()> const& func)
		{
			task = std::async(std::launch::async, func);
		}

		/// <summary>
		/// Checks if this Task is valid.
		/// </summary>
		/// <returns></returns>
		bool valid()
		{
			return task.valid();
		}

		/// <summary>
		/// Gets the value of this Task.
		/// </summary>
		/// <returns></returns>
		T get()
		{
			return task.get();
		}

		/// <summary>
		/// Waits for this Task to be complete.
		/// </summary>
		void wait()
		{
			task.wait();
		}

		/// <summary>
		/// Checks if this Task has been completed.
		/// </summary>
		/// <returns></returns>
		bool done()
		{
			return wait_for(0.0f);
		}

		/// <summary>
		/// Waits X seconds for the task to be completed.
		/// </summary>
		/// <param name="seconds"></param>
		/// <returns>True if the task was completed in time, otherwise false.</returns>
		bool wait_for(float const seconds)
		{
			return task.wait_for(std::chrono::duration<float>(seconds)) == std::future_status::ready;
		}
	};

	/// <summary>
	/// Represents a collection of Tasks that can be operated on in a group.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class TaskGroup
	{
	private:
		std::vector<Task<T>> tasks;

	public:
		TaskGroup()
		{}

		/// <summary>
		/// Creates a new Task within this TaskGroup.
		/// </summary>
		/// <param name="func"></param>
		void create(std::function<T()> const& func)
		{
			Task<T> task(func);
			tasks.push_back(std::move(task));
		}

		/// <summary>
		/// Moves a Task into this TaskGroup.
		/// </summary>
		/// <param name="task"></param>
		void emplace(Task<T>& task)
		{
			tasks.push_back(std::move(task));
		}

		/// <summary>
		/// Checks if all Tasks within this TaskGroup are complete.
		/// </summary>
		/// <returns></returns>
		bool done()
		{
			for (auto& task : tasks)
			{
				if (!task.done())
				{
					return false;
				}
			}

			return true;
		}

		/// <summary>
		/// Gets all of the values of each Task within this TaskGroup.
		/// </summary>
		/// <returns></returns>
		std::vector<T> get()
		{
			std::vector<T> results;
			results.reserve(tasks.size());

			for (auto& task : tasks)
			{
				results.push_back(task.get());
			}

			return results;
		}

		/// <summary>
		/// Waits for all Tasks within this TaskGroup to finish.
		/// </summary>
		void wait()
		{
			for (auto& task : tasks)
			{
				task.wait();
			}
		}

		/// <summary>
		/// Gets the number of Tasks within this TaskGroup.
		/// </summary>
		/// <returns></returns>
		size_t size()
		{
			return tasks.size();
		}

		/// <summary>
		/// Gets the Task at the given index.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		Task<T>& at(size_t index)
		{
			return tasks.at(index);
		}
	};

	/// <summary>
	/// Handles operations on many tasks.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class TaskFactory
	{
	private:
		struct TaskData
		{
			TaskGroup<T> group;
			std::function<void()> onComplete;
		};

		std::unordered_map<String, TaskData> tasks;

	public:
		TaskFactory()
		{}

		/// <summary>
		/// Refreshes all task groups and checks if they are completed. If they are, they will have their onComplete function called, and be destroyed.
		/// </summary>
		void update()
		{
			std::vector<String> completed;

			for (auto& data : tasks)
			{
				if (data.second.group.done())
				{
					// all done
					data.second.onComplete();

					completed.push_back(data.first);
				}
			}

			for (auto const& name : completed)
			{
				destroy(name);
			}
		}

		/// <summary>
		/// Creates a new TaskGroup, if able.
		/// </summary>
		/// <param name="name">The unique name identifier of the group.</param>
		/// <param name="onComplete">The function to be called after the group has finished.</param>
		/// <returns>The newly created TaskGroup, or nullptr if one already exists with the name.</returns>
		TaskGroup<T>* create(String const& name, std::function<void()> const& onComplete)
		{
			// only create if name not taken
			if (tasks.contains(name)) return nullptr;

			TaskGroup<T> taskGroup{};
			TaskData data
			{
				.group = std::move(taskGroup),
				.onComplete = onComplete
			};
			tasks.emplace(name, std::move(data));
			return &tasks.at(name).group;
		}

		/// <summary>
		/// Creates a new TaskGroup, if able.
		/// </summary>
		/// <param name="name">The unique name identifier of the group.</param>
		/// <returns>The newly created TaskGroup, or nullptr if one already exists with the name.</returns>
		TaskGroup<T>* create(String const& name)
		{
			return create(name, [] {});
		}

		/// <summary>
		/// Removes a TaskGroup from this TaskFactory.
		/// </summary>
		/// <param name="name"></param>
		void destroy(String const& name)
		{
			if (!tasks.contains(name)) return;

			tasks.erase(name);
		}

		/// <summary>
		/// Checks if the TaskGroup with the given name is completed.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		bool done(String const& name)
		{
			auto found = tasks.find(name);

			if (found == tasks.end()) return false; // no task found

			// check if group is done
			return found->second.group.done();
		}

		/// <summary>
		/// Checks if there is a TaskGroup with the given name within this TaskFactory.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		bool contains(String const& name)
		{
			return tasks.contains(name);
		}
	};
}