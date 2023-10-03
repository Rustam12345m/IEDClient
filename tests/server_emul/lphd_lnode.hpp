#pragma once

#include "logical_node.hpp"
#include <string>

class TLPHD : public TLogicalNode
{
public:
	TLPHD(LogicalDevice *t_ld, const std::string &t_name) : TLogicalNode(t_ld)
	{
		m_libLN = LogicalNode_create(t_name.c_str(), m_libLD);

		m_libMod = CDC_ENS_create("Mod", (ModelNode *)m_libLN, 0);
		m_libBeh = CDC_ENS_create("Beh", (ModelNode *)m_libLN, 0);
		m_libHealth = CDC_ENS_create("Health", (ModelNode *)m_libLN, 0);
	};

	~TLPHD() override 
	{
	}
};