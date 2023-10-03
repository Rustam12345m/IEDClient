#pragma once

extern "C"
{
	#include "iec61850_server.h"
}

#include "lln0_lnode.hpp"
#include "lphd_lnode.hpp"
#include "ggio_lnode.hpp"

#include <list>

class TLogicalDevice
{
public:
	TLogicalDevice(IedModel *t_model, const std::string &t_name)
	{
		m_libModel = t_model;
		m_libLD = LogicalDevice_create(t_name.c_str(), m_libModel);
	}
	~TLogicalDevice() {
		for (auto *it : m_lnodes) {
			delete it;
		}
		m_lnodes.clear();
	}

	LogicalDevice* 		lib() const { return m_libLD; }

	void 				push(TLogicalNode *t_node) {
		m_lnodes.push_back(t_node);
	}

protected:
	IedModel* 					m_libModel = nullptr;
	LogicalDevice* 				m_libLD = nullptr;
	std::list<TLogicalNode*>	m_lnodes;
};