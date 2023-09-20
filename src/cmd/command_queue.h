/*
 *  main.cpp
 *
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDClient.
 *
 *  IEDClient is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDClient is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDClient.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>

namespace Core::Cmd
{
	template<typename T>
	class CommandQueue
	{
	protected:
		std::queue< T >				m_queue;
		std::mutex					m_lock;
		std::condition_variable		m_condVar;
		bool						m_running;

	public:
		CommandQueue() {
			m_running = true;
		}

		bool	isRunning() {
			std::unique_lock<std::mutex> lock(m_lock);
			return m_running;
		}
		void	stop() {
			std::unique_lock<std::mutex> lock(m_lock);
			m_running = false;
			m_condVar.notify_all();
		}

		void	push(T cmd) {
			std::unique_lock<std::mutex> lock(m_lock);
			m_queue.push(cmd);
			m_condVar.notify_one();
		}

		T		pop() {
			std::unique_lock<std::mutex> lock(m_lock);
			while (m_running && m_queue.empty()) {
				m_condVar.wait_for(lock, std::chrono::milliseconds(100));
			}

			if (!m_running && m_queue.empty()) {
				return T();
			}

			T val = m_queue.front();
			m_queue.pop();
			return val;
		}
	};
}
