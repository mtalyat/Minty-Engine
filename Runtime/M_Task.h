#pragma once

#include "M_Base.h"
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
		Task(std::function<T()> const& func)
		{
			task = std::async(std::launch::async, func);
		}

		bool valid()
		{
			return task.valid();
		}

		T get()
		{
			return task.get();
		}

		void wait()
		{
			task.wait();
		}

		bool done()
		{
			return wait_for(0.0f);
		}

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

		void create(std::function<T()> const& func)
		{
			Task<T> task(func);
			tasks.push_back(std::move(task));
		}

		void emplace(Task<T>& task)
		{
			tasks.push_back(std::move(task));
		}

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

		void wait()
		{
			for (auto& task : tasks)
			{
				task.wait();
			}
		}

		size_t size()
		{
			return tasks.size();
		}

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
		/// Refreshes all task groups and checks if they are completed.
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

		TaskGroup<T>* create(String const& name)
		{
			return create(name, [] {});
		}

		void destroy(String const& name)
		{
			if (!tasks.contains(name)) return;

			tasks.erase(name);
		}

		bool done(String const& name)
		{
			auto found = tasks.find(name);

			if (found == tasks.end()) return false; // no task found

			// check if group is done
			return found->second.group.done();
		}

		bool contains(String const& name)
		{
			return tasks.contains(name);
		}
	};
}