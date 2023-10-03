#pragma once

#include "logical_node.hpp"
#include <string>

class TLLN0 : public TLogicalNode
{
public:
	TLLN0(LogicalDevice *t_ld) : TLogicalNode(t_ld)
	{
		m_libLN = LogicalNode_create("LLN0", m_libLD);

		m_libMod = CDC_ENS_create("Mod", (ModelNode *)m_libLN, 0);
		m_libBeh = CDC_ENS_create("Beh", (ModelNode *)m_libLN, 0);
		m_libHealth = CDC_ENS_create("Health", (ModelNode *)m_libLN, 0);
	};

	~TLLN0() override 
	{
	}
};